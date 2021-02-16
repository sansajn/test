//! OpenGL visualized fall sample
#include <algorithm>
#include <memory>
#include <utility>
#include <chrono>
#include <set>
#include <iterator>
#include <iostream>
#include <boost/range/iterator_range.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/btBulletCollisionCommon.h>
#include <GL/freeglut.h>
#include "DebugDrawer.h"

using std::find;
using std::unique_ptr, std::make_unique;
using std::move, std::make_pair;
using std::chrono::milliseconds, std::chrono::duration, std::chrono::duration_cast,
	std::chrono::steady_clock;
using namespace std::chrono_literals;
using std::size;
using std::cout, std::endl, std::ostream;
using boost::iterator_range;


btTransform translate(btVector3 const & v);

namespace physics {

class body
{
public:
	/*! represents body shape
	To create box shape, type
	\code
	shape_type box = make_unique<btBoxShape>(btVector3{50, 50, 50});
	\endcode */
	using shape_type = unique_ptr<btCollisionShape>;

	body(shape_type && shape, btTransform && T, btScalar mass = 0);
	btVector3 const & position() const;

	// native geters
	btRigidBody & rigid_body() {return _body;}

	template <typename T>
	bool is_same(T * p) const {return (void *)p == &_body;}

private:
	shape_type _shape;
	btDefaultMotionState _motion;
	btRigidBody _body;
};


struct collision_listener
{
	virtual void on_collision(btCollisionObject * a, btCollisionObject * b) {}
	virtual void on_separation(btCollisionObject * a, btCollisionObject * b) {}
};

class world
{
public:
	using collision_range = iterator_range<btCollisionObject * const *>;

	world();
	void add_body(body * b);
	void simulate(btScalar time_step, int sub_steps = 10);

	collision_range collision_objects();

	void subscribe_collisions(collision_listener * l);
	void unsubscribe_collisions(collision_listener * l);

	btDiscreteDynamicsWorld & native() {return _world;}

private:
	using collision_pairs = std::set<std::pair<btCollisionObject const *,
		btCollisionObject const *>>;

	void handle_collisions();
	void collision_event(btCollisionObject * a, btCollisionObject * b);
	void separation_event(btCollisionObject * a, btCollisionObject * b);

	btDefaultCollisionConfiguration _config;
	btCollisionDispatcher _dispatcher;
	btDbvtBroadphase _pair_cache;
	btSequentialImpulseConstraintSolver _solver;
	btDiscreteDynamicsWorld _world;

	collision_pairs _last_collisions;
	std::vector<collision_listener *> _collision_listeners;

	std::unique_ptr<DebugDrawer> _ddraw;
};

btVector3 calculate_local_inertia(btCollisionShape & shape, btScalar mass)
{
	btVector3 local_inertia = {0, 0, 0};
	if (mass > 0)
		shape.calculateLocalInertia(mass, local_inertia);
	return local_inertia;
}

body::body(shape_type && shape, btTransform && T, btScalar mass)
	: _shape{move(shape)}
	, _motion{move(T)}
	, _body{mass, &_motion, _shape.get(), calculate_local_inertia(*_shape, mass)}
{
	assert(_shape && "shape required");
}

btVector3 const & body::position() const
{
	return _body.getWorldTransform().getOrigin();
}

world::world()
	: _dispatcher{&_config}
	, _world{&_dispatcher, &_pair_cache, &_solver, &_config}
{
	_ddraw.reset(new DebugDrawer);
	_ddraw->ToggleDebugFlag(btIDebugDraw::DBG_DrawWireframe);
	_ddraw->ToggleDebugFlag(btIDebugDraw::DBG_DrawAabb);
	_world.setDebugDrawer(_ddraw.get());
}

void world::add_body(body * b)
{
	_world.addRigidBody(&b->rigid_body());
}

void world::simulate(btScalar time_step, int sub_steps)
{
	_world.stepSimulation(time_step, sub_steps);
	handle_collisions();
}

world::collision_range world::collision_objects()
{
	btCollisionObjectArray const & colls = _world.getCollisionObjectArray();
	return collision_range{&colls[0], &colls[size(colls)]};
}

void world::subscribe_collisions(collision_listener * l)
{
	_collision_listeners.push_back(l);
}

void world::unsubscribe_collisions(collision_listener * l)
{
	auto it = find(begin(_collision_listeners), end(_collision_listeners), l);
	if (it != end(_collision_listeners))
		_collision_listeners.erase(it);
}

void world::handle_collisions()
{
	// collisions this update
	collision_pairs pairs_this_update;
	for (int i = 0; i < _dispatcher.getNumManifolds(); ++i)
	{
		btPersistentManifold * manifold = _dispatcher.getManifoldByIndexInternal(i);
		if (manifold->getNumContacts() > 0)
		{
			auto body0 = manifold->getBody0();
			auto body1 = manifold->getBody1();

			// always create the pair in a predictable order
			bool const swapped = body0 > body1;
			auto sorted_body_a = swapped ? body1 : body0;
			auto sorted_body_b = swapped ? body0 : body1;
			auto collision = make_pair(sorted_body_a, sorted_body_b);
			pairs_this_update.insert(collision);

			if (_last_collisions.find(collision) == _last_collisions.end())
				collision_event((btCollisionObject *)body0, (btCollisionObject *)body1);
		}
	}

	// collisions removed this update
	collision_pairs removed_pairs;
	set_difference(_last_collisions.begin(), _last_collisions.end(),
		pairs_this_update.begin(), pairs_this_update.end(), inserter(removed_pairs, removed_pairs.begin()));

	for (auto const & collision : removed_pairs)
		separation_event((btCollisionObject *)collision.first, (btCollisionObject *)collision.second);

	std::swap(_last_collisions, pairs_this_update);
}


void world::collision_event(btCollisionObject * a, btCollisionObject * b)
{
	for (auto * l : _collision_listeners)
		l->on_collision(a, b);
}

void world::separation_event(btCollisionObject * a, btCollisionObject * b)
{
	for (auto * l : _collision_listeners)
		l->on_separation(a, b);
}

}  // physics


