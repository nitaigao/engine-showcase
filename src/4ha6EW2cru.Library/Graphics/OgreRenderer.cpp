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

	delete _scene;
	delete _badFactory;
	_badFactory = 0;
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

	{	// -- Interface
		_interface = new Interface( _root );
		_interface->Initialize( );
	}

	_scene = new OgreMax::OgreMaxScene( );

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

std::vector< std::string > OgreRenderer::GetVideoModes( ) const
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
				currentColorDepth << _root->getAutoCreatedWindow( )->getColourDepth( );

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

	_scene->Update( deltaMilliseconds );
	_interface->Update( deltaMilliseconds );

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

void OgreRenderer::ChangeResolution( int width, int height, bool fullscreen )
{
	_root->getAutoCreatedWindow( )->setFullscreen( fullscreen, width, height );
	_root->getAutoCreatedWindow( )->resize( width, height );
}