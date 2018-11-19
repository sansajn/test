// behaviour based window implementation with user input
#include <stdexcept>
#include <string>
#include <cassert>
#include <iostream>
#include <glm/vec2.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>

using std::string;
using std::cout;
using glm::ivec2;

void glew_init();

class event_handler
{
public:
	virtual void display() {}
	virtual void close() {}
	virtual void mouse_passive_motion(int x, int y) {}
	virtual void mouse_click(int button, int state, int modifier, int x, int y) {}
	virtual void key_typed(unsigned char c, int modifier, int x, int y) {}
	virtual void key_released(unsigned char c, int modifier, int x, int y) {}
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

	struct user_input
	{
		user_input();

		bool key(unsigned char c) const;
		bool key_up(unsigned char c) const;
		bool any_of_key(char const * s) const;
		bool any_of_key_up(char const * s) const;
		bool mouse(int button) const;
		bool mouse_up(int button) const;
		ivec2 const & mouse_position() const;
		// TODO: special support

		void update();  //!< for internal use only

	//private:

		static unsigned const NUM_KEYS = 256;
		static unsigned const NUM_BUTTONS = 6;

		bool _keys[NUM_KEYS];
		bool _keys_up[NUM_KEYS];
		bool _mouse_buttons[NUM_BUTTONS];
		bool _mouse_buttons_up[NUM_BUTTONS];
		ivec2 _mouse_pos;
	};

	glut_layer(parameters const & params);
	void display() override {swap_buffers();}
	void install_display_handler() override;
	void main_loop() override {glutMainLoop();}
	void main_loop_event() override {glutMainLoopEvent();}
	void swap_buffers() override {glutSwapBuffers();}
};


bool glut_layer::user_input::key(unsigned char c) const
{
	return _keys[c];
}

bool glut_layer::user_input::key_up(unsigned char c) const
{
	return _keys_up[c];
}

bool glut_layer::user_input::any_of_key(char const * s) const
{
	while (*s)
		if (key(*s++))
			return true;
	return false;
}

bool glut_layer::user_input::any_of_key_up(char const * s) const
{
	while (*s)
		if (key_up(*s++))
			return true;
	return false;
}

bool glut_layer::user_input::mouse(int button) const
{
	return _mouse_buttons[button];
}

bool glut_layer::user_input::mouse_up(int button) const
{
	return _mouse_buttons_up[button];
}

ivec2 const & glut_layer::user_input::mouse_position() const
{
	return _mouse_pos;
}

void glut_layer::user_input::update()
{
	for (bool & k : _keys_up)  // set all keys up to false
		k = false;

	for (bool & b : _mouse_buttons_up)  // set all buttons up to false
		b = false;
}

glut_layer::user_input::user_input()
{
	for (unsigned i = 0; i < NUM_KEYS; ++i)
		_keys[i] = _keys_up[i] = false;

	_mouse_pos = glm::vec2(0,0);  // TODO: zvaz hodnotu center
	for (unsigned i = 0; i < NUM_BUTTONS; ++i)
		_mouse_buttons[i] = _mouse_buttons_up[i] = false;
}


template <typename L>
class pool_behaviour : public L
{
public:
	using parameters = typename L::parameters;
	using user_input = typename L::user_input;

	pool_behaviour(parameters const & params) : L{params} {}
	void start();
	virtual void update(float dt) {}
	virtual void input(float dt) {}

	// layer events
	void close() override {_closed = true;}

	user_input in;

private:	
	void mouse_passive_motion(int x, int y) override {in._mouse_pos = ivec2{x, y};}
	void mouse_click(int button, int state, int modifier, int x, int y) override;
	void key_typed(unsigned char c, int modifier, int x, int y) override;
	void key_released(unsigned char c, int modifier, int x, int y) override;

	bool _closed = false;
};

template <typename L>
void pool_behaviour<L>::mouse_click(int button, int state, int modifier, int x, int y)
{
	if (state == GLUT_DOWN)
		in._mouse_buttons[button] = true;
	else  // up
	{
		in._mouse_buttons[button] = false;
		in._mouse_buttons_up[button] = true;
	}
	in._mouse_pos = ivec2{x, y};
}

template <typename L>
void pool_behaviour<L>::key_typed(unsigned char c, int modifier, int x, int y)
{
	in._keys[c] = true;
}

template <typename L>
void pool_behaviour<L>::key_released(unsigned char c, int modifier, int x, int y)
{
	in._keys[c] = false;
	in._keys_up[c] = true;
}

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

		in.update();
	}
}


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
void mouse_func(int mouse_btn, int btn_state, int x, int y);
void passive_motion_func(int x, int y);
void keyboard_func(unsigned char c, int x, int y);
void keyboard_up_func(unsigned char c, int x, int y);

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
	glutMouseFunc(glut_detail::mouse_func);
	glutPassiveMotionFunc(glut_detail::passive_motion_func);
	glutKeyboardFunc(glut_detail::keyboard_func);
	glutKeyboardUpFunc(glut_detail::keyboard_up_func);

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

	void display() override
	{
		glClearColor(1,0,1,1);
		glClear(GL_COLOR_BUFFER_BIT);
		base::display();
	}

	void input(float dt) override
	{
		if (in.key_up('a'))
			cout << "'a' key released" << std::endl;
		if (in.mouse_up(GLUT_LEFT_BUTTON))
			cout << "left button released" << std::endl;
		base::input(dt);
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

void mouse_func(int mouse_btn, int btn_state, int x, int y)
{
	assert(__window);
	__window->mouse_click(mouse_btn, btn_state, glutGetModifiers(), x, y);
}

void passive_motion_func(int x, int y)
{
	assert(__window);
	__window->mouse_passive_motion(x, y);
}

void keyboard_func(unsigned char c, int x, int y)
{
	assert(__window);
	__window->key_typed(c, glutGetModifiers(), x, y);
}

void keyboard_up_func(unsigned char c, int x, int y)
{
	assert(__window);
	__window->key_released(c, glutGetModifiers(), x, y);
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
