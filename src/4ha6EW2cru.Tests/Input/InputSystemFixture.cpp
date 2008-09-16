#include "InputSystemFixture.h"

#include "Input/InputSystem.h"
#include "Logging/Logger.h"

#include "Ogre.h"
using namespace Ogre;

CPPUNIT_TEST_SUITE_REGISTRATION( InputSystemFixture );

void InputSystemFixture::setUp( )
{
	Logger::Initialize( );
}

void InputSystemFixture::tearDown( )
{
	Logger::GetInstance( )->Release( );
}

void InputSystemFixture::Should_Initialize_Properly( )
{
	Root* root = new Root( );

	Ogre::LogManager::getSingletonPtr( )->destroyLog( Ogre::LogManager::getSingletonPtr( )->getDefaultLog( ) );
	Ogre::LogManager::getSingletonPtr( )->createLog( "default", true, false, true );

	root->loadPlugin( "RenderSystem_Direct3D9_d" );

	RenderSystemList *renderSystems = root->getAvailableRenderers( );
	RenderSystemList::iterator renderSystemIterator = renderSystems->begin( );

	root->setRenderSystem( *renderSystemIterator );
	
	( *renderSystemIterator )->setConfigOption( "Full Screen", "No" );
	( *renderSystemIterator )->setConfigOption( "Video Mode", "640 x 480 @ 32-bit colour" );

	root->initialise( true, "Human View" );

	RenderWindow* window = root->getAutoCreatedWindow( );

	size_t hWnd = 0;
	window->getCustomAttribute( "WINDOW", &hWnd );
	std::stringstream hWndParam;
	hWndParam << hWnd;

	OIS::ParamList params;
	params.insert( std::make_pair( std::string("WINDOW"), hWndParam.str( ) ) );		

	OIS::InputManager* oisManager = OIS::InputManager::createInputSystem( params );

	InputSystem* inputSystem = new InputSystem( oisManager );
	bool result = inputSystem->Initialize( );

	CPPUNIT_ASSERT( result );

	inputSystem->Release( );
	delete inputSystem;

	root->shutdown( );
	delete root;
}

void InputSystemFixture::Should_Not_Initialize_With_Null_OISInputManager( )
{	
	InputSystem* inputSystem = new InputSystem( 0 );
	bool result = inputSystem->Initialize( );

	CPPUNIT_ASSERT( !result );

	delete inputSystem;
}