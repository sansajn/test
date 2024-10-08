#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_glfw.h"
#include "imgui/examples/imgui_impl_opengl2.h"

int main(void)
{
	// Initialize the library
	if (!glfwInit())
		return -1;

	char window_title[255] = {"Hello World"};

	// Create a windowed mode window and its OpenGL context
	GLFWwindow * window = glfwCreateWindow(640, 480, window_title, nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO & io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL2_Init();


	float bgcolor[3] = {0,0,0};

	glClearColor(bgcolor[0], bgcolor[1], bgcolor[2], 1);

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Sample window"); // begin window

		// Window title text edit
		if (ImGui::InputText("Window title", window_title, 255))
		{
			glfwSetWindowTitle(window, window_title);
		}

		if (ImGui::ColorEdit3("Background color", bgcolor))
		{
			// this code gets called if color value changes, so
			// the background color is upgraded automatically!
			glClearColor(bgcolor[0], bgcolor[1], bgcolor[2], 1);
		}

		ImGui::End(); // end window

		ImGui::Render();

		// Render here
		glClear(GL_COLOR_BUFFER_BIT);
		// ...

		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}
