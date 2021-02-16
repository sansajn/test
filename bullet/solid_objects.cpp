// OpenGL 1.0 based bullet objects debug visualization
#include <algorithm>
#include <memory>
#include <utility>
#include <chrono>
#include <set>
#include <iterator>
#include <iostream>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <unistd.h>
#include <boost/range/iterator_range.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/btBulletCollisionCommon.h>
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

// ASCII code for the escape key.
#define ESCAPE 27

constexpr int WIDTH = 1024,
	HEIGHT = 768;

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
	_ddraw->setDebugMode(0);
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

int window;  // The number of our GLUT window
float rtri = 0.0f;  // Rotation angle for the triangle.
float rquad = 0.0f;  // Rotation angle for the quadrilateral.
float const cube_position[3] = {1.5f, 0.0f, -7.0f},
	pyramid_position[3] = {-1.5f, 0.0f, -6.0f};

physics::world world;
unique_ptr<physics::body> cube;
unique_ptr<DebugDrawer> ddraw;

void init_gl(int Width, int Height);
void init_phys();
void resize_scene(int Width, int Height);
void draw_scene();
void idle();
void update(steady_clock::duration const & dt);

void update(steady_clock::duration const & dt)
{
	double const dt_s = duration_cast<duration<double>>(dt).count();

	rtri += 15.0 * dt_s;

	rquad -= 15.0f * dt_s;
	btTransform T_cube = btTransform{
		btQuaternion{btVector3{1, 1, 1}, btRadians(rquad)},
		btVector3{cube_position[0], cube_position[1], cube_position[2]}
	};
	cube->rigid_body().setWorldTransform(T_cube);

	world.simulate(dt_s);
}

void init_phys()
{
	world.native().setGravity(btVector3{0,0,0});  // turn gravity off

	ddraw = make_unique<DebugDrawer>();
	ddraw->ToggleDebugFlag(btIDebugDraw::DBG_DrawWireframe);
	ddraw->ToggleDebugFlag(btIDebugDraw::DBG_DrawAabb);
	world.native().setDebugDrawer(ddraw.get());

	cube = make_unique<physics::body>(
		make_unique<btBoxShape>(btVector3{1, 1, 1}),
		translate(btVector3{cube_position[0], cube_position[1], cube_position[2]}), 0);
	world.add_body(cube.get());
}

// The main drawing function.
void draw_scene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View

	glTranslatef(pyramid_position[0], pyramid_position[1], pyramid_position[2]);
	
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

	world.native().debugDrawWorld();

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
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);

	/* Open a window */
	window = glutCreateWindow("bullet sample ...");

	glutDisplayFunc(&draw_scene);
	glutIdleFunc(&idle);
	glutReshapeFunc(&resize_scene);
	glutKeyboardFunc(&key_pressed);

	init_gl(WIDTH, HEIGHT);
	init_phys();

	glutMainLoop();

	return 1;
}

void idle()
{
	steady_clock::time_point now = steady_clock::now();
	static steady_clock::time_point before = now;
	steady_clock::duration dt = now - before;
	before = now;

	update(dt);
	draw_scene();
}

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

btTransform translate(btVector3 const & v)
{
	btTransform T;  // uninitialized by default
	T.setIdentity();
	T.setOrigin(v);
	return T;
}