ostream & operator<<(ostream & o, btVector3 const & v)
{
	o << "(" << v.getX() << ", " << v.getY() << ", " << v.getZ() << ")";
	return o;
}

struct collision_handler : public physics::collision_listener
{
	collision_handler(physics::body * ground, physics::body * sphere)
		: _ground{ground}, _sphere{sphere}, _done{false}
	{}

	void on_collision(btCollisionObject * a, btCollisionObject * b) override
	{
		if ((_ground->is_same(a) || _sphere->is_same(a))
			&& (_ground->is_same(b) || _sphere->is_same(b)))
		{
			_done = true;
			object_a = a;
			object_b = b;
		}
	}

	bool finished() const {return _done;}

	btCollisionObject * object_a,
		* object_b;

private:
	physics::body * _ground,
		* _sphere;
	bool _done;
};

namespace ui {

class glut_app
{
public:
	glut_app(int argc, char * argv[]);
	void go();  // blocking
	int width() const {return _w;}
	int height() const {return _h;}

protected:
	static glut_app & ref();

	virtual void initialize();
	virtual void update(steady_clock::duration const & dt) {}
	virtual void render() {}
	virtual void idle();
	virtual void reshape(int w, int h);
	virtual void keyboard_event(unsigned char key, int x, int y) {}
	virtual void keyboard_up_event(unsigned char key, int x, int y) {}
	virtual void special_event(int key, int x, int y) {}
	virtual void special_up_event(int key, int x, int y) {}
	virtual void mouse_event(int button, int state, int x, int y) {}
	virtual void motion_event(int x, int y) {}

private:
	static void keyboard_cb(unsigned char key, int x, int y);
	static void keyboard_up_cb(unsigned char key, int x, int y);
	static void special_cb(int key, int x, int y);
	static void special_up_cb(int key, int x, int y);
	static void reshape_cb(int w, int h);
	static void idle_cb();
	static void mouse_cb(int button, int state, int x, int y);
	static void motion_cb(int x, int y);
	static void display_cb();

	int _w, _h;
	static glut_app * _app;
};

glut_app * glut_app::_app = nullptr;

void glut_app::idle()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	steady_clock::time_point now = steady_clock::now();
	static steady_clock::time_point t0 = now;
	steady_clock::duration dt = now - t0;
	t0 = now;

	update(dt);

	render();

	glutSwapBuffers();
}

void glut_app::initialize()
{
	// create some floats for our ambient, diffuse, specular and position
	GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f }; // dark grey
	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // white
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // white
	GLfloat position[] = { 5.0f, 10.0f, 1.0f, 0.0f };

	// set the ambient, diffuse, specular and position for LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHTING); // enables lighting
	glEnable(GL_LIGHT0); // enables the 0th light
	glEnable(GL_COLOR_MATERIAL); // colors materials when lighting is enabled

	// enable specular lighting via materials
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMateriali(GL_FRONT, GL_SHININESS, 15);

	// enable smooth shading
	glShadeModel(GL_SMOOTH);

	// enable depth testing to be 'less than'
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// set the backbuffer clearing color to a lightish blue
	glClearColor(0.6, 0.65, 0.85, 0);
}

