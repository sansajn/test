// Google test fixture sample
#include <queue>
#include <gtest/gtest.h>

template <typename E> // E is the element type.
class queue {
public:
	queue() = default;
	
	void enqueue(E const & element) {
		_data.push(new E{element});
	}
	
	E * dequeue() {  // Returns nullptr if the queue is empty.
		if (!_data.empty()) {
			E * first = _data.front();
			_data.pop();
			return first;
		}
		else
			return nullptr;
	}

	[[nodiscard]] size_t size() const {return std::size(_data);}

private:
	std::queue<E*> _data;
};

class queue_test : public ::testing::Test {
protected:
	void SetUp() override {
		// _q0 remains empty
		_q1.enqueue(1);
		_q2.enqueue(2);
		_q2.enqueue(3);
	}

	// void TearDown() override {}

	queue<int> _q0;
	queue<int> _q1;
	queue<int> _q2;
};


TEST_F(queue_test, IsEmptyInitially) {
	EXPECT_EQ(_q0.size(), 0);
}

TEST_F(queue_test, DequeueWorks) {
	int * n = _q0.dequeue();
	EXPECT_EQ(n, nullptr);

	n = _q1.dequeue();
	ASSERT_NE(n, nullptr);
	EXPECT_EQ(*n, 1);
	EXPECT_EQ(_q1.size(), 0);
	delete n;

	n = _q2.dequeue();
	ASSERT_NE(n, nullptr);
	EXPECT_EQ(*n, 2);
	EXPECT_EQ(_q2.size(), 1);
	delete n;
}

int main(int argc, char * argv[]) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
