// OGRE 1.12 ImGui sample
#include <memory>
#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreImGuiOverlay.h>
#include <OgreImGuiInputListener.h>
#include <OgreOverlayManager.h>
#include <OgreOverlaySystem.h>

using std::unique_ptr, std::make_unique;
using namespace Ogre;
using namespace OgreBites;

class ogre_app
	: public ApplicationContext, public InputListener, public RenderTargetListener
{
public:
	ogre_app();
	~ogre_app() {}

private:
	void setup() override;

	// InputListener events
	bool keyPressed(KeyboardEvent const & evt) override;
	bool keyReleased(KeyboardEvent const & evt) override;
	bool mouseMoved(MouseMotionEvent const & evt) override;
	bool mouseWheelRolled(MouseWheelEvent const & evt) override;
	bool mousePressed(MouseButtonEvent const & evt) override;
	bool mouseReleased(MouseButtonEvent const & evt) override;
	bool textInput(TextInputEvent const & evt) override;

	// RenderTargetListener events
	void preViewportUpdate(RenderTargetViewportEvent const & evt) override;

	unique_ptr<ImGuiInputListener> _imgui_listener;
};

ogre_app::ogre_app()
	: ApplicationContext{"OgreImGuiSampleApp"}
{}

void ogre_app::setup()
{
	ApplicationContext::setup();
	addInputListener(this);  // register for input events

	SceneManager * scene = getRoot()->createSceneManager();

	// setup imgui overlay
	{
		ImGuiOverlay * imgui = new ImGuiOverlay();
		imgui->setZOrder(300);
		imgui->show();
		OverlayManager::getSingleton().addOverlay(imgui);  // imgui is now owned by OverlayManager
	}

	scene->addRenderQueueListener(getOverlaySystem());

	getRenderWindow()->addListener(this);  // register for render targets events

	_imgui_listener = make_unique<ImGuiInputListener>();


	// setup scene

	scene->setAmbientLight(ColourValue{0.5, 0.5, 0.5});

	// register our scene with the RTSS
	RTShader::ShaderGenerator * shadergen =
		RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(scene);

	SceneNode * root_node = scene->getRootSceneNode();

	// without light we would just get a black screen
	Light * light = scene->createLight("MainLight");
	SceneNode * light_node = root_node->createChildSceneNode();
	light_node->setPosition(20, 80, 50);
	light_node->attachObject(light);

	// create camera so we can observe scene
	Camera * camera = scene->createCamera("MainCamera");
	camera->setNearClipDistance(5);  // specific to this sample
	camera->setAutoAspectRatio(true);
	SceneNode * camera_node = root_node->createChildSceneNode();
	camera_node->setPosition(0, 0, 140);
	camera_node->lookAt(Vector3{0, 0, -1}, Node::TS_PARENT);
	camera_node->attachObject(camera);

	getRenderWindow()->addViewport(camera);  // render into the main window

	// finally something to render
	Entity * ent = scene->createEntity("ogrehead.mesh");
	SceneNode * node = root_node->createChildSceneNode();
	node->attachObject(ent);
}

bool ogre_app::keyPressed(KeyboardEvent const & evt)
{
	if (evt.keysym.sym == SDLK_ESCAPE)
	{
		getRoot()->queueEndRendering();
		return false;
	}
	else
		return _imgui_listener->keyPressed(evt);
}

bool ogre_app::keyReleased(KeyboardEvent const & evt)
{
	return _imgui_listener->keyReleased(evt);
}

bool ogre_app::mouseMoved(MouseMotionEvent const & evt)
{
	return _imgui_listener->mouseMoved(evt);
}

bool ogre_app::mouseWheelRolled(MouseWheelEvent const & evt)
{
	return _imgui_listener->mouseWheelRolled(evt);
}

bool ogre_app::mousePressed(MouseButtonEvent const & evt)
{
	return _imgui_listener->mousePressed(evt);
}

bool ogre_app::mouseReleased(MouseButtonEvent const & evt)
{
	return _imgui_listener->mouseReleased(evt);
}

bool ogre_app::textInput(TextInputEvent const & evt)
{
	return _imgui_listener->textInput(evt);
}

void ogre_app::preViewportUpdate(RenderTargetViewportEvent const & evt)
{
	if (!evt.source->getOverlaysEnabled())
		return;

	ImGuiOverlay::NewFrame();

	ImGui::ShowDemoWindow();
}


int main(int argc, char * argv[])
{
	ogre_app app;
	app.initApp();
	app.getRoot()->startRendering();
	app.closeApp();
	return 0;
}
