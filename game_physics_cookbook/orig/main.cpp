#include <chrono>
#include <thread>
#include <iostream>
#include <cassert>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include "IWindow.h"

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::cerr;
using std::cout;
using std::endl;

double GetMilliseconds()
{
	return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

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
	
	// imgui setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(w, true);
	ImGui_ImplOpenGL2_Init();
	
	IWindow * demo = IWindow::GetInstance();
	assert(demo);
	
	demo->OnInitialize();
	demo->OnResize(640, 480);
	demo->MarkAsShown();
	
	double lastTime = GetMilliseconds();
	double fixed_millis = demo->GetFixedFPS() / 1000.0;
	double fixed_ellapsed = 0.0;
	steady_clock::time_point next_game_tick = steady_clock::now();
	
	// loop
	while (!glfwWindowShouldClose(w))
	{
		glfwPollEvents();  // process events
		
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		// window update
		double time = GetMilliseconds();
		float deltaTime = float(time - lastTime) * 0.001f;  // in s
		lastTime = time;
		demo->OnUpdate(deltaTime);
		
		// phys update
		fixed_ellapsed += deltaTime;
		while (fixed_ellapsed > fixed_millis)
		{
			demo->OnFixedUpdate(fixed_millis);
			fixed_ellapsed -= fixed_millis;
		}
		
		// render
		glClear(GL_COLOR_BUFFER_BIT);  // render
		demo->OnRender();
		
		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
		
		glfwSwapBuffers(w);  // show
		
		// force FPS
		milliseconds delay{1000 / demo->GetTargetFPS()};
		next_game_tick += delay;
		if (steady_clock::now() < next_game_tick)
			std::this_thread::sleep_until(next_game_tick);
	}
	
	demo->Close();
	demo->OnShutdown();
	
	cout << "done!\n";
	return 0;
}
