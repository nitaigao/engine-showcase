#include "OgreRenderer.h"

#include "../Common/Paths.hpp"
#include "../Logging/Logger.h"
#include "../IO/BadArchiveFactory.h"
#include "../Events/EventManager.h"
#include "../Events/Event.h"

OgreRenderer::~OgreRenderer( )
{
	EventManager::GetInstance( )->RemoveEventListener( INPUT_MOUSE_MOVED, this, &OgreRenderer::OnMouseMoved );
	EventManager::GetInstance( )->RemoveEventListener( INPUT_MOUSE_MOVED, this, &OgreRenderer::OnMouseMoved );
	EventManager::GetInstance( )->RemoveEventListener( INPUT_MOUSE_RELEASED, this, &OgreRenderer::OnMouseReleased );

	_gui->shutdown( );
	delete _gui;
	_gui = 0;

	_root->shutdown( );
	delete _root;
	_root = 0;

	delete _badFactory;
	_badFactory = 0;
}

bool OgreRenderer::Initialize( int width, int height, bool fullScreen )
{
	{	// -- Ogre Init

		_root = new Root( );

		Ogre::LogManager::getSingletonPtr( )->destroyLog( Ogre::LogManager::getSingletonPtr( )->getDefaultLog( ) );
		Ogre::LogManager::getSingletonPtr( )->createLog( "default", true, false, true );

		_root->loadPlugin( "RenderSystem_Direct3D9_d" );

		_badFactory = new BadArchiveFactory( );
		ArchiveManager::getSingletonPtr( )->addArchiveFactory( _badFactory );
		this->LoadResources( );

		ResourceGroupManager::getSingleton( ).initialiseAllResourceGroups( );

		RenderSystemList *renderSystems = _root->getAvailableRenderers( );
		RenderSystemList::iterator renderSystemIterator = renderSystems->begin( );

		_root->setRenderSystem( *renderSystemIterator );

		HDC deviceContext = 0;

		if( !GetDC( NULL ) )
		{
			Logger::GetInstance( )->Fatal( "Could not get Device Information" );
			return false;
		}

		int desktopWidth = GetSystemMetrics( SM_CXSCREEN );
		int desktopHeight = GetSystemMetrics( SM_CYSCREEN );

		std::stringstream videoModeDesc;
		videoModeDesc << desktopWidth << " x " << desktopHeight << " @ 32-bit colour";

		( *renderSystemIterator )->setConfigOption( "Full Screen", "No" );
		( *renderSystemIterator )->setConfigOption( "Video Mode", videoModeDesc.str( ) );

		_root->initialise( true, "Human View" );

		SceneManager* sceneManager = _root->createSceneManager( ST_GENERIC, "default" );

		Camera* camera = sceneManager->createCamera( "default camera" );
		camera->setPosition( Vector3( 0, 20, 100 ) );
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
		_gui->initialise( _root->getAutoCreatedWindow(  ) );
		_gui->hidePointer( );
	}

	{	// -- Event Listeners

		EventManager::GetInstance( )->AddEventListener( INPUT_MOUSE_MOVED, this, &OgreRenderer::OnMouseMoved );
		EventManager::GetInstance( )->AddEventListener( INPUT_MOUSE_PRESSED, this, &OgreRenderer::OnMousePressed );
		EventManager::GetInstance( )->AddEventListener( INPUT_MOUSE_RELEASED, this, &OgreRenderer::OnMouseReleased );
	}

	return true;
}

size_t OgreRenderer::GetHwnd( ) const
{
	size_t hWnd = 0;
	_root->getAutoCreatedWindow( )->getCustomAttribute( "WINDOW", &hWnd );
	return hWnd;
}

void OgreRenderer::Render( ) const
{
	_root->renderOneFrame( );
}

void OgreRenderer::Update( ) const
{
	if ( _root != 0 )
	{
		if ( _root->getAutoCreatedWindow( )->isClosed( ) )
		{
			EventManager::GetInstance( )->QueueEvent( new Event( GAME_QUIT ) );
		}
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
			//ResourceGroupManager::getSingleton( ).addResourceLocation( i->second, i->first );
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