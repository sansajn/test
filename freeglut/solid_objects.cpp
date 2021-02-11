// OpenGL 1.0 based solid objects sample from NeHe (lesson 5)

#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <unistd.h>     // needed to sleep

// ASCII code for the escape key.
#define ESCAPE 27

int window;  // The number of our GLUT window
float rtri = 0.0f;  // Rotation angle for the triangle.
float rquad = 0.0f;  // Rotation angle for the quadrilateral.


/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void init_gl(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
	glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);			        // The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);		        // Enables Depth Testing
	glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();				// Reset The Projection Matrix

	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

	glMatrixMode(GL_MODELVIEW);
}

// The function called when our window is resized (which shouldn't happen, because we're fullscreen)
void resize_scene(int Width, int Height)
{
	if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
		Height=1;

	glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
}

// The main drawing function.
void draw_scene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View

	glTranslatef(-1.5f,0.0f,-6.0f);		// Move Left 1.5 Units And Into The Screen 6.0
	
	glRotatef(rtri,0.0f,1.0f,0.0f);		// Rotate The Pyramid On The Y axis

	// draw a pyramid (in smooth coloring mode)
	glBegin(GL_POLYGON);				// start drawing a pyramid

	// front face of pyramid
	glColor3f(1.0f,0.0f,0.0f);			// Set The Color To Red
	glVertex3f(0.0f, 1.0f, 0.0f);		        // Top of triangle (front)
	glColor3f(0.0f,1.0f,0.0f);			// Set The Color To Green
	glVertex3f(-1.0f,-1.0f, 1.0f);		// left of triangle (front)
	glColor3f(0.0f,0.0f,1.0f);			// Set The Color To Blue
	glVertex3f(1.0f,-1.0f, 1.0f);		        // right of traingle (front)

	// right face of pyramid
	glColor3f(1.0f,0.0f,0.0f);			// Red
	glVertex3f( 0.0f, 1.0f, 0.0f);		// Top Of Triangle (Right)
	glColor3f(0.0f,0.0f,1.0f);			// Blue
	glVertex3f( 1.0f,-1.0f, 1.0f);		// Left Of Triangle (Right)
	glColor3f(0.0f,1.0f,0.0f);			// Green
	glVertex3f( 1.0f,-1.0f, -1.0f);		// Right Of Triangle (Right)

	// back face of pyramid
	glColor3f(1.0f,0.0f,0.0f);			// Red
	glVertex3f( 0.0f, 1.0f, 0.0f);		// Top Of Triangle (Back)
	glColor3f(0.0f,1.0f,0.0f);			// Green
	glVertex3f( 1.0f,-1.0f, -1.0f);		// Left Of Triangle (Back)
	glColor3f(0.0f,0.0f,1.0f);			// Blue
	glVertex3f(-1.0f,-1.0f, -1.0f);		// Right Of Triangle (Back)

	// left face of pyramid.
	glColor3f(1.0f,0.0f,0.0f);			// Red
	glVertex3f( 0.0f, 1.0f, 0.0f);		// Top Of Triangle (Left)
	glColor3f(0.0f,0.0f,1.0f);			// Blue
	glVertex3f(-1.0f,-1.0f,-1.0f);		// Left Of Triangle (Left)
	glColor3f(0.0f,1.0f,0.0f);			// Green
	glVertex3f(-1.0f,-1.0f, 1.0f);		// Right Of Triangle (Left)

	glEnd();					// Done Drawing The Pyramid

	glLoadIdentity();				// make sure we're no longer rotated.
	glTranslatef(1.5f,0.0f,-7.0f);		// Move Right 3 Units, and back into the screen 7
	
	glRotatef(rquad,1.0f,1.0f,1.0f);		// Rotate The Cube On X, Y, and Z

	// draw a cube (6 quadrilaterals)
	glBegin(GL_QUADS);				// start drawing the cube.

	// top of cube
	glColor3f(0.0f,1.0f,0.0f);			// Set The Color To Blue
	glVertex3f( 1.0f, 1.0f,-1.0f);		// Top Right Of The Quad (Top)
	glVertex3f(-1.0f, 1.0f,-1.0f);		// Top Left Of The Quad (Top)
	glVertex3f(-1.0f, 1.0f, 1.0f);		// Bottom Left Of The Quad (Top)
	glVertex3f( 1.0f, 1.0f, 1.0f);		// Bottom Right Of The Quad (Top)

	// bottom of cube
	glColor3f(1.0f,0.5f,0.0f);			// Set The Color To Orange
	glVertex3f( 1.0f,-1.0f, 1.0f);		// Top Right Of The Quad (Bottom)
	glVertex3f(-1.0f,-1.0f, 1.0f);		// Top Left Of The Quad (Bottom)
	glVertex3f(-1.0f,-1.0f,-1.0f);		// Bottom Left Of The Quad (Bottom)
	glVertex3f( 1.0f,-1.0f,-1.0f);		// Bottom Right Of The Quad (Bottom)

	// front of cube
	glColor3f(1.0f,0.0f,0.0f);			// Set The Color To Red
	glVertex3f( 1.0f, 1.0f, 1.0f);		// Top Right Of The Quad (Front)
	glVertex3f(-1.0f, 1.0f, 1.0f);		// Top Left Of The Quad (Front)
	glVertex3f(-1.0f,-1.0f, 1.0f);		// Bottom Left Of The Quad (Front)
	glVertex3f( 1.0f,-1.0f, 1.0f);		// Bottom Right Of The Quad (Front)

	// back of cube.
	glColor3f(1.0f,1.0f,0.0f);			// Set The Color To Yellow
	glVertex3f( 1.0f,-1.0f,-1.0f);		// Top Right Of The Quad (Back)
	glVertex3f(-1.0f,-1.0f,-1.0f);		// Top Left Of The Quad (Back)
	glVertex3f(-1.0f, 1.0f,-1.0f);		// Bottom Left Of The Quad (Back)
	glVertex3f( 1.0f, 1.0f,-1.0f);		// Bottom Right Of The Quad (Back)

	// left of cube
	glColor3f(0.0f,0.0f,1.0f);			// Blue
	glVertex3f(-1.0f, 1.0f, 1.0f);		// Top Right Of The Quad (Left)
	glVertex3f(-1.0f, 1.0f,-1.0f);		// Top Left Of The Quad (Left)
	glVertex3f(-1.0f,-1.0f,-1.0f);		// Bottom Left Of The Quad (Left)
	glVertex3f(-1.0f,-1.0f, 1.0f);		// Bottom Right Of The Quad (Left)

	// Right of cube
	glColor3f(1.0f,0.0f,1.0f);			// Set The Color To Violet
	glVertex3f( 1.0f, 1.0f,-1.0f);	        // Top Right Of The Quad (Right)
	glVertex3f( 1.0f, 1.0f, 1.0f);		// Top Left Of The Quad (Right)
	glVertex3f( 1.0f,-1.0f, 1.0f);		// Bottom Left Of The Quad (Right)
	glVertex3f( 1.0f,-1.0f,-1.0f);		// Bottom Right Of The Quad (Right)
	glEnd();					// Done Drawing The Cube

	int now = glutGet(GLUT_ELAPSED_TIME);
	static int before = now;
	float dt = (before - now) / 1000.0f;
	before = now;

	rtri += 15.0f * dt;					// Increase The Rotation Variable For The Pyramid
	rquad -= 15.0f * dt;					// Decrease The Rotation Variable For The Cube

	// swap the buffers to display, since double buffering is used.
	glutSwapBuffers();
}

