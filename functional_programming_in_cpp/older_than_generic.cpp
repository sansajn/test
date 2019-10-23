// creating a function object with a generic call operator (listing 3.1)
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;


class person 
{
public:
	person(int age) : _age{age} {}
	int age() const;
	
private:
	int _age;
};

class older_than
{
public:
	older_than(int limit) : _limit{limit}
	{}
	
	template <typename T>
	bool operator()(T && obj) const
	{
		return forward<T>(obj).age() > _limit;
	}
	
private:
	int _limit;
};

int main(int argc, char * argv[])
{
	vector<person> persons;
	count_if(persons.begin(), persons.end(), older_than{42});
	
	return 0;
}


int person::age() const
{
	return _age;
}
