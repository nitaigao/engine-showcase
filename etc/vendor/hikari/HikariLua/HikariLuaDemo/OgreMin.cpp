#include <OIS\OIS.h>
#include "Ogre.h" 
#include "OgreFrameListener.h" 
#include <Hikari.h>
#include <HikariLua.h>
#include <updatable.h>

using namespace Ogre; 

void initLuaGlue(lua_State *pLua);

// your program will do much better than this... its a demo dammit!
bool shouldQuit;  // moved out the framelistener to allow easy access
Viewport* viewPort = NULL; // moved out of main to allow easy access

void QuitProgram(void)
{
	shouldQuit = true;
}

void SetViewportColor(Ogre::ColourValue c)
{
	viewPort->setBackgroundColour(c);
}

////////////////////////////////////////////////////////////////////////// 
// class CSimpleFrameListener 
////////////////////////////////////////////////////////////////////////// 
class CSimpleFrameListener : public FrameListener,  public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener
{ 
public: 
	CSimpleFrameListener(Ogre::RenderWindow* win)
	{
		shouldQuit = false;
		mRenderWin = win;

		mInputManager = (NULL);

		using namespace OIS;
		Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
		ParamList pl;
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;

		win->getCustomAttribute("WINDOW", &windowHnd);
		windowHndStr << windowHnd;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
		//Default mode is foreground exclusive..but, we want to show mouse - so nonexclusive
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));

		mInputManager = InputManager::createInputSystem( pl );

		//Create all devices 
		mKeyboard = static_cast<Keyboard*>(mInputManager->createInputObject( OISKeyboard, true ));
		mMouse = static_cast<Mouse*>(mInputManager->createInputObject( OISMouse, true ));

		windowResized(win);

		mMouse->setEventCallback(this);
		mKeyboard->setEventCallback(this);

		Ogre::WindowEventUtilities::addWindowEventListener(win, this);

	}

	~CSimpleFrameListener() 
	{
		windowClosed(mRenderWin);
		Ogre::WindowEventUtilities::removeWindowEventListener(mRenderWin, this);
	};

	// Most code will go here. This gets called every frame
	bool frameStarted(const FrameEvent& evt) 
	{ 
	if(mKeyboard)
		mKeyboard->capture();
	if(mMouse)
		mMouse->capture();

	CUpdatable::ProcessUpdates(1);	

	if(shouldQuit)
		return false;



		return true; 
	} 
	// This gets called at the end of a frame. It's not necessary to use it.
	bool frameEnded(const FrameEvent& evt) 
	{ 
		return true; 
	} 

	bool mouseMoved (const OIS::MouseEvent &arg)
	{
//		if(arg.state.Z.rel != 0) Hikari::HikariManager::Get().injectMouseWheel(arg.state.Z.rel);
		return Hikari::HikariManager::Get().injectMouseMove(arg.state.X.abs, arg.state.Y.abs);
	}

	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		Hikari::HikariManager::Get().injectMouseDown(id);
		return true;
	}

	bool mouseReleased (const OIS::MouseEvent &arg, OIS::MouseButtonID id)
	{
		Hikari::HikariManager::Get().injectMouseUp(id);
		return true;
	}

	bool keyPressed(const OIS::KeyEvent &e)
	{
		return true;
	}

	bool keyReleased(const OIS::KeyEvent &e)
	{
		if(e.key == OIS::KC_ESCAPE)
			shouldQuit = true;
		return true;
	}

	//Adjust mouse clipping area
	void windowResized(Ogre::RenderWindow* rw)
	{
		unsigned int width, height, depth;
		int left, top;
		rw->getMetrics(width, height, depth, left, top);

		const OIS::MouseState &ms = mMouse->getMouseState();
		ms.width = width;
		ms.height = height;
	}

	//Unattach OIS before window shutdown (very important under Linux)
	void windowClosed(Ogre::RenderWindow* rw)
	{
		//Only close for window that created OIS (the main window in these demos)
		if( rw == mRenderWin )
		{
			if( mInputManager )
			{
				mInputManager->destroyInputObject( mMouse );
				mInputManager->destroyInputObject( mKeyboard );
				mMouse = 0;
				mKeyboard = 0;

				OIS::InputManager::destroyInputSystem(mInputManager);
				mInputManager = 0;

			}
		}
		shouldQuit = true;
	}

private:
	Ogre::RenderWindow* mRenderWin;

	//OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;
}; 

////////////////////////////////////////////////////////////////////////// 
// class main 
////////////////////////////////////////////////////////////////////////// 

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 
#define WIN32_LEAN_AND_MEAN 
#include "windows.h" 
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT) 
#else 
int main(int argc, char **argv) 
#endif 
{ 
	//----------------------------------------------------- 
	// 1 enter ogre 
	//----------------------------------------------------- 
	Root* root = new Root("plugins.cfg", "ogre.cfg"); 

	//----------------------------------------------------- 
	// 2 configure resource paths
	//----------------------------------------------------- 
	// Load resource paths from config file

	// File format is:
	//  [ResourceGroupName]
	//  ArchiveType=Path
	//  .. repeat
	// For example:
	//  [General]
	//  FileSystem=media/
	//  Zip=packages/level1.zip

	ConfigFile cf;
	cf.load("resources.cfg");

	// Go through all sections & settings in the file
	ConfigFile::SectionIterator seci = cf.getSectionIterator();

	String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
	//----------------------------------------------------- 
	// 3 Configures the application and creates the window
	//----------------------------------------------------- 
	if(!root->showConfigDialog())
		return false;                                 // Exit the application on cancel

	RenderWindow* window = root->initialise(true, "Simple Ogre/Navi App");

	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	//----------------------------------------------------- 
	// 4 Create the SceneManager
	// 
	//		ST_GENERIC = octree
	//		ST_EXTERIOR_CLOSE = simple terrain
	//		ST_EXTERIOR_FAR = nature terrain (depreciated)
	//		ST_EXTERIOR_REAL_FAR = paging landscape
	//		ST_INTERIOR = Quake3 BSP
	//----------------------------------------------------- 
	SceneManager* sceneMgr = root->createSceneManager(ST_GENERIC); 

	//----------------------------------------------------- 
	// 5 Create the camera 
	//----------------------------------------------------- 
	Camera* camera = sceneMgr->createCamera("SimpleCamera"); 

	//----------------------------------------------------- 
	// 6 Create one viewport, entire window 
	//----------------------------------------------------- 
	viewPort = window->addViewport(camera);

	//---------------------------------------------------- 
	// 7 add event processor/ Frame listener
	//----------------------------------------------------

	CSimpleFrameListener* frameListener = new CSimpleFrameListener(window);
	root->addFrameListener(frameListener); 

	//----------------------------------------------------
	// 8 start Hikari
	//----------------------------------------------------
	initLuaGlue(initHikariLua(NULL,viewPort, "..\\media"));
	RunLuaScript("..\\Boot.lua");

	//----------------------------------------------------
	// 9 start rendering 
	//----------------------------------------------------
	root->startRendering(); // blocks until a frame listener returns false. eg from pressing escape in this example

	//----------------------------------------------------
	// 10 clean 
	//----------------------------------------------------
	delete frameListener; 
	delete root;    

	return 0; 
}


