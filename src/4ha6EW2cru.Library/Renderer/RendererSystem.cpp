#include "RendererSystem.h"

#include "RendererSystemScene.h"
#include "Color.hpp"
using namespace Ogre;

#include "../Management/Management.h"
#include "../Events/Event.h"
using namespace Events;

#include "../Logging/Logger.h"
using namespace Logging;

#include "../IO/BadArchiveFactory.h"
using namespace IO;

namespace Renderer
{

	RendererSystem::~RendererSystem( )
	{	
		Ogre::WindowEventUtilities::removeWindowEventListener( _window, this );

		if ( _root != 0 )
		{
			_root->shutdown( );
			delete _root;
			_root = 0;
		}
	}

	void RendererSystem::Initialize( )
	{
		if ( _configuration == 0 )
		{
			NullReferenceException e( "RenderSystem::Initialize - The Configuration is NULL" );
			Logger::Fatal( e.what( ) );
			throw e;
		}

		bool defaultFullScreen = false;
		int defaultWidth = 640;
		int defaultHeight = 480;
		int defaultDepth = 32;
		bool defaultVsync = true;
		std::string defaultWindowTitle = "Interactive View";

		_configuration->SetDefault( _configSectionName, "fullscreen", defaultFullScreen );
		_configuration->SetDefault( _configSectionName, "width", defaultWidth );
		_configuration->SetDefault( _configSectionName, "height", defaultHeight );
		_configuration->SetDefault( _configSectionName, "depth", defaultDepth );
		_configuration->SetDefault( _configSectionName, "window_title", defaultWindowTitle );
		_configuration->SetDefault( _configSectionName, "vsync", defaultVsync );

		_root = new Root( );

	#ifdef _DEBUG
		_root->loadPlugin( "RenderSystem_Direct3D9_d" );
	#else
		_root->loadPlugin( "RenderSystem_Direct3D9" );

		Ogre::LogManager::getSingletonPtr( )->destroyLog( Ogre::LogManager::getSingletonPtr( )->getDefaultLog( ) );
		Ogre::LogManager::getSingletonPtr( )->createLog( "default", true, false, true );
	#endif // _DEBUG	

		//_badFactory = new BadArchiveFactory( );
		ArchiveManager::getSingletonPtr( )->addArchiveFactory( new BadArchiveFactory( ) );
		this->LoadResources( );

		ResourceGroupManager::getSingleton( ).initialiseAllResourceGroups( );

		RenderSystemList *renderSystems = _root->getAvailableRenderers( );
		RenderSystemList::iterator renderSystemIterator = renderSystems->begin( );

		_root->setRenderSystem( *renderSystemIterator );

		std::stringstream videoModeDesc;
		videoModeDesc << _configuration->Find( _configSectionName, "width" ).GetValue< int >( ) << " x " << _configuration->Find( _configSectionName, "height" ).GetValue< int >( ) << " @ " << defaultDepth << "-bit colour";
		( *renderSystemIterator )->setConfigOption( "Video Mode", videoModeDesc.str( ) );
		( *renderSystemIterator )->setConfigOption( "Full Screen", _configuration->Find( _configSectionName, "fullscreen" ).GetValue< bool >( ) ? "Yes" : "No" );
		( *renderSystemIterator )->setConfigOption( "VSync", _configuration->Find( _configSectionName, "vsync" ).GetValue< bool >( ) ? "Yes" : "No" );

		_root->initialise( false );

		try
		{
			this->CreateRenderWindow(
				_configuration->Find( _configSectionName, "window_title" ).GetValue< std::string >( ), 
				_configuration->Find( _configSectionName, "width" ).GetValue< int >( ), 
				_configuration->Find( _configSectionName, "height" ).GetValue< int >( ),
				_configuration->Find( _configSectionName, "fullscreen" ).GetValue< bool >( )
				);
		}
		catch( Exception e )
		{
			if ( e.getNumber( ) == Exception::ERR_RENDERINGAPI_ERROR )
			{
				_configuration->Set( _configSectionName, "fullscreen", defaultFullScreen );
				_configuration->Set( _configSectionName, "width", defaultWidth );
				_configuration->Set( _configSectionName, "height", defaultHeight );
				_configuration->Set( _configSectionName, "depth", defaultDepth );
				_configuration->Set( _configSectionName, "window_title", defaultWindowTitle );

				Management::GetInstance( )->GetPlatformManager( )->CloseWindow( );
			}
		}

		Ogre::WindowEventUtilities::addWindowEventListener( _window, this );

		_properties[ "availableVideoModes" ] = this->GetVideoModes( );

		_sceneManager = _root->createSceneManager( ST_GENERIC, "default" );

		//_sceneManager->setFog(FOG_LINEAR, ColourValue( ), 0.0, 50, 100);

		Camera* camera = _sceneManager->createCamera( "default" );
		camera->setPosition( Vector3( 0, 0, 0 ) );
		camera->lookAt( Vector3( 0, 0, 0 ) );
		camera->setNearClipDistance( 1.0f );

		Viewport* viewPort = _window->addViewport( camera );
		viewPort->setBackgroundColour( ColourValue( 0, 0, 0 ) );

		camera->setAspectRatio(
			Real( viewPort->getActualWidth( )) / Real( viewPort->getActualHeight( ) )
			);

		_root->renderOneFrame( );

		Management::GetInstance( )->GetServiceManager( )->RegisterService( this );
	}

