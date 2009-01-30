#include "HumanView.h"

#include "../Events/EventListener.h"
#include "../Logging/Logger.h"
#include "../Common/Paths.hpp"
#include "../IO/FileManager.h"
#include "../Graphics/OgreRenderer.h"

#include "../Exceptions/UnInitializedException.hpp"

void HumanView::Initialize( int width, int height, int colorDepth, bool fullScreen )
{
	Logger::GetInstance( )->Info( "Initializing Human View" );

	_renderer = new OgreRenderer( );
	_renderer->Initialize( width, height, colorDepth, fullScreen );

	_inputSystem = new InputSystem( _renderer->GetHwnd( ) );
	_inputSystem->Initialize( );
	_inputSystem->SetCaptureArea( width, height );

	EventManager::GetInstance( )->AddEventListener( GAME_INITIALIZED, this, &HumanView::OnGameInitialized );
	EventManager::GetInstance( )->AddEventListener( VIEW_CHANGE_SCREEN, this, &HumanView::OnChangeScreen );

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

void HumanView::Update( const float deltaMilliseconds )
{
	if ( !_isIntialized )
	{
		UnInitializedException e( "HumanView::Update - Attempted to update when View isn't initialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}
	
	_inputSystem->Update( );
	_renderer->Update( deltaMilliseconds );
}

HumanView::~HumanView( )
{
	if ( _renderer != 0 )
	{
		delete _renderer;
		_renderer = 0;
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
	//Event* changeScreenEvent = new Event( VIEW_CHANGE_SCREEN, new ChangeScreenEventData( "introscreen" ) );
	//EventManager::GetInstance( )->QueueEvent( changeScreenEvent );
}

void HumanView::OnChangeScreen( const IEvent* event )
{
	ChangeScreenEventData* eventData = static_cast< ChangeScreenEventData* > ( event->GetEventData( ) );
	this->ChangeScreen( eventData->GetScreenName( ) );
}

void HumanView::ChangeScreen( std::string screenName )
{
	delete _currentScreen;

	_currentScreen = new GuiScreen( screenName );
	_currentScreen->Initialize( );
}