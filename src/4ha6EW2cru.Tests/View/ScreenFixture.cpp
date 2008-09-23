#include "ScreenFixture.h"

#include "Ogre.h"
using namespace Ogre;

#include "IO/FileManager.h"
#include "Events/EventManager.h"

#include "Logging/AppenderFactory.h"
#include "Logging/Logger.h"

#include "View/Screen.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ScreenFixture, Suites::ViewSuite( ) );

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

void ScreenFixture::Should_Throw_On_Initialize_Given_NULL_Gui( )
{
	FileBuffer* buffer = FileManager::GetInstance( )->GetFile( "testscript.lua" );
	Script* script = Script::CreateFromFileBuffer( buffer );
	script->Initialize( );

	Screen screen( "randomscreen" );
	CPPUNIT_ASSERT_THROW( screen.Initialize( 0, script ), NullReferenceException );
}

void ScreenFixture::Should_Throw_On_Initialize_Given_NULL_Script( )
{
	Screen screen( "randomscreen" );
	CPPUNIT_ASSERT_THROW( screen.Initialize( _gui, 0 ), NullReferenceException );
}

void ScreenFixture::Should_Throw_On_Initialize_Given_Already_Initialized( )
{

}

void ScreenFixture::Should_Initialize_Given_Valid_Parameters( )
{
	FileManager::GetInstance( )->AddFileStore( "../game/test/gui/screens/testscreen" );

	FileBuffer* buffer = FileManager::GetInstance( )->GetFile( "testscreen.lua" );
	Script* script = Script::CreateFromFileBuffer( buffer );
	script->Initialize( );

	Screen screen( "testscreen" );
	screen.Initialize( _gui, script );
}

void ScreenFixture::Should_Return_ScreenName( )
{

}

void ScreenFixture::Should_Return_VisibilityMask( )
{

}