#include "ScreenFixture.h"

#include "Ogre.h"
using namespace Ogre;

#include "IO/FileManager.h"
#include "Events/EventManager.h"

#include "Logging/AppenderFactory.h"
#include "Logging/Logger.h"

#include "View/Screen.h"

CPPUNIT_TEST_SUITE_REGISTRATION( ScreenFixture );

void ScreenFixture::setUp( )
{
	Logger::Initialize( );
	Logger::GetInstance( )->AddAppender( AppenderFactory::CreateAppender( CONSOLEAPPENDER ) );

	EventManager::Initialize( );

	FileManager::Initialize( );
	FileManager::GetInstance( )->AddFileStore( "../game/test" );

	Root* root = new Root( );
	
	Ogre::LogManager::getSingletonPtr( )->destroyLog( Ogre::LogManager::getSingletonPtr( )->getDefaultLog( ) );
	Ogre::LogManager::getSingletonPtr( )->createLog( "default", true, false, true );

	root->loadPlugin( "RenderSystem_Direct3D9_d" );

	RenderSystemList *renderSystems = root->getAvailableRenderers( );
	RenderSystemList::iterator renderSystemIterator = renderSystems->begin( );

	root->setRenderSystem( *renderSystemIterator );

	( *renderSystemIterator )->setConfigOption( "Full Screen", "No" );
	( *renderSystemIterator )->setConfigOption( "Video Mode", "640 x 480 @ 32-bit colour" );

	SceneManager* sceneManager = root->createSceneManager( ST_GENERIC, "default" );

	root->initialise( true, "Human View" );

	Camera* camera = sceneManager->createCamera( "default camera" );
	camera->setPosition( Vector3( 0, 20, 100 ) );
	camera->lookAt( Vector3( 0, 0, 0 ) );
	camera->setNearClipDistance( 1.0f );

	Viewport* viewPort = root->getAutoCreatedWindow( )->addViewport( camera );
	viewPort->setBackgroundColour( ColourValue( 0, 0, 0 ) );

	_gui = new Gui( );
	_gui->initialise( root->getAutoCreatedWindow(  ) );
}

void ScreenFixture::tearDown( )
{
	_gui->shutdown( );
	delete _gui;

	Root* root = Root::getSingletonPtr( );
	root->shutdown( );
	delete root;
	
	FileManager::GetInstance( )->Release( );
	EventManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void ScreenFixture::Should_Fail_Intialization_No_Gui( )
{
	FileBuffer* buffer = FileManager::GetInstance( )->GetFile( "../game/test/testscript.lua" );
	Script* script = Script::CreateFromFileBuffer( buffer );
	script->Initialize( );

	Screen screen( "randomscreen" );
	CPPUNIT_ASSERT_THROW( screen.Initialize( 0, script ), std::exception* );
}

void ScreenFixture::Should_Fail_Initialization_No_Script( )
{
	Screen screen( "randomscreen" );
	CPPUNIT_ASSERT_THROW( screen.Initialize( _gui, 0 ), std::exception* );
}

void ScreenFixture::Should_SuccessFully_Initialize( )
{
	FileManager::GetInstance( )->AddFileStore( "../game/test/gui/screens/testscreen" );

	FileBuffer* buffer = FileManager::GetInstance( )->GetFile( "testscreen.lua" );
	Script* script = Script::CreateFromFileBuffer( buffer );
	script->Initialize( );

	Screen screen( "testscreen" );
	bool result = screen.Initialize( _gui, script );

	CPPUNIT_ASSERT( result );
}