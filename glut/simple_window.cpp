#include <stdexcept>
#include <cassert>
#include <GL/glew.h>
#include <GL/freeglut.h>

void glew_init();
void display_func();

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);

	glutInitContextFlags(GLUT_DEBUG);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitWindowSize(800, 600);
	glutCreateWindow("window");

	glutDisplayFunc(display_func);

	glew_init();

	glutMainLoop();

	return 0;
}

void display_func()
{
	glClearColor(1,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}


void glew_init()
{
	assert(glGetError() == 0);
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
		throw std::runtime_error("GLEW initialization failed");
	glGetError();  // eat error
}