// The function called whenever a key is pressed.
void key_pressed(unsigned char key, int x, int y)
{
	/* avoid thrashing this call */
	usleep(100);

	/* If escape is pressed, kill everything. */
	if (key == ESCAPE)
	{
		/* shut down our window */
		glutDestroyWindow(window);

		/* exit the program...normal termination. */
		exit(0);
	}
}

int main(int argc, char **argv) 
{  
	/* Initialize GLUT state - glut will take any command line arguments that pertain to it or
	  X Windows - look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html */
	glutInit(&argc, argv);

	/* Select type of Display mode:
	  Double buffer
	  RGBA color
	  Alpha components supported
	  Depth buffered for automatic clipping */
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

	/* get a 1024 x 768 window */
	glutInitWindowSize(1024, 768);

	/* the window starts at the upper left corner of the screen */
	glutInitWindowPosition(0, 0);

	/* Open a window */
	window = glutCreateWindow("Jeff Molofee's GL Code Tutorial ... NeHe '99");

	/* Register the function to do all our OpenGL drawing. */
	glutDisplayFunc(&draw_scene);

	/* Go fullscreen.  This is as soon as possible. */
	//glutFullScreen();

	/* Even if there are no events, redraw our gl scene. */
	glutIdleFunc(&draw_scene);

	/* Register the function called when our window is resized. */
	glutReshapeFunc(&resize_scene);

	/* Register the function called when the keyboard is pressed. */
	glutKeyboardFunc(&key_pressed);

	/* Initialize our window. */
	init_gl(1024, 768);

	/* Start Event Processing Engine */
	glutMainLoop();

	return 1;
}




