#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include <stdexcept>

// External service that we want to mock (e.g., database, web API, file system)
class DataRepository {
public:
	bool connect(const std::string& connectionString) {
		// Real implementation would connect to database
		return !connectionString.empty();
	}
	
	std::string fetchUserData(int userId) {
		// Real implementation would query database
		if (userId <= 0) return "";
		return "user_" + std::to_string(userId) + "_data";
	}
	
	bool saveResult(int userId, const std::string& result) {
		// Real implementation would save to database
		return userId > 0 && !result.empty();
	}
};

// Mock for the external service
class MockDataRepository {
public:
	MOCK_METHOD(bool, connect, (const std::string& connectionString));
	MOCK_METHOD(std::string, fetchUserData, (int userId));
	MOCK_METHOD(bool, saveResult, (int userId, const std::string& result));
};

// Business logic class that we want to test (this has real logic worth testing!)
template<typename RepositoryType>
class UserService {
private:
	RepositoryType* repo;
	bool isConnected = false;
	
public:
	explicit UserService(RepositoryType* repository) : repo(repository) {}
	
	bool initialize(const std::string& connectionString) {
		if (connectionString.empty()) {
			return false;
		}
		isConnected = repo->connect(connectionString);
		return isConnected;
	}
	
	std::string processUserData(int userId) {
		if (!isConnected) {
			throw std::runtime_error("Service not initialized");
		}
		
		if (userId <= 0) {
			return "ERROR: Invalid user ID";
		}
		
		std::string rawData = repo->fetchUserData(userId);
		if (rawData.empty()) {
			return "ERROR: User not found";
		}
		
		// Business logic: uppercase the data and add prefix
		std::string processed = "PROCESSED_" + rawData;
		std::transform(processed.begin(), processed.end(), processed.begin(), ::toupper);
		
		// Save the processed result
		if (!repo->saveResult(userId, processed)) {
			return "ERROR: Failed to save result";
		}
		
		return processed;
	}
	
	bool isServiceReady() const {
		return isConnected;
	}
};

// Test class
class UserServiceTest : public ::testing::Test {
protected:
	void SetUp() override {
		service = std::make_unique<UserService<MockDataRepository>>(&mockRepo);
	}
	
	MockDataRepository mockRepo;
	std::unique_ptr<UserService<MockDataRepository>> service;
};

// Test 1: Service becomes ready after successful initialization
TEST_F(UserServiceTest, ServiceBecomesReadyAfterValidInitialization) {
	// Setup mock to succeed when any connection is attempted
	EXPECT_CALL(mockRepo, connect(::testing::_))
		.WillOnce(::testing::Return(true));
	
	bool result = service->initialize("valid_connection");
	
	EXPECT_TRUE(result);
	EXPECT_TRUE(service->isServiceReady());
}

// Test 2: Service rejects empty connection strings
TEST_F(UserServiceTest, ServiceRejectsEmptyConnectionString) {
	bool result = service->initialize("");
	
	EXPECT_FALSE(result);
	EXPECT_FALSE(service->isServiceReady());
}

// Test 3: Service handles repository connection failures
TEST_F(UserServiceTest, ServiceHandlesConnectionFailure) {
	EXPECT_CALL(mockRepo, connect("failing_connection"))
		.WillOnce(::testing::Return(false));
	
	bool result = service->initialize("failing_connection");
	
	EXPECT_FALSE(result);
	EXPECT_FALSE(service->isServiceReady());
}

// Test 4: Service processes user data correctly (main business logic)
TEST_F(UserServiceTest, ServiceProcessesUserDataCorrectly) {
	// Given: service is ready and repository returns user data
	EXPECT_CALL(mockRepo, connect(::testing::_))
		.WillOnce(::testing::Return(true));
	EXPECT_CALL(mockRepo, fetchUserData(123))
		.WillOnce(::testing::Return("user_123_data"));
	EXPECT_CALL(mockRepo, saveResult(123, "PROCESSED_USER_123_DATA"))
		.WillOnce(::testing::Return(true));
	
	service->initialize("test_connection");
	
	// When: processing user data
	std::string result = service->processUserData(123);
	
	// Then: data is transformed correctly (business logic verification)
	EXPECT_EQ(result, "PROCESSED_USER_123_DATA");
}

// Test 5: Service requires initialization before processing
TEST_F(UserServiceTest, ServiceRequiresInitializationBeforeProcessing) {
	// When: trying to process without initialization
	// Then: should throw exception
	EXPECT_THROW(service->processUserData(123), std::runtime_error);
}

// Test 6: Service validates user IDs
TEST_F(UserServiceTest, ServiceValidatesUserIds) {
	// Given: service is ready
	EXPECT_CALL(mockRepo, connect(::testing::_))
		.WillOnce(::testing::Return(true));
	service->initialize("test_connection");
	
	// When/Then: invalid user IDs return error messages
	EXPECT_EQ(service->processUserData(0), "ERROR: Invalid user ID");
	EXPECT_EQ(service->processUserData(-1), "ERROR: Invalid user ID");
	EXPECT_EQ(service->processUserData(-100), "ERROR: Invalid user ID");
}

// Test 7: Service handles missing users gracefully
TEST_F(UserServiceTest, ServiceHandlesMissingUsers) {
	// Given: service is ready but user doesn't exist
	EXPECT_CALL(mockRepo, connect(::testing::_))
		.WillOnce(::testing::Return(true));
	EXPECT_CALL(mockRepo, fetchUserData(999))
		.WillOnce(::testing::Return(""));  // Empty = user not found
	
	service->initialize("test_connection");
	
	// When: processing non-existent user
	std::string result = service->processUserData(999);
	
	// Then: appropriate error message is returned
	EXPECT_EQ(result, "ERROR: User not found");
}

// Test 8: Service handles save failures gracefully
TEST_F(UserServiceTest, ServiceHandlesSaveFailures) {
	// Given: service ready, user exists, but save fails
	EXPECT_CALL(mockRepo, connect(::testing::_))
		.WillOnce(::testing::Return(true));
	EXPECT_CALL(mockRepo, fetchUserData(456))
		.WillOnce(::testing::Return("user_456_data"));
	EXPECT_CALL(mockRepo, saveResult(456, ::testing::_))
		.WillOnce(::testing::Return(false));  // Save fails
	
	service->initialize("test_connection");
	
	// When: processing user but save fails
	std::string result = service->processUserData(456);
	
	// Then: appropriate error message is returned
	EXPECT_EQ(result, "ERROR: Failed to save result");
}

int main(int argc, char **argv) {
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}
