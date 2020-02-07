#pragma once
#include <boost/noncopyable.hpp>
#include "Camera.h"

class DemoBase : boost::noncopyable
{
public:
	OrbitCamera camera;
	
	DemoBase();
	virtual ~DemoBase();
	virtual void Initialize(int width, int height);
	virtual void Resize(int width, int height);
	virtual void Render();
	virtual void Update(float dt);
	virtual void ImGUI();
	virtual void Shutdown();
	
	void SetMouseState(bool left, bool middle, bool right, vec2 const & delta, 
		vec2 const & mouse);

protected:
	bool show_help,
		mouseLeftDown,
		mouseRightDown,
		mouseMiddleDown;
	vec2 mouseDelta,
		mousePos,
		size;
	bool configHelpWindow;
};
