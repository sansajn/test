// behaviour based window implementation
#include <stdexcept>
#include <string>
#include <cassert>
#include <GL/glew.h>
#include <GL/freeglut.h>

using std::string;

void glew_init();

class event_handler
{
public:
	virtual void display() {}
	virtual void close() {}
};

class window_layer : public event_handler
{
public:
	class parameters
	{
	public:
		parameters();
		parameters & geometry(unsigned w, unsigned h) {_w = w; _h = h; return *this;}
		parameters & name(string const & s) {_name = s; return *this;}
		parameters & debug(bool d) {_debug = d; return *this;}
		unsigned width() const {return _w;}
		unsigned height() const {return _h;}
		string const & name() const {return _name;}
		bool debug() const {return _debug;}

	private:
		unsigned _w, _h;
		string _name;
		bool _debug;
	};

	virtual ~window_layer() {}
	virtual void install_display_handler() {}
	virtual void main_loop() {}
	virtual void main_loop_event() {}
	virtual void swap_buffers() {}
};

class glut_layer : public window_layer
{
public:
	using parameters = window_layer::parameters;

	glut_layer(parameters const & params);
	void display() override {swap_buffers();}
	void install_display_handler() override;
	void main_loop() override {glutMainLoop();}
	void main_loop_event() override {glutMainLoopEvent();}
	void swap_buffers() override {glutSwapBuffers();}
};

template <typename L>  // L : window-layer implementation \sa glut_layer
class event_behaviour : public L
{
public:
	using parameters = typename L::parameters;

	event_behaviour(parameters const & params);
	void start() {L::main_loop();}
};

template <typename L>
event_behaviour<L>::event_behaviour(parameters const & params)
	: L{params}
{
	L::install_display_handler();
}

template <typename L>
class pool_behaviour : public L
{
public:
	using parameters = typename L::parameters;

	pool_behaviour(parameters const & params) : L{params} {}
	void start();
	void update(float dt);
	void input(float dt);

	// layer events
	void close() override {_closed = true;}

private:
	bool _closed = false;
};

template <typename L>
void pool_behaviour<L>::start()
{
	while (true)
	{
		float dt = 0.1f;

		L::main_loop_event();
		if (_closed)
			break;

		input(dt);
		update(dt);
		this->display();
	}
}

template <typename L>
void pool_behaviour<L>::update(float dt)
{}

template <typename L>
void pool_behaviour<L>::input(float dt)
{}


template <template<class> class B, typename L>
class window : public B<L>
{
public:
	using parameters = typename L::parameters;

	window(parameters const & params = parameters{});
	unsigned width() const {return _w;}
	unsigned height() const {return _h;}
	float aspect_ratio() const {return (float)_w/(float)_h;}

private:
	unsigned _w, _h;
};

template <template<class> class B, typename L>
window<B, L>::window(parameters const & params)
	: B<L>{params}
{
	_w = params.width();
	_h = params.height();
}


namespace glut_detail {

glut_layer * __window = nullptr;

void display_func();
void close_func();

}  // glut_detail

glut_layer::glut_layer(parameters const & params)
{
	if (!glut_detail::__window)
	{
		int argc = 1;
		char const * argv = "dummy";
		glutInit(&argc, (char **)&argv);
	}

	if (params.debug())
		glutInitContextFlags(GLUT_DEBUG);

	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitWindowSize(params.width(), params.height());

	glutCreateWindow(params.name().c_str());
	glut_detail::__window = this;

	glutCloseFunc(glut_detail::close_func);

	glew_init();
}

void glut_layer::install_display_handler()
{
	glutDisplayFunc(glut_detail::display_func);
}



window_layer::parameters::parameters()
{
	_w = 800;
	_h = 600;
	_name = "window";
	_debug = true;
}


class event_scene : public window<event_behaviour, glut_layer>
{
public:
	using base = window<event_behaviour, glut_layer>;

	void display() override {
		glClearColor(0,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT);
		base::display();
	}
};


class pool_scene : public window<pool_behaviour, glut_layer>
{
public:
	using base = window<pool_behaviour, glut_layer>;

	void display() override {
		glClearColor(1,0,1,1);
		glClear(GL_COLOR_BUFFER_BIT);
		base::display();
	}
};


int main(int argc, char * argv[])
{
	pool_scene w;
	w.start();
	return 0;
}


namespace glut_detail {

void display_func()
{
	assert(__window);
	__window->display();
}

void close_func()
{
	assert(__window);
	__window->close();
}

}  // glut_detail


void glew_init()
{
	assert(glGetError() == 0);
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
		throw std::runtime_error("GLEW initialization failed");
	glGetError();  // eat error
}
