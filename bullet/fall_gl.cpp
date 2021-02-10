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
#include "DebugDrawer.h"

using std::find;
using std::unique_ptr, std::make_unique;
using std::move, std::make_pair;
using std::chrono::milliseconds, std::chrono::duration, std::chrono::duration_cast;
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

namespace gui {

class glut_app
{
public:
	static glut_app & ref();
	void go();  // blocking

protected:
	void keyboard_event(unsigned char key, int x, int y);
	void keyboard_up_event(unsigned char key, int x, int y);
	void special_event(int key, int x, int y);
	void special_up_event(int key, int x, int y);
	void reshape_event(int w, int h);
	void idle();
	void mouse_event(int button, int state, int x, int y);
	void motion_event(int x, int y);
	void display();

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
};

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
	glut_app::ref().reshape_event(w, h);
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
	glut_app::ref().display();
}

}  // gui


int main(int argc, char * argv[])
{
	//! initialization
	physics::world world;
	world.native().setGravity(btVector3{0, -10, 0});

	// the ground is a cube of side 100 at position y = -56.
	physics::body ground{make_unique<btBoxShape>(btVector3{50, 50, 50}),
		translate(btVector3{0, -56, 0}), 0};
	world.add_body(&ground);

	// create a dynamic rigidbody (mass > 0)
	btScalar mass = 1;
	physics::body sphere{make_unique<btSphereShape>(1), translate(btVector3{2, 10, 0}),
		mass};
	world.add_body(&sphere);

	collision_handler impact_event{&ground, &sphere};
	world.subscribe_collisions(&impact_event);

	milliseconds t_impact = 0ms;
	milliseconds const time_step = 10ms;

	//! do the simulation
	for (int i = 0; i < 300; ++i)
	{
		world.simulate(time_step.count() / 1000.f, 10);
		t_impact += time_step;

		if (impact_event.finished())
		{
			cout << "ball hits ground in "
				<< duration_cast<duration<double>>(t_impact).count() << "s\n";

			cout << "at " << sphere.position() << " position\n";

			break;  // impact detected, we are done
		}
	}

	cout << "done!" << std::endl;

	gui::glut_app::ref()
		.go();

	return 0;
}

btTransform translate(btVector3 const & v)
{
	btTransform T;  // uninitialized by default
	T.setIdentity();
	T.setOrigin(v);
	return T;
}
