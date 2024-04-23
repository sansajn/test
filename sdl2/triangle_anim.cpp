/* OpenGL ES 3.2, animated triangle and static axis object sample. The sample
program shows how to use VBO a VAO to render two objects (triangle and axis). */
#include <chrono>
#include <iostream>
#include <cassert>
#include <SDL.h>
#include <GLES3/gl32.h>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using std::cout, std::endl;
using std::chrono::steady_clock, std::chrono::duration_cast, std::chrono::milliseconds;
using glm::mat4, 
	glm::vec3, 
	glm::value_ptr,
	glm::perspective,
	glm::translate,
	glm::radians,
	glm::lookAt;

constexpr GLuint WIDTH = 800,
	HEIGHT = 600;

// TODO: is version correct for OpenGL ES 3.2 shader
GLchar const * vertex_shader_source = R"(
#version 100
attribute vec3 position;
uniform mat4 local_to_screen;
void main() {
	gl_Position = local_to_screen * vec4(position, 1.0);
})";

GLchar const * fragment_shader_source = R"(
#version 100
void main() {
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
})";


// just three lines
constexpr float axis_verts[] = {
	0,0,0, 1,0,0,  // x
	0,0,0, 0,1,0,  // y
	0,0,0, 0,0,1  // z
};

// triangle
constexpr float triangle_verts[] = {
	-0.3f, 0.0f, 0.0f,
	 0.3f, 0.0f, 0.0f,
	 0.0f, 0.6f, 0.0f,
};


GLint get_shader_program(char const * vertex_shader_source, char const * fragment_shader_source);


int main(int argc, char * argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("OpenGL ES 3.2", SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	SDL_GLContext context = SDL_GL_CreateContext(window);

	cout << "GL_VENDOR: " << glGetString(GL_VENDOR) << "\n" 
		<< "GL_VERSION: " << glGetString(GL_VERSION) << "\n"
		<< "GL_RENDERER: " << glGetString(GL_RENDERER) << "\n"
		<< "GLSL_VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	
	GLuint const shader_program = get_shader_program(vertex_shader_source, fragment_shader_source);
	
	GLint const position_loc = glGetAttribLocation(shader_program, "position");
	GLint const local_to_screen_loc = glGetUniformLocation(shader_program, "local_to_screen");
	
	vec3 const camera_position = vec3{5,5,5};
	
	mat4 P = perspective(radians(60.f), WIDTH/(float)HEIGHT, 0.01f, 1000.f);
	mat4 V = lookAt(camera_position, vec3{0,0,0}, vec3{0,1,0});
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, WIDTH, HEIGHT);

	// for triangle
	GLuint triangle_vao;
	glGenVertexArrays(1, &triangle_vao);

	glBindVertexArray(triangle_vao);
	GLuint triangle_vbo;
	glGenBuffers(1, &triangle_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_verts), triangle_verts, GL_STATIC_DRAW);
	glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(position_loc);

	// for axis
	GLuint axis_vao;
	glGenVertexArrays(1, &axis_vao);

	glBindVertexArray(axis_vao);
	GLuint axis_vbo;
	glGenBuffers(1, &axis_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, axis_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axis_verts), axis_verts, GL_STATIC_DRAW);
	glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(position_loc);

	glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind buffer
	glBindVertexArray(0);  // unbind vertex array

	glUseProgram(shader_program);

	auto t_prev = steady_clock::now();

	mat4 R_triangle{1};  // triangle rotation

	while (true) {
		auto t_now = steady_clock::now();
		
		double const dt = duration_cast<milliseconds>(t_now - t_prev).count() / 1000.0;
		t_prev = t_now;

		SDL_Event event;
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			break;

		// render
		glClear(GL_COLOR_BUFFER_BIT);  // clear buffer

		// draw triangle
		double const angle_speed = 2*3.1415 / 2.0;  // TODO: PI?
		vec3 const up{0.0, 1.0, 0.0};
		R_triangle = glm::rotate(R_triangle, float{dt*angle_speed}, up);
		mat4 local_to_screen_triangle = P*V*R_triangle;
		glUniformMatrix4fv(local_to_screen_loc, 1, GL_FALSE, value_ptr(local_to_screen_triangle));
		glBindVertexArray(triangle_vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		// draw axis
		mat4 M_axis{1};
		mat4 local_to_screen_axis = P*V*M_axis;
		glUniformMatrix4fv(local_to_screen_loc, 1, GL_FALSE, value_ptr(local_to_screen_axis));
		glBindVertexArray(axis_vao);
		glDrawArrays(GL_LINES, 0, 6);

		glBindVertexArray(0);  // unbind VAO

		SDL_GL_SwapWindow(window);
	}
	
	glDeleteBuffers(1, &axis_vbo);
	glDeleteBuffers(1, &triangle_vbo);
	glDeleteProgram(shader_program);

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}

GLint get_shader_program(char const * vertex_shader_source, char const * fragment_shader_source) {
	enum Consts {INFOLOG_LEN = 512};
	GLchar infoLog[INFOLOG_LEN];
	GLint fragment_shader;
	GLint shader_program;
	GLint success;
	GLint vertex_shader;

	/* Vertex shader */
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, INFOLOG_LEN, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" 
			<< infoLog << endl;
	}

	/* Fragment shader */
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, INFOLOG_LEN, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" 
			<< infoLog << endl;
	}

	/* Link shaders */
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, INFOLOG_LEN, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" 
			<< infoLog << endl;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	return shader_program;
}
