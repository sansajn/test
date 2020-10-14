// filesystem path sample
#include <filesystem>
#include <iostream>
using std::cout, std::endl;
using namespace std::filesystem;

int main(int argc, char * argv[])
{
	path p;
	if (argc > 1)
		p = argv[1];
	else
		p = current_path();
	
	cout << "current path: " << p << "\n"
		<< "root_name: " << p.root_name() << "\n"
		<< "root_directory: " << p.root_directory() << "\n"
		<< "root_path: " << p.root_path() << "\n"
		<< "relative_path: " << p.relative_path() << "\n"
		<< "parent_path: " << p.parent_path() << "\n"
		<< "filename: " << p.filename() << "\n"
		<< "stem: " << p.stem() << "\n"
		<< "extension: " << p.extension()
		<< endl;

	cout << "\npath elements:\n";
	for (auto const & e : p)  // path iteration
		cout << e << ", ";
	cout << "\n";

	// replace first two elements with "/tmp"
	cout << "replace /home with /tmp\n";
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
