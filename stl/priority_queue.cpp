// test std::priority_queue struktury
#include <queue>
#include <functional>
#include <vector>
#include <iostream>

using std::priority_queue;
using std::greater;
using std::vector;
using std::cout;

struct foo
{
	int delay;
};

template <typename T>
void print_element(T const & t)
{
	cout << t;
}

template <typename Q>
void print_queue(Q & q)
{
	while (!q.empty())
	{
		print_element(q.top());
		cout << " ";
		q.pop();
	}
	cout << "\n";
}

template <>
void print_element<foo>(foo const & f)
{
	cout << f.delay;
}

struct foo_delay_compare
{
	bool operator()(foo const & a, foo const & b) {return a.delay > b.delay;}
};

int main(int argc, char * argv[])
{
	// standard p-queue
	priority_queue<int> q;
	for (int e : {6, 3, 8, 1, 9, 2, 5, 7, 4})
		q.push(e);
	print_queue(q);

	// p-queue with custom compare
	priority_queue<int, vector<int>, greater<int>> q2;
	for (int e : {6, 3, 8, 1, 9, 2, 5, 7, 4})
		q2.push(e);
	print_queue(q2);

	// p-queue with custom structure and lambda compare
	auto foo_cmp = [](foo const & a, foo const & b){return a.delay > b.delay;};
	priority_queue<foo, vector<foo>, decltype(foo_cmp)> q3{foo_cmp};  // lambda can't be default constructed
	for (int e : {6, 3, 8, 1, 9, 2, 5, 7, 4})
	{
		foo f;
		f.delay = e;
		q3.push(f);
	}
	print_queue(q3);

	// q-queue with custom structure
	priority_queue<foo, vector<foo>, foo_delay_compare> q4;
	for (int e : {6, 3, 8, 1, 9, 2, 5, 7, 4})
	{
		foo f;
		f.delay = e;
		q4.push(f);
	}
	print_queue(q4);

	return 0;
}
