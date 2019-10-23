// generic function object implemented by generic lambda
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

int main(int argc, char * argv[])
{
	vector<person> persons;
	count_if(persons.begin(), persons.end(), 
		[limit = 42](person const & p){
			return p.age() > limit;
		});
	
	return 0;
}


int person::age() const
{
	return _age;
}
