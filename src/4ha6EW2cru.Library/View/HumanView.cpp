#include "HumanView.h"

#include "../Events/EventListener.h"
#include "../Logging/Logger.h"
#include "../Common/Paths.hpp"
#include "../IO/FileManager.h"
#include "../Graphics/OgreRenderer.h"

bool HumanView::Initialize( )
{
	Logger::GetInstance( )->Info( "Initializing Human View" );

	int width = 1920;
	int height = 1200;
	bool fullScreen = false;

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
		_viewScript->Initialize( );
		
		module( _viewScript->GetState( ) )
		[
			class_< HumanView >( "HumanView" ),
			class_< Screen >( "Screen" )
				.def( constructor< std::string, unsigned int > ( ) )
				.def( "Initialize", &Screen::Initialize )
			,

			def( "changeScreen", &HumanView::FromLua_ChangeScreen )
		];
	}

	{	// -- Event Listeners

		EventManager::GetInstance( )->AddEventListener( GAME_INITIALIZED, this, &HumanView::OnGameInitialized );
		EventManager::GetInstance( )->AddEventListener( CHANGE_SCREEN, this, &HumanView::OnChangeScreen );
	}

	return true;
}

void HumanView::Render( )
{
	_renderer->Render( );
}

void HumanView::Update( )
{
	if ( _inputSystem != 0 )
	{
		_inputSystem->Update( );
	}
}

HumanView::~HumanView( )
{
	EventManager::GetInstance( )->RemoveEventListener( GAME_INITIALIZED, this, &HumanView::OnGameInitialized );
	EventManager::GetInstance( )->RemoveEventListener( CHANGE_SCREEN, this, &HumanView::OnChangeScreen );

	delete _currentScreen;
	delete _inputSystem;
	_inputSystem = 0;

	if ( _viewScript != 0 )
	{
		delete _viewScript;
		_viewScript = 0;
	}

	delete _renderer;
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

void HumanView::FromLua_ChangeScreen( HumanView* sender, std::string screenName, unsigned int visibilityMask )
{
	if ( sender->_currentScreen != 0 )
	{
		delete sender->_currentScreen;
	}

	std::stringstream scriptPath;
	scriptPath << "gui/screens/" << screenName << "/" << screenName << ".lua";

	FileBuffer* scriptBuffer = FileManager::GetInstance( )->GetFile( scriptPath.str( ) );
	Script* script = Script::CreateFromFileBuffer( scriptBuffer );
	script->Initialize( );

	sender->_currentScreen = new Screen( screenName, visibilityMask );
	sender->_currentScreen->Initialize( sender->_renderer->GetGui( ), script );
}