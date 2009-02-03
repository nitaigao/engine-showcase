#include "OgreRenderer.h"

#include "../Common/Paths.hpp"
#include "../Logging/Logger.h"
#include "../IO/BadArchiveFactory.h"

#include "../Events/EventManager.h"
#include "../Events/Event.h"

#include "../IO/FileManager.h"

#include "../Exceptions/ScreenDimensionsException.hpp"
#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/ArchiveNotFoundException.hpp"

#include "../Scripting/ScriptManager.h"

using namespace Ogre;
using namespace MyGUI;

OgreRenderer::~OgreRenderer( )
{
	EventManager::GetInstance( )->RemoveEventListener( INPUT_MOUSE_PRESSED, this, &OgreRenderer::OnMousePressed );
	EventManager::GetInstance( )->RemoveEventListener( INPUT_MOUSE_MOVED, this, &OgreRenderer::OnMouseMoved );
	EventManager::GetInstance( )->RemoveEventListener( INPUT_MOUSE_RELEASED, this, &OgreRenderer::OnMouseReleased );
	EventManager::GetInstance( )->RemoveEventListener( INPUT_KEY_DOWN, this, &OgreRenderer::OnKeyDown );
	EventManager::GetInstance( )->RemoveEventListener( INPUT_KEY_UP, this, &OgreRenderer::OnKeyUp );

	if( _rootUIController != 0 )
	{
		delete _rootUIController;
	}

	if ( _gui != 0 )
	{
		_gui->shutdown( );
		delete _gui;
		_gui = 0;
	}

	if ( _root != 0 )
	{
		_root->shutdown( );
		delete _root;
		_root = 0;
	}


	delete _scene;
	delete _badFactory;
	_badFactory = 0;
}

