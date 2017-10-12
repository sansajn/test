// implementacia atopmic_counter-u pomocou std::atomic
#include <atomic>
#include <iostream>

using std::cout;

class atomic_counter
{
public:
	atomic_counter() : _val{0} {}
	void increment() {++_val;}
	void decrement() {--_val;}
	int value() {return _val.load();}

private:
	std::atomic<int> _val;
};

int main(int argc, char * argv[]) 
{
	atomic_counter count;
	count.increment();
	count.increment();
	count.increment();
	count.increment();
	cout << "counter-value: " << count.value() << std::endl;

	return 0;
}
