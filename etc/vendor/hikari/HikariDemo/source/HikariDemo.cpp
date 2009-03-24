#include "HikariDemo.h"

using namespace Ogre;
using namespace Hikari;

HikariDemo::HikariDemo() : root(0), shouldQuit(false), renderWin(0), sceneMgr(0), hikariMgr(0), inputMgr(0)
{
	lastTime = timer.getMilliseconds();

	root = new Root();

	shouldQuit = !root->showConfigDialog();
	if(shouldQuit) return;

	renderWin = root->initialise(true, "HikariDemo");
	sceneMgr = root->createSceneManager("DefaultSceneManager");
	WindowEventUtilities::addWindowEventListener(renderWin, this);

	createScene();

	setupHikari();

	loadInputSystem();
}

HikariDemo::~HikariDemo()
{
	WindowEventUtilities::removeWindowEventListener(renderWin, this);

	if(inputMgr)
		delete inputMgr;
	if(hikariMgr)
		delete hikariMgr;
	if(root)
		delete root;
}

void HikariDemo::createScene()
{
	sceneMgr->setAmbientLight(ColourValue::White);
	sceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_ADDITIVE);

	Camera* camera = sceneMgr->createCamera("MainCam");
	viewport = renderWin->addViewport(camera);
	viewport->setBackgroundColour(ColourValue(0.2, 0.2, 0.2));

	camera->setAspectRatio((float)viewport->getActualWidth() / (float)viewport->getActualHeight());
}

void HikariDemo::setupHikari()
{
	hikariMgr = new HikariManager("..\\media");

	fps = hikariMgr->createFlashOverlay("FPS", viewport, 130, 91, Position(TopLeft));
	fps->load("fps.swf");
	fps->setTransparent(true);
	fps->setDraggable(false);

	controls = hikariMgr->createFlashOverlay("Controls", viewport, 350, 400, Position(Center));
	controls->load("controls.swf");
	controls->setTransparent(true, true);
	controls->bind("opacityChange", FlashDelegate(this, &HikariDemo::onOpacityChange));
	controls->bind("colorChange", FlashDelegate(this, &HikariDemo::onColorChange));
	controls->bind("exitClick", FlashDelegate(this, &HikariDemo::onExitClick));
}

void HikariDemo::update()
{
	hikariMgr->update();
	root->renderOneFrame();
	Ogre::WindowEventUtilities::messagePump();
	inputMgr->capture();

	const RenderTarget::FrameStats& stats = renderWin->getStatistics();
	fps->callFunction("setFPS", Args((int)stats.lastFPS));
}

Hikari::FlashValue HikariDemo::onOpacityChange(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	Real opacityPercent = args.at(0).getNumber();

	caller->setOpacity(opacityPercent / 100);

	return FLASH_VOID;
}

Hikari::FlashValue HikariDemo::onColorChange(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	ColourValue color = args.at(0).getNumberAsColor();

	viewport->setBackgroundColour(color);

	return FLASH_VOID;
}

Hikari::FlashValue HikariDemo::onExitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args)
{
	shouldQuit = true;

	return FLASH_VOID;
}

void HikariDemo::loadInputSystem()
{
	inputMgr = InputManager::getSingletonPtr();
    inputMgr->initialise(renderWin);
    inputMgr->addMouseListener(this, "MouseListener");
	inputMgr->addKeyListener(this, "KeyListener");
}

bool HikariDemo::mouseMoved(const OIS::MouseEvent &arg)
{
	return hikariMgr->injectMouseMove(arg.state.X.abs, arg.state.Y.abs) || hikariMgr->injectMouseWheel(arg.state.Z.rel);
}

bool HikariDemo::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	return hikariMgr->injectMouseDown(id);
}

bool HikariDemo::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	return hikariMgr->injectMouseUp(id);
}

bool HikariDemo::keyPressed( const OIS::KeyEvent &arg )
{
	if(hikariMgr->isAnyFocused())
		return true;

	return true;
}

bool HikariDemo::keyReleased( const OIS::KeyEvent &arg )
{
	switch(arg.key)
	{
	case OIS::KC_ESCAPE:
		shouldQuit = true;
		break;
	case OIS::KC_F1:
	{
		const RenderTarget::FrameStats& stats = renderWin->getStatistics();
		Ogre::LogManager::getSingleton().logMessage("Current FPS: " + StringConverter::toString(stats.lastFPS));
		Ogre::LogManager::getSingleton().logMessage("Triangle Count: " + StringConverter::toString(stats.triangleCount));
		Ogre::LogManager::getSingleton().logMessage("Batch Count: " + StringConverter::toString(stats.batchCount));
		break;
	}
	case OIS::KC_F2:
		sceneMgr->getRootSceneNode()->flipVisibility(true);
		break;
	}

	return true;
}

void HikariDemo::windowMoved(RenderWindow* rw) {}

void HikariDemo::windowResized(RenderWindow* rw) 
{
	inputMgr->setWindowExtents(rw->getWidth(), rw->getHeight());
}

void HikariDemo::windowClosed(RenderWindow* rw) 
{
	shouldQuit = true;
}

void HikariDemo::windowFocusChange(RenderWindow* rw) {}
