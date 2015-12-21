// vylistuje adresar
#include <iostream>
#include <cassert>
// posix api
#include <sys/types.h>
#include <dirent.h>

using std::cout;

char const * dir_to_list_path = "/tmp";

int main(int argc, char * argv[])
{
	DIR * dir = opendir(dir_to_list_path);
	assert(dir && "unable to open a directory");

	dirent entry;
	dirent * result;
	
	for (;;)
	{
		int error = readdir_r(dir, &entry, &result);
		assert(error == 0 && "directory reading error");
		if (!result)
			break;  // end of directory
		cout << entry.d_name << "\n";		
	}
	
	closedir(dir);
	return 0;
}

 
