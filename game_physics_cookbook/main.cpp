#include <iostream>
#include <cassert>
#include <GLFW/glfw3.h>
#include "IWindow.h"
using std::cerr;
using std::cout;
using std::endl;

int main(int argc, char * argv[])
{
	if (!glfwInit())
	{
		cerr << "fatal, glfw initialization failed" << endl;
		return 1;
	}
	
	GLFWwindow * w = glfwCreateWindow(640, 480, "glfw window", nullptr, nullptr);
	assert(w);
	
	glfwMakeContextCurrent(w);
	
	// loop
	while (!glfwWindowShouldClose(w))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(w);
		glfwPollEvents();  // process events
	}	
	
	cout << "done!\n";
	return 0;
}
