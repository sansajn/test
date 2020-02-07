// implemented using GLFW3 v3.3
#include <GLFW/glfw3.h>
#include "FixedFunctionPrimitives.h"

void FixedFunctionOrigin()
{
	FixedFunctionOrigin(false, false);
}

void FixedFunctionOrigin(bool depthTest, bool twoSided)
{
	bool isLit = glIsEnabled(GL_LIGHTING) == GL_TRUE;
	bool depthOn = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE;
	
	if (isLit)
		glDisable(GL_LIGHTING);
	
	if (depthOn && !depthTest)
		glEnable(GL_DEPTH_TEST);
	
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	if (twoSided)
	{
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, 0.0f, 0.0f);
	}
	
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	if (twoSided)
	{
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, -1.0f, 0.0f);
	}
	
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	if (twoSided)
	{
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, -1.0f);
	}	
	glEnd();
	
	if (isLit)
		glEnable(GL_LIGHTING);
	
	if (depthOn)
		glEnable(GL_DEPTH_TEST);
}

