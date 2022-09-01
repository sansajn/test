/*! Atomic shared_ptr<> sample taken from *C++20 The Complete Guide*, 
chapter 12, sample `lib/atomicshared.cpp` 
\note not buildable under GCC 11, but ok with GCC 12, also not buildable under clang 14 */
#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <vector>
using std::shared_ptr, std::make_shared;
using std::string, std::to_string, std::vector;
using std::jthread, std::atomic;
using std::cout;
namespace this_thread = std::this_thread;
using namespace std::literals;  // for duration literals

template <typename T>
class atomic_list {
private:
	struct node {
		T val;
		shared_ptr<node> next;
	};

public:
	atomic_list() = default;

	void insert(T v) {
		auto p = make_shared<node>();
		p->val = v;
		p->next = head;
		while (!head.compare_exchange_weak(p->next, p))  // atomic update
			;
	}

	void print() const {
		cout << "HEAD";
		for (auto p = head.load(); p; p = p->next)  // atomic read
			cout << "->" << p->val;
		cout << std::endl;
	}

private:
	atomic<shared_ptr<node>> head;
};


int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[]) {
	atomic_list<string> alist;

	// populate list by elements from 10 threads
	{
		vector<jthread> pool;
		for (int i = 0; i < 100; ++i) {
			pool.push_back(jthread{
				[&,i]{
					for (auto s : {"hi", "hey", "ho", "last"}) {
						alist.insert(to_string(i) + s);
						this_thread::sleep_for(5ns);
					}
				}
			});
		}
	}  // wait for all threads to finish

	alist.print();

	return 0;
}
