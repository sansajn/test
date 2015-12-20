// vylistuje adresar

#include <iostream>
#include <cassert>
// posix api
#include <sys/types.h>
#include <dirent.h>

using std::cout;

/* Práca s adresármi v posix kompatibilnom systéme
Funkcie api sú definované v hlavičkovom súbore 'dirent.h':

	opendir()
	readdir()
	seekdir()
	telldir()
	closedir()
	dirfd()
	scandir() 
	
*/

char const * dir_to_list_path = "/tmp";

int main(int argc, char * argv[])
{
	DIR * dir = opendir(dir_to_list_path);
	assert(dir && "unable to open a directory");

	dirent * dir_entry;
  	while ((dir_entry = readdir(dir)) != NULL)
	{
		cout << dir_entry->d_name << "\n";
	}

	closedir(dir);	
	return 0;
}


