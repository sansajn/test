// insert_or_assign() sample (for map)

#include <map>
#include <string>
#include <iostream>
using std::map, std::string, std::cout;

struct user
{
	string name;

	user(string s) : name{move(s)}
	{
		cout << "user::user(" << name << ")\n";
	}

	~user()
	{
		cout << "user::~user(" << name << ")\n";
	}

	user(user const & u) : name{u.name}
	{
		cout << "user::user(copy, " << name << ")\n";
	}

	friend bool operator<(user const & a, user const & b)
	{
		return a.name < b.name;
	}
};

int main(int argc, char * argv[])
{
	map<string, user> m;
	//m["John"] = user("John Doe");  // error: no empty constructor for user

	auto [iter, inserted] = m.insert_or_assign("John", user{"John Doe"});
	if (inserted)
		cout << iter->first << " entry was inserted\n";
	else
		cout << iter->first << " entry was updated\n";

	return 0;
}
