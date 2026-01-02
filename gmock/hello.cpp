#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

// Simple class without virtual functions (concrete implementation)
class Calculator {
public:
	int add(int a, int b) {
		return a + b;
	}
	
	int multiply(int a, int b) {
		return a * b;
	}
	
	std::string getVersion() {
		return "Calculator v1.0";
	}
};

// Mock class that mimics the Calculator interface
class MockCalculator {
public:
	MOCK_METHOD(int, add, (int a, int b));
	MOCK_METHOD(int, multiply, (int a, int b));
	MOCK_METHOD(std::string, getVersion, ());
};

// Template-based class that can work with any calculator-like object
template<typename CalculatorType>
class MathProcessor {
private:
	CalculatorType* calc;
public:
	explicit MathProcessor(CalculatorType* calculator) : calc(calculator) {}
	
	int processAddition(int x, int y) {
		return calc->add(x, y);
	}
	
	int processMultiplication(int x, int y) {
		return calc->multiply(x, y);
	}
	
	std::string getCalculatorInfo() {
		return calc->getVersion();
	}
};

// Test with mock (no inheritance needed)
TEST(GMockModernTest, TestWithMock) {
	MockCalculator mockCalc;
	MathProcessor processor(&mockCalc);
	
	// Set expectations
	EXPECT_CALL(mockCalc, add(2, 3))
		.WillOnce(::testing::Return(5));
	
	EXPECT_CALL(mockCalc, getVersion())
		.WillOnce(::testing::Return("Mock Calculator v2.0"));
	
	// Test the functionality
	int result = processor.processAddition(2, 3);
	std::string version = processor.getCalculatorInfo();
	
	// Verify results
	EXPECT_EQ(result, 5);
	EXPECT_EQ(version, "Mock Calculator v2.0");
}

// Test with real implementation (same template works!)
TEST(GMockModernTest, TestWithRealCalculator) {
	Calculator realCalc;
	MathProcessor processor(&realCalc);
	
	// Test with real implementation
	int result = processor.processAddition(2, 3);
	std::string version = processor.getCalculatorInfo();
	
	EXPECT_EQ(result, 5);
	EXPECT_EQ(version, "Calculator v1.0");
}

// Test with argument matchers
TEST(GMockModernTest, TestWithMatchers) {
	MockCalculator mockCalc;
	MathProcessor processor(&mockCalc);
	
	// Use argument matchers
	EXPECT_CALL(mockCalc, multiply(::testing::_, ::testing::Gt(0)))
		.WillOnce(::testing::Return(42));
	
	int result = processor.processMultiplication(6, 7);
	EXPECT_EQ(result, 42);
}

int main(int argc, char **argv) {
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}