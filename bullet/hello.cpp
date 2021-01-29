//! Bullet hello world sample (based on `<bullet>/examples/HelloWorld`).
#include <iostream>
#include <bullet/btBulletDynamicsCommon.h>

using std::cout;

btTransform translate(btVector3 const & v);


int main(int argc, char * argv[])
{
	//! initialization

	// collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration config;

	// use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher dispatcher{&config};

	// btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btDbvtBroadphase pair_cache;

	// the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver solver;

	btDiscreteDynamicsWorld world{&dispatcher, &pair_cache, &solver, &config};
	world.setGravity(btVector3{0, -10, 0});

	//! creating a few basic rigid bodies

	// the ground is a cube of side 100 at position y = -56.
	// the sphere will hit it at y = -6, with center at -5
	btBoxShape ground_shape{btVector3{50, 50, 50}};
	
	// using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState ground_motion{translate(btVector3{0, -56, 0})};

	btRigidBody ground_body{0, &ground_motion, &ground_shape};
	world.addRigidBody(&ground_body);

	// create a dynamic rigidbody
	btSphereShape sphere_shape{1};

	// using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState sphere_motion{translate(btVector3{2, 10, 0})};

	btScalar mass = 1;
	btVector3 local_inertia = {0, 0, 0};
	sphere_shape.calculateLocalInertia(mass, local_inertia);

	btRigidBody sphere_body{mass, &sphere_motion, &sphere_shape, local_inertia};
	world.addRigidBody(&sphere_body);

	//! do the simulation
	for (int i = 0; i < 150; ++i)
	{
		world.stepSimulation(1.f / 60, 10);

		for (int j = world.getNumCollisionObjects() - 1; j >= 0; --j)
		{
			btCollisionObject * obj = world.getCollisionObjectArray()[j];

			btTransform trans;
			btRigidBody * body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
				body->getMotionState()->getWorldTransform(trans);
			else
				trans = obj->getWorldTransform();

			cout << "world pos object " << j << " = "
				<< trans.getOrigin().getX() << ", " << trans.getOrigin().getY() << ", " << trans.getOrigin().getZ() << "\n";
		}
	}

	cout << "done!" << std::endl;

	return 0;
}

btTransform translate(btVector3 const & v)
{
	btTransform T;  // uninitialized by default
	T.setIdentity();
	T.setOrigin(v);
	return T;
}
