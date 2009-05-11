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

#include "Line3d.h"

#include "../Maths/MathVector3.hpp"
using namespace Maths;

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

		ResourceGroupManager::getSingleton( ).addResourceLocation( "/", "BAD" );
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

		Camera* camera = _sceneManager->createCamera( "default" );
		camera->setPosition( Vector3( 0, 0, 0 ) );
		camera->lookAt( Vector3( 0, 0, 0 ) );
		camera->setNearClipDistance( 1.0f );
		camera->setFarClipDistance( 500.0f );

		Viewport* viewPort = _window->addViewport( camera );
		viewPort->setBackgroundColour( ColourValue( 0, 0, 0 ) );

		camera->setAspectRatio(
			Real( viewPort->getActualWidth( )) / Real( viewPort->getActualHeight( ) )
			);

		_root->renderOneFrame( );

		Management::GetInstance( )->GetServiceManager( )->RegisterService( this );

		//CompositorManager::getSingletonPtr( )->addCompositor( _sceneManager->getCurrentViewport( ), "HDR" );
		//CompositorManager::getSingletonPtr( )->setCompositorEnabled( _sceneManager->getCurrentViewport( ), "HDR", true );
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
			Color color = value.GetValue< Renderer::Color >( );
			ColourValue colorValue( color.Red, color.Green, color.Blue );

			_sceneManager->setAmbientLight( colorValue );
		}

		if ( name == "backgroundColor" )
		{
			Color color = value.GetValue< Renderer::Color >( );
			ColourValue colorValue( color.Red, color.Green, color.Blue );

			_sceneManager->getCurrentViewport( )->setBackgroundColour( colorValue );
		}

		if ( name == "farClip" )
		{
			float farClip = value.GetValue< float >( );

			if ( _sceneManager->isSkyBoxEnabled( ) )
			{
				SceneManager::SkyBoxGenParameters skyBoxParameters = _sceneManager->getSkyBoxGenParameters( ); 
				_sceneManager->setSkyBox( true, _skyBoxMaterial, 350 );
			}

			_sceneManager->getCurrentViewport( )->getCamera( )->setFarClipDistance( farClip );
		}

		if( name == "fog" )
		{
			AnyValue::AnyValueMap parameters = value.GetValue< AnyValue::AnyValueMap >( );
			Color color = parameters[ "color" ].GetValue< Renderer::Color >( );

			_sceneManager->setFog( 
				FOG_LINEAR, 
				ColourValue( color.Red, color.Green, color.Blue ),
				0.001000, 
				parameters[ "linearStart" ].GetValue< float >( ), 
				parameters[ "linearEnd" ].GetValue< float >( ) 
				);				

			/*Ogre::Plane skyPlane;
			skyPlane.d = 50;
			skyPlane.normal = Vector3::NEGATIVE_UNIT_Y;

			_sceneManager->setSkyPlane( true, skyPlane, "24-Default", 8, 4, true, 0, 4, 4 );*/
		}

		if ( name == "skyBox" )
		{
			AnyValue::AnyValueMap parameters = value.GetValue< AnyValue::AnyValueMap >( );

			std::string material = parameters[ "material" ].GetValue< std::string >( );

			if ( material.empty( ) )
			{
				_sceneManager->setSkyBox( false, "" );
			}
			else
			{
				_skyBoxMaterial = material;

				_sceneManager->setSkyBox( 
					true, 
					_skyBoxMaterial, 
					parameters[ "distance" ].GetValue< float >( ) );
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
			_window->setFullscreen(  
				parameters[ "fullScreen" ].GetValue< bool >( ),
				parameters[ "width" ].GetValue< int >( ),
				parameters[ "height" ].GetValue< int >( )
				);
		}

		if ( actionName == "drawLine" )
		{
			Line3D* line = new Line3D( );
			line->drawLine( 
				parameters[ "origin" ].GetValue< MathVector3 >( ).AsOgreVector3( ), 
				parameters[ "destination" ].GetValue< MathVector3 >( ).AsOgreVector3( ) 
				);

			SceneNode* lineNode = _sceneManager->createSceneNode( );
			lineNode->attachObject( line );
			_sceneManager->getRootSceneNode( )->addChild( lineNode );

		}

		return results;
	}
}