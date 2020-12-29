// scene with cameraman floor and model
#include <memory>
#include <iostream>
#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreCameraMan.h>

using std::unique_ptr, std::make_unique;
using std::cout, std::endl;
using namespace Ogre;
using namespace OgreBites;

class ogre_app
	: public ApplicationContext, public InputListener
{
public:
	void go();
	void setup() override;

	// user input
	bool keyPressed(KeyboardEvent const & evt) override;
	bool keyReleased(KeyboardEvent const & evt) override;
	bool mouseMoved(MouseMotionEvent const & evt) override;
	bool mousePressed(MouseButtonEvent const & evt) override;
	bool mouseReleased(MouseButtonEvent const & evt) override;
	void frameRendered(Ogre::FrameEvent const & evt) override;

private:
	unique_ptr<CameraMan> _cameraman;
};

void ogre_app::setup()
{
	ApplicationContext::setup();
	addInputListener(this);

	SceneManager * scene = getRoot()->createSceneManager();
	scene->setAmbientLight(ColourValue{.5, .5, .5});

	// register our scene with the RTSS
	RTShader::ShaderGenerator * shadergen = RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(scene);

	SceneNode * root_nd = scene->getRootSceneNode();

	// add light
	SceneNode * light_nd = root_nd->createChildSceneNode();
	light_nd->setPosition(20, 80, 50);
	Light * light = scene->createLight("light");
	light_nd->attachObject(light);

	// floor
	MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Plane(Vector3::UNIT_Y, -1), 250, 250, 25, 25, true, 1, 15, 15, Vector3::UNIT_Z);

	Entity * floor = scene->createEntity("Floor", "floor");
	floor->setMaterialName("Examples/Rockwall");
	floor->setCastShadows(false);
	root_nd->attachObject(floor);

	// add ogre head
	SceneNode * head_nd = root_nd->createChildSceneNode();
	head_nd->setPosition(Vector3{0, 30, 0});
	Entity * head = scene->createEntity("ogrehead.mesh");
	head_nd->attachObject(head);

	// add camera
	SceneNode * cam_nd = root_nd->createChildSceneNode();
	cam_nd->setPosition(0, 30, 140);
	cam_nd->lookAt(Vector3{0, 0, -1},Node::TS_PARENT);
	Camera * cam = scene->createCamera("camera");
	cam->setNearClipDistance(5);
	cam->setAutoAspectRatio(true);
	cam_nd->attachObject(cam);

	getRenderWindow()->addViewport(cam);

	// cameraman
	_cameraman = make_unique<CameraMan>(cam_nd);
	_cameraman->setStyle(CS_FREELOOK);

	setWindowGrab();  // grab mouse
}

bool ogre_app::keyPressed(KeyboardEvent const & evt)
{
	return _cameraman->keyPressed(evt);
}

bool ogre_app::keyReleased(KeyboardEvent const & evt)
{
	return _cameraman->keyReleased(evt);
}

bool ogre_app::mouseMoved(MouseMotionEvent const & evt)
{
	return _cameraman->mouseMoved(evt);
}

bool ogre_app::mousePressed(MouseButtonEvent const & evt)
{
	return _cameraman->mousePressed(evt);
}

bool ogre_app::mouseReleased(MouseButtonEvent const & evt)
{
	return _cameraman->mouseReleased(evt);
}

void ogre_app::frameRendered(Ogre::FrameEvent const & evt)
{
	_cameraman->frameRendered(evt);
}

void ogre_app::go()
{
	initApp();

	if (getRoot()->getRenderSystem())
		getRoot()->startRendering();

	closeApp();
}

int main(int argc, char * argv[])
{
	ogre_app app;
	app.go();
	cout << "done\n";
	return 0;
}
