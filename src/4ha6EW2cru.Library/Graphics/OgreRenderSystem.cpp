#include "OgreRenderSystem.h"

#include "../System/Management.h"
#include "../Events/Event.h"

#include "../Logging/Logger.h"

#include "../Exceptions/NullReferenceException.hpp"
#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/ArchiveNotFoundException.hpp"

#include "Interface.h"

#include "../IO/FileManager.h"
#include "../IO/BadArchiveFactory.h"

OgreRenderSystem::OgreRenderSystem( IConfiguration* configuration )
{
	FileManager* fileManager = new FileManager( );
	fileManager->Initialize( );
	this->Constructor( configuration, fileManager );
}

OgreRenderSystem::OgreRenderSystem( IConfiguration* configuration, IFileManager* fileManager )
{
	this->Constructor( configuration, fileManager );
}

OgreRenderSystem::~OgreRenderSystem( )
{
	if( _interface != 0 )
	{
		delete _interface;
	}

	if ( _root != 0 )
	{
		_root->shutdown( );
		delete _root;
		_root = 0;
	}

	if( _badFactory != 0 )
	{
		delete _badFactory;
		_badFactory = 0;
	}

	_fileManager->Release( );
	delete _fileManager;
	_fileManager = 0;
}

void OgreRenderSystem::Initialize( )
{
	if ( _configuration == 0 )
	{
		NullReferenceException e( "RenderSystem::Initialize - The Configuration is NULL" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
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
	videoModeDesc << _configuration->Find( "Graphics", "Width", 640 ) << " x " << _configuration->Find( "Graphics", "Height", 480 ) << " @ 32-bit colour";

	( *renderSystemIterator )->setConfigOption( "Full Screen", _configuration->Find( "Graphics", "FullScreen", false ) ? "Yes" : "No" );
	( *renderSystemIterator )->setConfigOption( "Video Mode", videoModeDesc.str( ) );

	_root->initialise( true, "Interactive View" );
	
	size_t hWnd = 0;
	_root->getAutoCreatedWindow( )->getCustomAttribute( "WINDOW", &hWnd );
	
	Property hWndProperty;
	hWndProperty.SetValue( hWnd );
	_properties[ "hwnd" ] = hWndProperty;

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

	_interface = new Interface( _root );
	_interface->Initialize( );

	_isIntialized = true;
}

void OgreRenderSystem::Update( float deltaMilliseconds )
{
	if ( !_isIntialized )
	{
		UnInitializedException e( "RenderSystem::Update - Attempted to update when View isn't initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	_interface->Update( deltaMilliseconds );
	_root->renderOneFrame( );

	if ( _root->getAutoCreatedWindow( )->isClosed( ) )
	{
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new Event( GAME_QUIT ) );
	}
}

void OgreRenderSystem::LoadResources( )
{
	ConfigFile cf;
	cf.load( "../game/config/resources.cfg" );

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

			int result = _fileManager->MountFileStore( i->second, "data/" );

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