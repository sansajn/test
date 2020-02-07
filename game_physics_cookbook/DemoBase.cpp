#include "DemoBase.h"
#include "imgui/imgui.h"
#include "FixedFunctionPrimitives.h"

DemoBase::DemoBase()
{
	show_help = false;
	mouseLeftDown = false;
	mouseRightDown = false;
	mouseMiddleDown = false;
	mouseDelta = vec2{0.0f, 0.0f};
	mousePos = vec2{0.0f, 0.0f};
	size = vec2{0.0f, 0.0f};
	configHelpWindow = true;
}

void DemoBase::Initialize(int width, int height)
{
	Resize(width, height);
}

void DemoBase::Resize(int width, int height)
{
	camera.Perspective(60.0f, width / (float)height, 0.01f, 1000.0f);
	size = vec2(width, height);
}

void DemoBase::Update(float dt)
{
	if (mouseRightDown)
		camera.Rotate(mouseDelta, dt);
	else if (mouseMiddleDown)
		camera.Zoom(mouseDelta.y, dt);
	else if (mouseLeftDown)
		camera.Pan(mouseDelta, dt);
	
	camera.Update(dt);  // update the camera position
}

void DemoBase::Render()
{
	FixedFunctionOrigin();
}

void DemoBase::SetMouseState(bool left, bool middle, bool right, vec2 const & delta, vec2 const & mouse)
{
	mouseLeftDown = left;
	mouseMiddleDown = middle;
	mouseRightDown = right;
	mouseDelta = delta;
	mousePos = mouse;
}