Gui* OgreRenderer::GetGui( )
{
	if ( !_isInitialized )
	{
		UnInitializedException e( "OgreRenderer::GetGui - Renderer isn't initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	return _gui;
}

void OgreRenderer::Initialize( int width, int height, int colorDepth, bool fullScreen )
{
	{	// -- Ogre Init

		if ( width < 1 || height < 1 || fullScreen < 0 )
		{
			throw ScreenDimensionsException( );
		}

		_root = new Root( );

		// Switches off the Ogre Logging unless the whole game is in debug mode
		if ( Logger::GetInstance( )->GetLogLevel( ) < WARNA )
		{
			Ogre::LogManager::getSingletonPtr( )->destroyLog( Ogre::LogManager::getSingletonPtr( )->getDefaultLog( ) );
			Ogre::LogManager::getSingletonPtr( )->createLog( "default", true, false, true );
		}

		_root->loadPlugin( "RenderSystem_Direct3D9_d" );
		
		_badFactory = new BadArchiveFactory( );
		ArchiveManager::getSingletonPtr( )->addArchiveFactory( _badFactory );
		this->LoadResources( );

		ResourceGroupManager::getSingleton( ).initialiseAllResourceGroups( );

		RenderSystemList *renderSystems = _root->getAvailableRenderers( );
		RenderSystemList::iterator renderSystemIterator = renderSystems->begin( );

		_root->setRenderSystem( *renderSystemIterator );
	
		std::stringstream videoModeDesc;
		videoModeDesc << width << " x " << height << " @ 32-bit colour";

		( *renderSystemIterator )->setConfigOption( "Full Screen", fullScreen ? "Yes" : "No" );
		( *renderSystemIterator )->setConfigOption( "Video Mode", videoModeDesc.str( ) );

		_root->initialise( true, "Human View" );

		SceneManager* sceneManager = _root->createSceneManager( ST_GENERIC, "default" );

		Camera* camera = sceneManager->createCamera( "default camera" );
		camera->setPosition( Vector3( 0, 0, 5 ) );
		camera->lookAt( Vector3( 0, 0, 0 ) );
		camera->setNearClipDistance( 1.0f );

		Viewport* viewPort = _root->getAutoCreatedWindow( )->addViewport( camera );
		viewPort->setBackgroundColour( ColourValue( 0, 0, 0 ) );

		camera->setAspectRatio( 
			Real( viewPort->getActualWidth( )) / Real( viewPort->getActualHeight( ) )
			);
	}

	{	// -- MyGUI 
		_gui = new Gui( );
		_gui->initialise( _root->getAutoCreatedWindow( ), "/data/gui/core/core.xml" );
		_gui->hidePointer( );
	}

	{	// -- Event Listeners

		EventManager::GetInstance( )->AddEventListener( INPUT_MOUSE_PRESSED, this, &OgreRenderer::OnMousePressed );
		EventManager::GetInstance( )->AddEventListener( INPUT_MOUSE_MOVED, this, &OgreRenderer::OnMouseMoved );
		EventManager::GetInstance( )->AddEventListener( INPUT_MOUSE_RELEASED, this, &OgreRenderer::OnMouseReleased );
		EventManager::GetInstance( )->AddEventListener( INPUT_KEY_DOWN, this, &OgreRenderer::OnKeyDown );
		EventManager::GetInstance( )->AddEventListener( INPUT_KEY_UP, this, &OgreRenderer::OnKeyUp );
		EventManager::GetInstance( )->AddEventListener( VIEW_CHANGE_SCREEN, this, &OgreRenderer::OnChangeScreen );
		EventManager::GetInstance( )->AddEventListener( GAME_LEVEL_CHANGED, this, &OgreRenderer::OnLevelChanged );
	}

	_scene = new OgreMax::OgreMaxScene( );

	_rootUIController = ScriptManager::GetInstance( )->CreateScript( "/data/gui/RootUIController.lua" );

	module( _rootUIController->GetState( ) )
	[
		class_< Gui >( "Gui" ),

		class_< RootUIController >( "RootUIController" )
		.def( constructor< luabind::object, Gui* >( ) )
			.def( "loadComponent", &RootUIController::LoadComponent )
			.def( "destroyAllComponents", &RootUIController::DestroyAllComponents )
			.def( "update", &RootUIController::Update )
	];

	_rootUIController->Initialize( );
	_rootUIController->CallFunction( "onLoad", _gui );

	_isInitialized = true;
}

size_t OgreRenderer::GetHwnd( ) const
{
	if ( !_isInitialized )
	{
		UnInitializedException e( "OgreRenderer::GetHwnd - Renderer isn't initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	size_t hWnd = 0;
	_root->getAutoCreatedWindow( )->getCustomAttribute( "WINDOW", &hWnd );
	return hWnd;
}

void OgreRenderer::Render( ) const
{
	if ( !_isInitialized )
	{
		UnInitializedException e( "OgreRenderer::Render - Renderer isn't initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	_root->renderOneFrame( );
}

void OgreRenderer::Update( const float deltaMilliseconds ) const
{
	if ( !_isInitialized )
	{
		UnInitializedException e( "OgreRenderer::Update - Renderer isn't initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	luabind::call_function< void >( _rootUIController->GetState( ), "onUpdate", deltaMilliseconds );
	_scene->Update( deltaMilliseconds );

	if ( _root->getAutoCreatedWindow( )->isClosed( ) )
	{
		EventManager::GetInstance( )->QueueEvent( new Event( GAME_QUIT ) );
	}
}

void OgreRenderer::LoadResources( )
{
	ConfigFile cf;
	cf.load( Paths::GetConfigPath( ) + "/resources.cfg" );

	ConfigFile::SectionIterator seci = cf.getSectionIterator( );

	while ( seci.hasMoreElements( ) )
	{
		std::string sectionName = seci.peekNextKey( );
		ConfigFile::SettingsMultiMap *settings = seci.getNext( );

		for ( ConfigFile::SettingsMultiMap::iterator i = settings->begin( ); i != settings->end( ); ++i )
		{
			/* So basically the deal here is the whole game runs on a file system that adds bad files together
			   to make one giant directory tree. Ogre however runs on the premise that the bad files create their
			   own individual directory tree for each seperate bad file. This hack fixes that problem by allowing only one 
			   stub bad file to be created within ogre, but it registers each bad file with the Game Filesystem allowing
			   the stub to still access the combined bad files as one giant filesystem */

			int result = FileManager::GetInstance( )->MountFileStore( i->second, "data/" );

			if ( !result )
			{
				ArchiveNotFoundException e( "BadArchive, There was an error adding a BAD file location to the FileManager" );
				Logger::GetInstance( )->Fatal( e.what( ) );
				throw e;
			}

			if ( !_badStubCreated )
			{
				ResourceGroupManager::getSingleton( ).addResourceLocation( i->second, i->first );
				_badStubCreated = true;
			}
		}
	}
}

void OgreRenderer::OnMouseMoved( const IEvent* event )
{
	MouseEventData* eventData = static_cast< MouseEventData* >( event->GetEventData( ) );
	_gui->injectMouseMove( eventData->GetMouseState( ).X.abs, eventData->GetMouseState( ).Y.abs, eventData->GetMouseState( ).Z.abs );
}

void OgreRenderer::OnMousePressed( const IEvent* event )
{
	MouseEventData* eventData = static_cast< MouseEventData* >( event->GetEventData( ) );
	_gui->injectMousePress( eventData->GetMouseState( ).X.abs, eventData->GetMouseState( ).Y.abs, ( MyGUI::MouseButton ) eventData->GetMouseButtonId( ) );
}

void OgreRenderer::OnMouseReleased( const IEvent* event )
{
	MouseEventData* eventData = static_cast< MouseEventData* >( event->GetEventData( ) );
	_gui->injectMouseRelease( eventData->GetMouseState( ).X.abs, eventData->GetMouseState( ).Y.abs, ( MyGUI::MouseButton ) eventData->GetMouseButtonId( ) );
}

void OgreRenderer::OnKeyUp( const IEvent* event )
{
	KeyEventData* eventData = static_cast< KeyEventData* >( event->GetEventData( ) );
	_gui->injectKeyRelease( ( MyGUI::KeyCode ) eventData->GetKeyCode( ) );
}

void OgreRenderer::OnKeyDown( const IEvent* event )
{ 
	KeyEventData* eventData = static_cast< KeyEventData* >( event->GetEventData( ) );
	_gui->injectKeyPress( ( MyGUI::KeyCode ) eventData->GetKeyCode( ) );
}

void OgreRenderer::OnLevelChanged( const IEvent* event )
{

}

void OgreRenderer::OnChangeScreen( const IEvent* event )
{
	_scene->Destroy( );
	_root->getSceneManager( "default" )->clearScene( );

	ChangeScreenEventData* eventData = static_cast< ChangeScreenEventData* >( event->GetEventData( ) );

	std::stringstream sceneFilePath;
	sceneFilePath << "data/gui/views/" << eventData->GetScreenName( ) << "/" << eventData->GetScreenName( ) << ".scene";

	_scene->Load( sceneFilePath.str( ), _root->getAutoCreatedWindow( ), 0x4000, _root->getSceneManager( "default" ) );
	
	std::string mainCameraName = "camera_main";

	if ( _scene->GetSceneManager( )->hasCamera( mainCameraName ) )
	{
		Camera* camera = _scene->GetSceneManager( )->getCamera( mainCameraName );
		_root->getAutoCreatedWindow( )->removeAllViewports( );
		_root->getAutoCreatedWindow( )->addViewport( camera );
	}
}