glut_app::glut_app(int argc, char * argv[])
{
	assert(_app == nullptr);
	_app = this;

	char const * title = "glut window";
	constexpr int width = 800,
		height = 600;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow(title);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	initialize();

	glutKeyboardFunc(keyboard_cb);
	glutKeyboardUpFunc(keyboard_up_cb);
	glutSpecialFunc(special_cb);
	glutSpecialUpFunc(special_up_cb);
	glutReshapeFunc(reshape_cb);
	glutIdleFunc(idle_cb);
	glutMouseFunc(mouse_cb);
	glutPassiveMotionFunc(motion_cb);
	glutMotionFunc(motion_cb);
	glutDisplayFunc(display_cb);
}

void glut_app::go()
{
	glutMainLoop();
}

void glut_app::reshape(int w, int h)
{
	_w = w;
	_h = h;
	glViewport(0, 0, _w, _h);
}

glut_app & glut_app::ref()
{
	assert(_app);
	return *_app;
}

void glut_app::keyboard_cb(unsigned char key, int x, int y)
{
	glut_app::ref().keyboard_event(key, x, y);
}

void glut_app::keyboard_up_cb(unsigned char key, int x, int y)
{
	glut_app::ref().keyboard_up_event(key, x, y);
}

void glut_app::special_cb(int key, int x, int y)
{
	glut_app::ref().special_event(key, x, y);
}

void glut_app::special_up_cb(int key, int x, int y)
{
	glut_app::ref().special_up_event(key, x, y);
}

void glut_app::reshape_cb(int w, int h)
{
	glut_app::ref().reshape(w, h);
}

void glut_app::idle_cb()
{
	glut_app::ref().idle();
}

void glut_app::mouse_cb(int button, int state, int x, int y)
{
	glut_app::ref().mouse_event(button, state, x, y);
}

void glut_app::motion_cb(int x, int y)
{
	glut_app::ref().motion_event(x, y);
}

void glut_app::display_cb()
{
	glut_app::ref().render();
}

}  // ui

class bullet_app : public ui::glut_app
{
public:
	bullet_app(int argc, char * argv[]);
	~bullet_app();

	void update(steady_clock::duration const & dt) override;
	void render() override;
	void reshape(int w, int h) override;

private:
	void keyboard_event(unsigned char key, int x, int y) override;

	void update_camera();
	void zoom_camera(float distance);

	physics::world _world;
	physics::body _ground,
		_sphere;
	collision_handler _impact_event;
	DebugDrawer * _ddraw;

	bool _impact = false;
	steady_clock::duration _t_impact = 0s;

	// camera stuff
	btVector3 m_cameraPosition; // the camera's current position
	btVector3 m_cameraTarget;	 // the camera's lookAt target
	float m_nearPlane; // minimum distance the camera will render
	float m_farPlane; // farthest distance the camera will render
	btVector3 m_upVector; // keeps the camera rotated correctly
	float m_cameraDistance; // distance from the camera to its target
	float m_cameraPitch; // pitch of the camera
	float m_cameraYaw; // yaw of the camera
};

void bullet_app::render()
{
	_world.native().debugDrawWorld();
}

// Some constants for 3D math and the camera speed
#define RADIANS_PER_DEGREE 0.01745329f
#define CAMERA_STEP_SIZE 5.0f

void bullet_app::keyboard_event(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'z': zoom_camera(+CAMERA_STEP_SIZE); break;
	case 'x': zoom_camera(-CAMERA_STEP_SIZE); break;
	}
}

void bullet_app::zoom_camera(float distance)
{
	// change the distance value
	m_cameraDistance -= distance;
	// prevent it from zooming in too far
	if (m_cameraDistance < 0.1f) m_cameraDistance = 0.1f;
	// update the camera since we changed the zoom distance
	update_camera();
}

