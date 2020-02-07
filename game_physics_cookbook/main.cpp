#include <iostream>
#include <cassert>
#include <GLFW/glfw3.h>
#include "IWindow.h"
using std::cerr;
using std::cout;
using std::endl;

void window_size(GLFWwindow * win, int width, int height)
{
	IWindow * demo = IWindow::GetInstance();
	demo->Resize(width, height);
	if (demo->WasWindowShown())
		demo->OnResize(width, height);
}

int main(int argc, char * argv[])
{
	if (!glfwInit())
	{
		cerr << "fatal, glfw initialization failed" << endl;
		return 1;
	}
	
	GLFWwindow * w = glfwCreateWindow(640, 480, "glfw window", nullptr, nullptr);
	assert(w);
	
	glfwSetWindowSizeCallback(w, window_size);
	
	glfwMakeContextCurrent(w);
	
	IWindow * demo = IWindow::GetInstance();
	
	// loop
	while (!glfwWindowShouldClose(w))
	{
		glClear(GL_COLOR_BUFFER_BIT);  // render
		glfwSwapBuffers(w);  // show
		glfwPollEvents();  // process events
	}
	
	demo->Close();
	
	cout << "done!\n";
	return 0;
}
