#include <iostream>
#include <X11/Xlib.h>

using std::cout; 

int main(int argc, char * argv[]) 
{
	cout << "Window:" << sizeof(Window) << "\n";
	return 0;
}