	void RendererSystem::Update( const float& deltaMilliseconds )
	{
		_root->renderOneFrame( );
	}

	void RendererSystem::SetProperty( const std::string& name, AnyValue value )
	{
		_properties[ name ] = value;

		if ( name == "activeCamera" )
		{
			std::string cameraName = value.GetValue< std::string >( );
			Camera* camera = _sceneManager->getCamera( cameraName );
			_sceneManager->getCurrentViewport( )->setCamera( camera );
		}

		if ( name == "ambientColor" )
		{
			Color color = value.GetValue< Color >( );
			ColourValue colorValue( color.Red, color.Green, color.Blue );

			_sceneManager->setAmbientLight( colorValue );
		}

		if ( name == "backgroundColor" )
		{
			Color color = value.GetValue< Color >( );
			ColourValue colorValue( color.Red, color.Green, color.Blue );

			_sceneManager->getCurrentViewport( )->setBackgroundColour( colorValue );
		}

		if ( name == "farClip" )
		{
			float farClip = value.GetValue< float >( );

			if ( _sceneManager->isSkyBoxEnabled( ) )
			{
				SceneManager::SkyBoxGenParameters skyBoxParameters = _sceneManager->getSkyBoxGenParameters( ); 
				_sceneManager->setSkyBox( true, _skyBoxMaterial, farClip - 200.0f );
			}

			_sceneManager->getCurrentViewport( )->getCamera( )->setFarClipDistance( farClip );
		}

		if ( name == "skyBox" )
		{
			std::string materialName = value.GetValue< std::string >( );

			if ( materialName.empty( ) )
			{
				_sceneManager->setSkyBox( false, "" );
			}
			else
			{
				_skyBoxMaterial = materialName;
				SceneManager::SkyBoxGenParameters skyBoxParameters = _sceneManager->getSkyBoxGenParameters( ); 
				float currentFarClip = _sceneManager->getCurrentViewport( )->getCamera( )->getFarClipDistance( );
				_sceneManager->setSkyBox( true, _skyBoxMaterial, currentFarClip - 200.0f );
			}
		}
	}

	ISystemScene* RendererSystem::CreateScene( )
	{
		_scene = new RendererSystemScene( _sceneManager );
		return _scene;
	}

	void RendererSystem::windowClosed( RenderWindow* rw )
	{
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new Event( GAME_QUIT ) );
	}

	void RendererSystem::LoadResources( )
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
				ResourceGroupManager::getSingleton( ).addResourceLocation( i->second, "BAD" );
			}
		}
	}

	std::vector< std::string > RendererSystem::GetVideoModes( ) const
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

		

	void RendererSystem::CreateRenderWindow( const std::string& windowTitle, int width, int height, bool fullScreen )
	{
		Management::GetInstance( )->GetPlatformManager( )->CreateInteractiveWindow( windowTitle, width, height, fullScreen );
		
		NameValuePairList params;
		params[ "externalWindowHandle" ] = StringConverter::toString( ( int ) Management::GetInstance( )->GetPlatformManager( )->GetWindowId( ) );
		params[ "vsync" ] = _configuration->Find( _configSectionName, "vsync" ).GetValue< bool >( ) ? "true" : "false";

		_window = _root->createRenderWindow( windowTitle, width, height, fullScreen, &params ); 
	}

	AnyValue::AnyValueMap RendererSystem::Execute( const std::string& actionName, AnyValue::AnyValueMap& parameters )
	{
		AnyValue::AnyValueMap results;

		if ( actionName == "playAnimation" )
		{
			IRendererSystemComponent* component = _scene->GetComponent( parameters[ "entityName" ].GetValue< std::string >( ) );
			
			component->PlayAnimation( 
				parameters[ "animationName" ].GetValue< std::string >( ),
				parameters[ "loopAnimation" ].GetValue< bool >( )
				);
		}

		if ( actionName == "getRenderWindow" )
		{
			results[ "renderWindow" ] = _window;
		}

		if( actionName == "getAvailableVideoModes" )
		{
			results[ "availableVideoModes" ] = this->GetVideoModes( );
		}

		if ( actionName == "changeResolution" )
		{
			//int width = _configuration->Find( _configSectionName, "width" ).GetValue< int >( );
			//int height = _configuration->Find( _configSectionName, "height" ).GetValue< int >( );
			//bool fullScreen = _configuration->Find( _configSectionName, "fullscreen" ).GetValue< bool >( );

			//window->setFullscreen( fullScreen, width, height );
			//_interface->ResetWidgetPositions( );

			_window->setFullscreen(  
				parameters[ "fullScreen" ].GetValue< bool >( ),
				parameters[ "width" ].GetValue< int >( ),
				parameters[ "height" ].GetValue< int >( )
				);
		}

		return results;
	}
}