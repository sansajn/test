#include <memory>
#include <atomic>
using std::atomic;
using std::unique_ptr;

int main(int argc, char * argv[]) {
	atomic<unique_ptr<int>> a;
	return 0;
}