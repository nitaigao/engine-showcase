#include "RendererSystem.h"

#include "../System/Management.h"
#include "../Events/Event.h"

#include "RendererSystemScene.h"
#include "Color.hpp"

#include "../Logging/Logger.h"

#include "../Exceptions/NullReferenceException.hpp"
#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/ArchiveNotFoundException.hpp"

#include "Interface.h"

#include "../IO/FileManager.h"
#include "../IO/BadArchiveFactory.h"

#include "../Maths/MathQuaternion.hpp"

#include <complex>

RendererSystem::~RendererSystem( )
{
	Management::GetInstance( )->GetEventManager( )->RemoveEventListener( GRAPHICS_SETTINGS_CHANGED, this, &RendererSystem::OnGraphicsSettingsUpdated );
	
	Ogre::WindowEventUtilities::removeWindowEventListener( _window, this );

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
}

void RendererSystem::Initialize( )
{
	if ( _configuration == 0 )
	{
		NullReferenceException e( "RenderSystem::Initialize - The Configuration is NULL" );
		Logger::GetInstance( )->Fatal( e.what( ) );
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

	_badFactory = new BadArchiveFactory( );
	ArchiveManager::getSingletonPtr( )->addArchiveFactory( _badFactory );
	this->LoadResources( );

	ResourceGroupManager::getSingleton( ).initialiseAllResourceGroups( );

	RenderSystemList *renderSystems = _root->getAvailableRenderers( );
	RenderSystemList::iterator renderSystemIterator = renderSystems->begin( );

	_root->setRenderSystem( *renderSystemIterator );

	std::stringstream videoModeDesc;
	videoModeDesc << _configuration->Find< int >( _configSectionName, "width" ) << " x " << _configuration->Find< int >( _configSectionName, "height" ) << " @ " << defaultDepth << "-bit colour";
	( *renderSystemIterator )->setConfigOption( "Video Mode", videoModeDesc.str( ) );
	( *renderSystemIterator )->setConfigOption( "Full Screen", _configuration->Find< bool >( _configSectionName, "fullscreen" ) ? "Yes" : "No" );
	( *renderSystemIterator )->setConfigOption( "VSync", _configuration->Find< bool >( _configSectionName, "vsync" ) ? "Yes" : "No" );

	_root->initialise( false );

	try
	{
		this->CreateRenderWindow(
			_configuration->Find< std::string >( _configSectionName, "window_title" ), 
			_configuration->Find< int >( _configSectionName, "width" ), 
			_configuration->Find< int >( _configSectionName, "height" ),
			_configuration->Find< bool >( _configSectionName, "fullscreen" )
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

	_interface = new Interface( _configuration, _window );
	_interface->Initialize( );

	Management::GetInstance( )->GetEventManager( )->AddEventListener( GRAPHICS_SETTINGS_CHANGED, this, &RendererSystem::OnGraphicsSettingsUpdated );
	Management::GetInstance( )->GetServiceManager( )->RegisterService( this );

	_isIntialized = true;
}

void RendererSystem::Update( float deltaMilliseconds )
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
		ColourValue colorValue( color.GetRed( ), color.GetGreen( ), color.GetBlue( ) );

		_sceneManager->setAmbientLight( colorValue );
	}

	if ( name == "backgroundColor" )
	{
		Color color = value.GetValue< Color >( );
		ColourValue colorValue( color.GetRed( ), color.GetGreen( ), color.GetBlue( ) );

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

void RendererSystem::OnGraphicsSettingsUpdated( const IEvent* event )
{
	Ogre::RenderWindow* window = static_cast< Ogre::RenderWindow* >( _root->getRenderTarget(  _configuration->Find< std::string >( _configSectionName, "window_title" ) ) );

	int width = _configuration->Find< int >( _configSectionName, "width" );
	int height = _configuration->Find< int >( _configSectionName, "height" );
	bool fullScreen = _configuration->Find< bool >( _configSectionName, "fullscreen" );

	window->setFullscreen( fullScreen, width, height );
	_interface->ResetWidgetPositions( );
}

void RendererSystem::CreateRenderWindow( const std::string& windowTitle, int width, int height, bool fullScreen )
{
	Management::GetInstance( )->GetPlatformManager( )->CreateInteractiveWindow( windowTitle, width, height, fullScreen );
	
	NameValuePairList params;
	params[ "externalWindowHandle" ] = StringConverter::toString( ( int ) Management::GetInstance( )->GetPlatformManager( )->GetHwnd( ) );
	params[ "vsync" ] = _configuration->Find< bool >( _configSectionName, "vsync" ) ? "true" : "false";

	_window = _root->createRenderWindow( windowTitle, width, height, fullScreen, &params ); 
}

AnyValue::AnyValueMap RendererSystem::Execute( const std::string& actionName, AnyValueMap parameters )
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

	return results;
}