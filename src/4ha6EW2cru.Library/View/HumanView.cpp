#include "HumanView.h"

#include "../Events/EventListener.h"
#include "../Logging/Logger.h"
#include "../Common/Paths.hpp"
#include "../IO/FileManager.h"
#include "../Graphics/OgreRenderer.h"

#include "../Exceptions/UnInitializedException.hpp"

void HumanView::Initialize( )
{
	Logger::GetInstance( )->Info( "Initializing Human View" );

	_renderer = new OgreRenderer( );
	_renderer->Initialize( _configuration->GetDisplayWidth( ), _configuration->GetDisplayHeight( ), _configuration->GetColorDepth( ), _configuration->IsFullScreen( ) );

	_configuration->SetAvailableVideoModes( _renderer->GetVideoModes( ) );

	_inputSystem = new InputSystem( _renderer->GetHwnd( ) );
	_inputSystem->Initialize( );
	_inputSystem->SetCaptureArea( _configuration->GetDisplayWidth( ), _configuration->GetDisplayHeight( ) );

	EventManager::GetInstance( )->AddEventListener( GRAPHICS_SETTINGS_CHANGED, this, &HumanView::OnViewSettingsChanged );

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

	if ( _inputSystem != 0 )
	{
		delete _inputSystem;
		_inputSystem = 0;
	}
}

void HumanView::OnViewSettingsChanged( const IEvent* event )
{
	_renderer->ChangeResolution( _configuration->GetDisplayWidth( ), _configuration->GetDisplayHeight( ), _configuration->IsFullScreen( ) );
	_inputSystem->SetCaptureArea( _configuration->GetDisplayWidth( ), _configuration->GetDisplayHeight( ) );
}
