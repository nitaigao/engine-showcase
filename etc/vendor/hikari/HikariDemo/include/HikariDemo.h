#ifndef __NaviDemo_H__
#define __NaviDemo_H__

#include "Ogre.h"
#include "InputManager.h"
#include "Hikari.h"

class HikariDemo : public OIS::MouseListener, public OIS::KeyListener, public Ogre::WindowEventListener
{
	Ogre::Root* root;
	Ogre::RenderWindow* renderWin;
	Ogre::SceneManager* sceneMgr;
	Ogre::Viewport* viewport;
	InputManager* inputMgr;
	Hikari::HikariManager* hikariMgr;
	Hikari::FlashControl *fps, *controls;
	unsigned long lastTime;
	Ogre::Timer timer;
	void parseResources();
	void loadInputSystem();
	void updateStats();
public:
	bool shouldQuit;
	HikariDemo();
	~HikariDemo();

	void createScene();
	void setupHikari();

	void update();

	Hikari::FlashValue onOpacityChange(Hikari::FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue onColorChange(Hikari::FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue onExitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);
	
	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );	

	void windowMoved(Ogre::RenderWindow* rw);
	void windowResized(Ogre::RenderWindow* rw);
	void windowClosed(Ogre::RenderWindow* rw);
	void windowFocusChange(Ogre::RenderWindow* rw);
};

#endif