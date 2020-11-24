// extract (for set, map) function sample

#include <algorithm>
#include <set>
#include <string>
#include <iostream>

using std::move;
using std::set;
using std::string;
using std::cout;

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
	set<user> names;
	names.emplace("John");
	names.emplace("Alex");

	set<user> out;
	// move John to the out set
	auto handle = names.extract(user{"John"});
	out.insert(move(handle));  // insert John without node allocation

	for (auto & e : names)
		cout << e.name << "\n";

	cout << "done!\n";

	return 0;
}
