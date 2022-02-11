/*! \file uniqueptr.cpp 
basic unique_ptr usage (creation and assignment) */

#include <memory>
#include <functional>
#include <iostream>

using std::unique_ptr;
using std::make_unique;
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

void close_file(std::FILE * f) {std::fclose(f);}

struct free_delete {
	void operator()(void * p) {std::free(p);}
};

struct brdf {};

struct lambertian : public brdf {};

brdf * make_lambertian() {return new lambertian;}

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

	unique_ptr<std::FILE, decltype(&close_file)> f{std::fopen("demo.txt", "r"), &close_file};
	
	// with lambda deleter
	auto deleter = [](int * p){delete p;};
	unique_ptr<int, decltype(deleter)> u5{new int{11}, deleter};
	
	// with function deleter
	unique_ptr<int, function<void (int *)>> u6{new int{12}, [](int * p){delete p;}};

	// with functor deleter
	unique_ptr<int, free_delete> u_free{(int *)malloc(sizeof(int))};
	
	// assignment
	unique_ptr<int> u7;
	u7.reset(new int{14});
	
	u7 = unique_ptr<int>{new int{15}};
	
	u7 = nullptr;  // release u7 content
	
	unique_ptr<lambertian> u8;
	u8.reset(static_cast<lambertian *>(make_lambertian()));  // downcast

	return 0;
}
