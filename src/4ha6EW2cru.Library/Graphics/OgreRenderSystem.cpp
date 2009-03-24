#include "OgreRenderSystem.h"

#include "../System/Management.h"
#include "../Events/Event.h"

#include "OgreSystemScene.h"

#include "../Logging/Logger.h"

#include "../Exceptions/NullReferenceException.hpp"
#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/ArchiveNotFoundException.hpp"

#include "Interface.h"

#include "../IO/FileManager.h"
#include "../IO/BadArchiveFactory.h"

#include "../Maths/MathQuaternion.hpp"

#include <complex>

OgreRenderSystem::OgreRenderSystem( Configuration* configuration )
{
	FileManager* fileManager = new FileManager( );
	fileManager->Initialize( );
	this->Constructor( configuration, fileManager );
}

OgreRenderSystem::OgreRenderSystem( Configuration* configuration, IFileManager* fileManager )
{
	this->Constructor( configuration, fileManager );
}

void OgreRenderSystem::Constructor( Configuration* configuration, IFileManager* fileManager )
{
	_configuration = configuration;
	_badStubCreated = false;
	_isIntialized = false;
	_root = 0;
	_window = 0;
	_interface = 0;
	_badFactory = 0;
	_fileManager = fileManager;
}

OgreRenderSystem::~OgreRenderSystem( )
{
	Management::GetInstance( )->GetEventManager( )->RemoveEventListener( GRAPHICS_SETTINGS_CHANGED, this, &OgreRenderSystem::OnGraphicsSettingsUpdated );
	
	Ogre::WindowEventUtilities::removeWindowEventListener( _window, this );

	delete this->GetProperties( )[ "availableVideoModes" ].GetValue< std::vector< std::string >* >( );

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

	_configuration->SetDefault( "Graphics", "fullscreen", false );
	_configuration->SetDefault( "Graphics", "width", 640 );
	_configuration->SetDefault( "Graphics", "height", 480 );
	_configuration->SetDefault( "Graphics", "depth", 32 );
	_configuration->SetDefault( "Graphics", "window_title", std::string( "Interactive View" ) );

	_root = new Root( );

	// Switches off the Ogre Logging unless the whole game is in debug mode
	if ( Logger::GetInstance( )->GetLogLevel( ) < WARNA )
	{
		Ogre::LogManager::getSingletonPtr( )->destroyLog( Ogre::LogManager::getSingletonPtr( )->getDefaultLog( ) );
		Ogre::LogManager::getSingletonPtr( )->createLog( "default", true, false, true );
	}

#ifdef _DEBUG
	_root->loadPlugin( "RenderSystem_Direct3D9_d" );
#else
	_root->loadPlugin( "RenderSystem_Direct3D9" );
#endif // _DEBUG	

	_badFactory = new BadArchiveFactory( );
	ArchiveManager::getSingletonPtr( )->addArchiveFactory( _badFactory );
	this->LoadResources( );

	ResourceGroupManager::getSingleton( ).initialiseAllResourceGroups( );

	RenderSystemList *renderSystems = _root->getAvailableRenderers( );
	RenderSystemList::iterator renderSystemIterator = renderSystems->begin( );

	_root->setRenderSystem( *renderSystemIterator );

	std::stringstream videoModeDesc;
	videoModeDesc << _configuration->Find< int >( "Graphics", "width" ) << " x " << _configuration->Find< int >( "Graphics", "height" ) << " @ 32-bit colour";
	( *renderSystemIterator )->setConfigOption( "Video Mode", videoModeDesc.str( ) );
	( *renderSystemIterator )->setConfigOption( "Full Screen", _configuration->Find< bool >( "Graphics", "fullscreen" ) ? "Yes" : "No" );

	_root->initialise( false );

	Management::GetInstance( )->GetPlatformManager( )->CreateInteractiveWindow( 
		_configuration->Find< std::string >( "Graphics", "window_title" ), 
		_configuration->Find< int >( "Graphics", "width" ), 
		_configuration->Find< int >( "Graphics", "height" ),
		_configuration->Find< bool >( "Graphics", "fullscreen" )
		);

	NameValuePairList params;
	params[ "externalWindowHandle" ] = StringConverter::toString( ( int ) Management::GetInstance( )->GetPlatformManager( )->GetHwnd( ) );

	_window = _root->createRenderWindow( 
		_configuration->Find< std::string >( "Graphics", "window_title" ), 
		_configuration->Find< int >( "Graphics", "width" ), 
		_configuration->Find< int >( "Graphics", "height" ),
		_configuration->Find< bool >( "Graphics", "fullscreen" ), 
		&params 
		);

	Ogre::WindowEventUtilities::addWindowEventListener( _window, this );

	_properties[ "availableVideoModes" ] = SystemProperty( "availableVideoModes", new std::vector< std::string >( this->GetVideoModes( ) ) ); 

	SceneManager* sceneManager = _root->createSceneManager( ST_GENERIC, "default" );

	Camera* camera = sceneManager->createCamera( "default" );
	camera->setPosition( Vector3( 0, 0, 0 ) );
	camera->lookAt( Vector3( 0, 0, 0 ) );
	camera->setNearClipDistance( 1.0f );

	Viewport* viewPort = _window->addViewport( camera );
	viewPort->setBackgroundColour( ColourValue( 1, 1, 1 ) );

	camera->setAspectRatio(
		Real( viewPort->getActualWidth( )) / Real( viewPort->getActualHeight( ) )
		);

	_root->renderOneFrame( );

	_interface = new Interface( _configuration, _root );
	_interface->Initialize( );

	Management::GetInstance( )->GetEventManager( )->AddEventListener( GRAPHICS_SETTINGS_CHANGED, this, &OgreRenderSystem::OnGraphicsSettingsUpdated );

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
}

ISystemScene* OgreRenderSystem::CreateScene()
{
	return new OgreSystemScene( this );
}

void OgreRenderSystem::windowClosed( RenderWindow* rw )
{
	Management::GetInstance( )->GetEventManager( )->QueueEvent( new Event( GAME_QUIT ) );
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

std::vector< std::string > OgreRenderSystem::GetVideoModes( ) const
{
	std::vector< std::string > availableDisplayModes;
	ConfigOptionMap options = _root->getRenderSystem( )->getConfigOptions( );
	for( ConfigOptionMap::iterator cm = options.begin( ); cm != options.end( ); ++cm )
	{
		if ( ( *cm ).first == "Video Mode" )
		{
			StringVector possibleModes = ( *cm ).second.possibleValues;

			for( StringVector::iterator i = possibleModes.begin( ); i != possibleModes.end( ); ++i )
			{
				std::stringstream currentColorDepth;
				currentColorDepth << _window->getColourDepth( );

				int result = ( *i ).find( currentColorDepth.str( ) );

				if ( result > -1 )
				{
					std::string mode = ( *i ).substr( 0, ( *i ).find( " @ " ) );
					availableDisplayModes.push_back( mode );
				}
			}
		}
	}

	return availableDisplayModes;
}

void OgreRenderSystem::OnGraphicsSettingsUpdated( const IEvent* event )
{
	Ogre::RenderWindow* window = static_cast< Ogre::RenderWindow* >( _root->getRenderTarget(  _configuration->Find< std::string >( "Graphics", "window_title" ) ) );
	
	window->setFullscreen(
		_configuration->Find< bool >( "Graphics", "fullscreen" ),
		_configuration->Find< int >( "Graphics", "width" ), 
		_configuration->Find< int >( "Graphics", "height" )
		);
}