void bullet_app::update_camera()
{
	// handlig camera with bullet math

	// exit in erroneous situations
	if (width() == 0 && height() == 0)
		return;

	// select the projection matrix
	glMatrixMode(GL_PROJECTION);
	// set it to the matrix-equivalent of 1
	glLoadIdentity();
	// determine the aspect ratio of the screen
	float aspectRatio = width() / (float)height();
	// create a viewing frustum based on the aspect ratio and the
	// boundaries of the camera
	glFrustum (-aspectRatio * m_nearPlane, aspectRatio * m_nearPlane, -m_nearPlane, m_nearPlane, m_nearPlane, m_farPlane);
	// the projection matrix is now set

	// select the view matrix
	glMatrixMode(GL_MODELVIEW);
	// set it to '1'
	glLoadIdentity();

	// our values represent the angles in degrees, but 3D
	// math typically demands angular values are in radians.
	float pitch = m_cameraPitch * RADIANS_PER_DEGREE;
	float yaw = m_cameraYaw * RADIANS_PER_DEGREE;

	// create a quaternion defining the angular rotation
	// around the up vector
	btQuaternion rotation(m_upVector, yaw);

	// set the camera's position to 0,0,0, then move the 'z'
	// position to the current value of m_cameraDistance.
	btVector3 cameraPosition(0,0,0);
	cameraPosition[2] = -m_cameraDistance;

	// create a Bullet Vector3 to represent the camera
	// position and scale it up if its value is too small.
	btVector3 forward(cameraPosition[0], cameraPosition[1], cameraPosition[2]);
	if (forward.length2() < SIMD_EPSILON) {
		forward.setValue(1.f,0.f,0.f);
	}

	// figure out the 'right' vector by using the cross
	// product on the 'forward' and 'up' vectors
	btVector3 right = m_upVector.cross(forward);

	// create a quaternion that represents the camera's roll
	btQuaternion roll(right, - pitch);

	// turn the rotation (around the Y-axis) and roll (around
	// the forward axis) into transformation matrices and
	// apply them to the camera position. This gives us the
	// final position
	cameraPosition = btMatrix3x3(rotation) * btMatrix3x3(roll) * cameraPosition;

	// save our new position in the member variable, and
	// shift it relative to the target position (so that we
	// orbit it)
	m_cameraPosition[0] = cameraPosition.getX();
	m_cameraPosition[1] = cameraPosition.getY();
	m_cameraPosition[2] = cameraPosition.getZ();
	m_cameraPosition += m_cameraTarget;

	// create a view matrix based on the camera's position and where it's
	// looking
	gluLookAt(m_cameraPosition[0], m_cameraPosition[1], m_cameraPosition[2], m_cameraTarget[0], m_cameraTarget[1], m_cameraTarget[2], m_upVector.getX(), m_upVector.getY(), m_upVector.getZ());
	// the view matrix is now set
}

bullet_app::bullet_app(int argc, char * argv[])
	: ui::glut_app{argc, argv}
	, _ground{make_unique<btBoxShape>(btVector3(1,50,50)), translate(btVector3(0.0f, 0.0f, 0.0f)), 0}
	, _sphere{make_unique<btSphereShape>(1), translate(btVector3{2, 10, 0}), 1}
	, _impact_event{&_ground, &_sphere}
	, m_cameraPosition(10.0f, 5.0f, 0.0f)
	, m_cameraTarget(0.0f, 0.0f, 0.0f)
	, m_nearPlane(1.0f)
	, m_farPlane(1000.0f)
	, m_upVector(0.0f, 1.0f, 0.0f)
	, m_cameraDistance(15.0f)
	, m_cameraPitch(20.0f)
	, m_cameraYaw(0.0f)
{
	_world.native().setGravity(btVector3{0, -10, 0});
	_world.add_body(&_ground);
	_world.add_body(&_sphere);
	_world.subscribe_collisions(&_impact_event);
	_ddraw = new DebugDrawer{};
	_ddraw->ToggleDebugFlag(btIDebugDraw::DBG_DrawWireframe);
	_ddraw->ToggleDebugFlag(btIDebugDraw::DBG_DrawAabb);
	_world.native().setDebugDrawer(_ddraw);
}

bullet_app::~bullet_app()
{
	_world.native().setDebugDrawer(nullptr);
	delete _ddraw;
}


void bullet_app::update(steady_clock::duration const & dt)
{
	ui::glut_app::update(dt);

	// update scene there ...
	_world.simulate(duration_cast<duration<btScalar>>(dt).count());

	// impact detection
	if (!_impact && _impact_event.finished())
	{
		cout << "ball hits ground in "
			<< duration_cast<duration<double>>(_t_impact).count() << "s\n";

		cout << "at " << _sphere.position() << " position\n";

		_impact = true;
	}
	else
		_t_impact += dt;

	update_camera();
}

void bullet_app::reshape(int w, int h)
{
	ui::glut_app::reshape(w, h);
	update_camera();
}


int main(int argc, char * argv[])
{
	bullet_app app{argc, argv};
	app.go();

	return 0;
}

btTransform translate(btVector3 const & v)
{
	btTransform T;  // uninitialized by default
	T.setIdentity();
	T.setOrigin(v);
	return T;
}
