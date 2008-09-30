#include "HumanView.h"

#include "../Events/EventListener.h"
#include "../Logging/Logger.h"
#include "../Common/Paths.hpp"
#include "../IO/FileManager.h"
#include "../Graphics/OgreRenderer.h"

#include "../Exceptions/UnInitializedException.hpp"

void HumanView::Initialize( int width, int height, bool fullScreen )
{
	Logger::GetInstance( )->Info( "Initializing Human View" );

	_renderer = new OgreRenderer( );
	_renderer->Initialize( width, height, fullScreen );

	{	// -- Input

		_inputSystem = new InputSystem( _renderer->GetHwnd( ) );
		_inputSystem->Initialize( );
		_inputSystem->SetCaptureArea( width, height );
	}

	{	// -- View Script Initialization

		FileManager::GetInstance( )->AddFileStore( Paths::GetGUIPackagePath( ) );
		FileBuffer* viewScriptBuffer = FileManager::GetInstance( )->GetFile( "views/interactiveview.lua" );

		_viewScript = Script::CreateFromFileBuffer( viewScriptBuffer );
		
		module( _viewScript->GetState( ) )
		[
			class_< HumanView >( "HumanView" ),
			class_< Screen >( "Screen" )
				.def( constructor< std::string, unsigned int > ( ) )
				.def( "Initialize", &Screen::Initialize )
			,

			def( "changeScreen", &HumanView::FromLua_ChangeScreen )
		];

		_viewScript->Initialize( );
	}

	{	// -- Event Listeners

		EventManager::GetInstance( )->AddEventListener( GAME_INITIALIZED, this, &HumanView::OnGameInitialized );
		EventManager::GetInstance( )->AddEventListener( VIEW_CHANGE_SCREEN, this, &HumanView::OnChangeScreen );
	}

	_isIntialized = true;
}

void HumanView::Render( )
{
	if ( !_isIntialized )
	{
		UnInitializedException e( "HumanView::Render - Attempted to render when View isn't initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	_renderer->Render( );
}

void HumanView::Update( )
{
	if ( !_isIntialized )
	{
		UnInitializedException e( "HumanView::Update - Attempted to update when View isn't initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}
	
	_inputSystem->Update( );
	_renderer->Update( );
}

HumanView::~HumanView( )
{
	if ( _renderer != 0 )
	{
		delete _renderer;
		_renderer = 0;
	}

	if ( _viewScript != 0 )
	{
		delete _viewScript;
		_viewScript = 0;
	}

	if ( _currentScreen != 0 )
	{
		delete _currentScreen;
		_currentScreen = 0;
	}

	if ( _inputSystem != 0 )
	{
		delete _inputSystem;
		_inputSystem = 0;
	}

	if ( _isIntialized )
	{
		EventManager::GetInstance( )->RemoveEventListener( GAME_INITIALIZED, this, &HumanView::OnGameInitialized );
		EventManager::GetInstance( )->RemoveEventListener( VIEW_CHANGE_SCREEN, this, &HumanView::OnChangeScreen );		
	}
}

void HumanView::OnGameInitialized( const IEvent* event )
{
	luabind::call_function< int >( _viewScript->GetState( ), "onGameInitialized", this );
}

void HumanView::OnChangeScreen( const IEvent* event )
{
	ChangeScreenEventData* eventData = static_cast< ChangeScreenEventData* > ( event->GetEventData( ) );
	luabind::call_function< int >( _viewScript->GetState( ), "onScreenChange", this, eventData->GetScreenName( ) );
}

void HumanView::FromLua_ChangeScreen( HumanView* view, std::string screenName, unsigned int visibilityMask )
{
	if ( view->_currentScreen != 0 )
	{
		delete view->_currentScreen;
	}

	std::stringstream scriptPath;
	scriptPath << "gui/screens/" << screenName << "/" << screenName << ".lua";

	FileBuffer* scriptBuffer = FileManager::GetInstance( )->GetFile( scriptPath.str( ) );
	Script* script = Script::CreateFromFileBuffer( scriptBuffer );
	script->Initialize( );

	view->_currentScreen = new Screen( screenName, visibilityMask );
	view->_currentScreen->Initialize( view->_renderer->GetGui( ), script );
}