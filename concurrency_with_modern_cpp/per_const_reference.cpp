// implicit copying of a per-reference thread argument
#include <thread>

using namespace std;

class non_copyable_class
{
public:
	non_copyable_class() = default;
	non_copyable_class(non_copyable_class const &) = delete;
	non_copyable_class & operator=(non_copyable_class const &) = delete;
};

void per_const_reference(non_copyable_class const & non_copy) {}

int main(int argc, char * argv[])
{
	non_copyable_class non_copy;
	per_const_reference(non_copy);
	//thread t{per_const_reference, non_copy};  // error, copy not allowed
	thread t{per_const_reference, cref(non_copy)};
	t.join();
	return 0;
}
