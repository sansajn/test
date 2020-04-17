/*! \file uniqueptr.cpp 
basic unique_ptr usage (creation and assignment) */

#include <memory>
#include <functional>
#include <iostream>

using std::unique_ptr;
using std::function;
using std::cout;

unique_ptr<int[]> make_sequence(int n)
{
	unique_ptr<int[]> p{new int[n]};
	for (int i = 0; i < n; ++i)
		p[i] = i;
	return p;
}

void plain_deleter(int * p) {delete p;}


int main(int argc, char * argv[])
{
	unique_ptr<int> u1{new int(15)};
	unique_ptr<int[]> u2{make_sequence(10)};
	unique_ptr<int[]> u3 = make_sequence(10);

	cout << "u1:" << *u1 << "\n";
	
	cout << "u2:[";
	for (int i = 0; i < 10; ++i)
		cout << u2[i] << ", ";
	cout << "]\n";
	
	// plain function
	unique_ptr<int, void (*)(int *)> u4{new int{101}, plain_deleter};
	
	// with lambda deleter
	auto deleter = [](int * p){delete p;};
	unique_ptr<int, decltype(deleter)> u5{new int{11}, deleter};
	
	// with function deleter
	unique_ptr<int, function<void (int *)>> u6{new int{12}, [](int * p){delete p;}};
	
	// assignment
	unique_ptr<int> u7;
	u7.reset(new int{14});
	
	u7 = unique_ptr<int>{new int{15}};
	
	u7 = nullptr;  // release u7 content
	

	return 0;
}
