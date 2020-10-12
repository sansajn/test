// filesystem path sample
#include <filesystem>
#include <iostream>
using std::cout, std::endl;
using namespace std::filesystem;

int main(int argc, char * argv[])
{
	path p = current_path();
	
	cout << "current path: " << p << "\n"
		<< "root path: " << p.root_path() << "\n"
		<< "relative path: " << p.relative_path() << "\n"
		<< "parent path: " << p.parent_path() << "\n"
		<< "root name: " << p.root_name() << "\n"
		<< "root directory: " << p.root_directory()
		<< endl;

	cout << "\npath elements:\n";
	for (auto const & e : p)  // path iteration
		cout << e << ", ";
	cout << "\n";

	// replace "/home" with "/tmp"
	auto it = p.begin();
	++it;
	++it;
	
	path p2;
	for (; it != p.end(); ++it)  // path p2{it, p.end()};
		p2 /= *it;

	cout << "\n" << p2 << "\n"
		<< "/tmp" / p2 << "\n"; 

	cout << "\ndone!\n";
	return 0;
}
