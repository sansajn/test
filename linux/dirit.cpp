// implementacia a test directory_iterator
#include <iostream>
// posix
#include <dirent.h>

using std::cout;

char const * dir_to_list_path = "/tmp";

using directory_entry = dirent *;

/*! Implementacia directory iterator-a (podobneho s boost::filesystem).
Umoznuje ziskat obsah adresara. Trieda directory_iterator implementuje input iterator koncept. */
class directory_iterator 
{
public:
	directory_iterator() noexcept;
	directory_iterator(directory_iterator const & other) = delete;
	explicit directory_iterator(char const * dirpath);
	~directory_iterator();
	directory_iterator & operator=(directory_iterator const &) = delete;
	directory_iterator & operator++();  // pre-increment operator
	directory_entry operator*();
	bool operator==(directory_iterator const & rhs) const;
	bool operator!=(directory_iterator const & rhs) const {return !(*this == rhs);}
	
private:
	void next_entry();
	
	DIR * _dir;
	dirent * _entry;
};

directory_iterator const & begin(directory_iterator const & iter) {return iter;}
directory_iterator end(directory_iterator const &) {return directory_iterator{};}

directory_iterator::directory_iterator() noexcept
	: _dir{nullptr}, _entry{nullptr}
{}

directory_iterator::directory_iterator(char const * dirpath)
{
	_dir = opendir(dirpath);
	if (_dir)
		next_entry();
	else
		_entry = nullptr;
}

directory_iterator::~directory_iterator()
{
	closedir(_dir);
}

directory_iterator & directory_iterator::operator++()
{
	next_entry();
	return *this;
}

directory_entry directory_iterator::operator*()
{
	return _entry;
}

void directory_iterator::next_entry()
{
	_entry = readdir(_dir);
}

bool directory_iterator::operator==(directory_iterator const & rhs) const
{
	return _entry == rhs._entry;
}


int main(int argc, char * argv[])
{
	directory_iterator it{dir_to_list_path}, end_it;
	for (; it != end_it; ++it)
		cout << (*it)->d_name << "\n";
	return 0;
}
