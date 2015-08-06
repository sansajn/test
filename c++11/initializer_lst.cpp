// implementacia initializer listu
#include <initializer_list>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

template <typename T>
T num_double(T const & x) {return x*x;}

class foo
{
public:
	foo(initializer_list<int> lst)
	{
		for (auto item : lst)
			_nums.push_back(num_double(item));
	}

	void append(initializer_list<int> lst)
	{
		for (auto item : lst)
			_nums.push_back(num_double(item));
	}

	string str()
	{
		ostringstream ostr;
		ostr << "[";
		for (int i = 0; i < _nums.size()-1; ++i)
			ostr << _nums[i] << ", ";
		ostr << _nums.back() << "]";
		return ostr.str();
	}

private:
	vector<int> _nums;
};

int main(int argc, char * argv[])
{
	foo f{1, 2, 3, 4, 5};
	cout << "f = " << f.str() << "\n";
	cout << "append 6 and 7\n";
	f.append({6, 7});
	cout << "f = " << f.str() << "\n";
	return 0;
}

