#include "Interface.h"

#include <luabind/table_policy.hpp>

#include "../Events/Event.h"
#include "../Events/EventManager.h"

#include "MyGUI_Any.h"

#include "../System/Management.h"
#include "../Scripting/ScriptSystemScene.h"
#include "../Scripting/ScriptSystem.h"
#include "../Scripting/ScriptComponent.h"
#include "../Scripting/ScriptEvent.hpp"

static ScriptSystemScene* g_InterfaceScriptScene = 0;

Interface::~Interface( )
{
	Management::GetInstance( )->GetEventManager( )->RemoveEventListener( INPUT_MOUSE_PRESSED, this, &Interface::OnMousePressed );
	Management::GetInstance( )->GetEventManager( )->RemoveEventListener( INPUT_MOUSE_MOVED, this, &Interface::OnMouseMoved );
	Management::GetInstance( )->GetEventManager( )->RemoveEventListener( INPUT_MOUSE_RELEASED, this, &Interface::OnMouseReleased );
	Management::GetInstance( )->GetEventManager( )->RemoveEventListener( INPUT_KEY_DOWN, this, &Interface::OnKeyDown );
	Management::GetInstance( )->GetEventManager( )->RemoveEventListener( INPUT_KEY_UP, this, &Interface::OnKeyUp );

	if ( _gui != 0 )
	{
		_gui->shutdown( );
		delete _gui;
		_gui = 0;
	}

	if ( g_InterfaceScriptScene != 0 )
	{
		delete g_InterfaceScriptScene;
		g_InterfaceScriptScene = 0;
	}
}

void Interface::Initialize( )
{
	g_InterfaceScriptScene = ( ScriptSystemScene* ) Management::GetInstance( )->GetSystemManager( )->GetSystem( ScriptSystemType )->CreateScene( );
	g_InterfaceScriptScene->Initialize( );

	_gui->initialise( _renderWindow, "/data/interface/core/core.xml" );

	Management::GetInstance( )->GetEventManager( )->AddEventListener( INPUT_MOUSE_PRESSED, this, &Interface::OnMousePressed );
	Management::GetInstance( )->GetEventManager( )->AddEventListener( INPUT_MOUSE_MOVED, this, &Interface::OnMouseMoved );
	Management::GetInstance( )->GetEventManager( )->AddEventListener( INPUT_MOUSE_RELEASED, this, &Interface::OnMouseReleased );
	Management::GetInstance( )->GetEventManager( )->AddEventListener( INPUT_KEY_DOWN, this, &Interface::OnKeyDown );
	Management::GetInstance( )->GetEventManager( )->AddEventListener( INPUT_KEY_UP, this, &Interface::OnKeyUp );

	ScriptComponent* scriptComponent = ( ScriptComponent* ) g_InterfaceScriptScene->CreateComponent( "interface", "default" );

	AnyValueMap properties;
	std::string scriptPath = "/data/interface/interface.lua";
	properties.insert( std::make_pair( "scriptPath", scriptPath ) );
	scriptComponent->Initialize( properties );

	module( scriptComponent->GetState( ) )
	[
		class_< Widget >( "Widget" )
			.def( "getDimensions", &Widget::getClientCoord )
			.def( "setSize", ( void( Widget::* )( int, int ) ) &Widget::setSize )
			.def( "setPosition", ( void( Widget::* )( int, int ) ) &Widget::setPosition )
			.def( "getType", &Widget::getClassTypeName )
			.def( "setVisible", &Widget::setVisible )
			.def( "isVisible", &Widget::isVisible )
			.def( "getText", &Interface::GetText )
			.def( "setText", &Interface::SetText )
			.def( "asButton", &Interface::AsButton )
			.def( "asComboBox", &Interface::AsComboBox )
			.def( "asEditBox", &Interface::AsEditBox )
			.def( "asProgressBar", &Interface::AsProgressBar )
			.def( "setFocus", &Interface::SetFocus ),

		class_< Button >( "Button" )
			.def( "setChecked", &Button::setStateCheck )
			.def( "getChecked", &Button::getStateCheck ),

		class_< ComboBox, Widget >( "ComboBox" )
			.def( "addItem", ( void ( ComboBox::* ) ( const std::string&, const std::string& ) ) &ComboBox::addItem )
			.def( "getValueAt", ( const std::string& ( ComboBox::* ) ( int ) ) &ComboBox::getItemNameAt )
			.def( "getSelectedIndex", &ComboBox::getIndexSelected )
			.def( "setSelectedIndex", &ComboBox::setIndexSelected ),

		class_< Edit, Widget >( "EditBox" )
			.def( "addText", &Interface::AddText ),

		class_< Progress, Widget >( "ProgressBar" )
			.def( "setProgress", &Progress::setProgressPosition ),

		class_< IntCoord >( "IntCoord" )
			.def_readonly( "x" , &IntCoord::left )
			.def_readonly( "y" , &IntCoord::top )
			.def_readonly( "width" , &IntCoord::width )
			.def_readonly( "height" , &IntCoord::height ),

		class_< Any >( "Any" )
			.def( constructor<>( ) )
	];

	Interface::RegisterGlobals( scriptComponent->GetState( ) );

	lua_pcall( scriptComponent->GetState( ), 0, 0, 0 );

	WidgetManager::getInstancePtr( )->registerUnlinker( this );
}

void Interface::Update( const float deltaMilliseconds )
{
	_gui->injectFrameEntered( deltaMilliseconds );

	if ( _updateSkipCount++ >= 100 )
	{
		Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "UPDATE_TICK" ) ); 
	}
}

void Interface::LoadComponent( const std::string componentName )
{
	std::stringstream layoutPath;
	layoutPath << "/data/interface/components/" << componentName << ".layout";

	Gui::getInstancePtr( )->load( layoutPath.str( ) );

	std::stringstream scriptPath;
	scriptPath << "/data/interface/components/" << componentName << ".lua";

	ScriptComponent* scriptComponent = ( ScriptComponent* ) g_InterfaceScriptScene->CreateComponent( componentName, "default" );

	AnyValueMap properties;
	properties.insert( std::make_pair( "scriptPath", scriptPath.str( ) ) );
	scriptComponent->Initialize( properties );

	Interface::RegisterGlobals( scriptComponent->GetState( ) );

	lua_pcall( scriptComponent->GetState( ), 0, 0, 0 );

	Gui::getInstancePtr( )->windowResized( Gui::getInstancePtr( )->getRenderWindow( ) );
}

WidgetPtr Interface::FindWidget( const std::string widgetName )
{
	return Gui::getInstancePtr( )->findWidgetT( widgetName );
}

void Interface::_unlinkWidget ( WidgetPtr widget )
{
	void* userData = widget->getUserData( );
	WidgetUserData* widgetUserData = static_cast< WidgetUserData* >( userData );
	
	if ( 0 != widgetUserData )
	{
		for ( WidgetUserData::iterator i = widgetUserData->begin( ); i != widgetUserData->end( ); ++i )
		{
			delete ( *i ).second;
		}

		delete widgetUserData;
	}
}

void Interface::ScriptWidget( Widget* widget, const std::string eventName, object function )
{
	void* userData = widget->getUserData( );

	WidgetUserData* widgetUserData = static_cast< WidgetUserData* >( userData );

	if ( widgetUserData == 0 )
	{
		widgetUserData = new WidgetUserData( );
	}

	object* handlerFunctionPtr = new object( function );
	widgetUserData->insert( std::make_pair( eventName, handlerFunctionPtr ) );

	widget->setUserData( static_cast< void* >( widgetUserData ) );

	if ( eventName == "onRelease" )
	{
		widget->eventMouseButtonReleased = newDelegate( &Interface::OnMouseReleased );
	}

	if ( eventName == "onKeyUp" )
	{
		widget->eventKeyButtonReleased = newDelegate( &Interface::OnKeyUp );
	}
}

void Interface::OnMouseReleased( WidgetPtr widget, int left, int top, MouseButton id )
{
	void* userData = widget->getUserData( );
	WidgetUserData* widgetUserData = static_cast< WidgetUserData* >( userData );

	for ( WidgetUserData::iterator i = widgetUserData->begin( ); i != widgetUserData->end( ); ++i )
	{
		if ( ( *i ).first == "onRelease" )
		{
			object eventHandler = *( *i ).second;
			eventHandler( left, top );
		}
	}
}

void Interface::OnKeyUp( WidgetPtr widget, KeyCode key )
{
	void* userData = widget->getUserData( );
	WidgetUserData* widgetUserData = static_cast< WidgetUserData* >( userData );

	for ( WidgetUserData::iterator i = widgetUserData->begin( ); i != widgetUserData->end( ); ++i )
	{
		if ( ( *i ).first == "onKeyUp" )
		{
			Char keyCode = InputManager::getInstancePtr( )->getKeyChar( key, 0 );
			char* keyText = ( char* ) &keyCode;

			object eventHandler = *( *i ).second;
			eventHandler( keyCode, std::string( keyText ) );
		}
	}
}

void Interface::OnMouseMoved( const IEvent* event )
{
	MouseEventData* eventData = static_cast< MouseEventData* >( event->GetEventData( ) );
	_gui->injectMouseMove( eventData->GetMouseState( ).X.abs, eventData->GetMouseState( ).Y.abs, eventData->GetMouseState( ).Z.abs );
}

void Interface::OnMousePressed( const IEvent* event )
{
	MouseEventData* eventData = static_cast< MouseEventData* >( event->GetEventData( ) );
	_gui->injectMousePress( eventData->GetMouseState( ).X.abs, eventData->GetMouseState( ).Y.abs, MouseButton::Enum( eventData->GetMouseButtonId( ) ) );
}

void Interface::OnMouseReleased( const IEvent* event )
{
	MouseEventData* eventData = static_cast< MouseEventData* >( event->GetEventData( ) );
	_gui->injectMouseRelease( eventData->GetMouseState( ).X.abs, eventData->GetMouseState( ).Y.abs, MouseButton::Enum( eventData->GetMouseButtonId( ) ) );
}

void Interface::OnKeyUp( const IEvent* event )
{
	KeyEventData* eventData = static_cast< KeyEventData* >( event->GetEventData( ) );
	_gui->injectKeyRelease( KeyCode::Enum( eventData->GetKeyCode( ) ) );
}

void Interface::OnKeyDown( const IEvent* event )
{
	KeyEventData* eventData = static_cast< KeyEventData* >( event->GetEventData( ) );
	_gui->injectKeyPress( KeyCode::Enum( eventData->GetKeyCode( ) ) );
}

void Interface::RegisterEvent( EventType eventType, object function )
{
	g_InterfaceScriptScene->RegisterEvent( eventType, function );
}

void Interface::UnRegisterEvent( EventType eventType, object function )
{
	g_InterfaceScriptScene->UnRegisterEvent( eventType, function );
}

void Interface::BroadcastEvent( const std::string& eventName )
{
	IEvent* event = new ScriptEvent( eventName ) ;
	Management::GetInstance( )->GetEventManager( )->QueueEvent( event );
}

void Interface::ExecuteString( const std::string& input )
{
	g_InterfaceScriptScene->ExecuteString( input );
}

void Interface::RegisterEventHandler( object function )
{
	g_InterfaceScriptScene->RegisterEvent( ALL_EVENTS, function );
}

void Interface::UnRegisterEventHandler( object function )
{
	g_InterfaceScriptScene->UnRegisterEvent( ALL_EVENTS, function );
}

void Interface::SetInputAllowed( bool inputAllowed )
{
	ISystem* inputSystem = Management::GetInstance( )->GetSystemManager( )->GetSystem( InputSystemType );

	if ( inputSystem != 0 )
	{
		inputSystem->SetProperty( "inputAllowed", inputAllowed );
	}
}

void Interface::SetFocus( WidgetPtr widget, bool focus )
{
	if ( focus )
	{
		MyGUI::InputManager::getInstancePtr( )->setKeyFocusWidget( widget );
	}
	else
	{
		MyGUI::InputManager::getInstancePtr( )->resetKeyFocusWidget( widget );
	}
}

void Interface::ChangeResolution( int width, int height, bool isFullScreen )
{
	Management::GetInstance( )->GetEventManager( )->QueueEvent( new Event( GRAPHICS_SETTINGS_CHANGED ) );
	Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "GRAPHICS_SETTINGS_CHANGED", width, height ) );
}

void Interface::SetFarClip( const float& farClip )
{
	ISystem* renderSystem = Management::GetInstance( )->GetSystemManager( )->GetSystem( RenderSystemType );
	renderSystem->SetProperty( "farClip", farClip );
}

std::vector< std::string > Interface::GetSupportedResolutions()
{
	ISystem* renderSystem = Management::GetInstance( )->GetSystemManager( )->GetSystem( RenderSystemType );
	std::vector< std::string > resolutions = renderSystem->GetProperties( )[ "availableVideoModes" ].GetValue< std::vector< std::string > >( );

	std::multimap< int, std::string > resolutionWidths;

	for( std::vector< std::string >::iterator i = resolutions.begin( ); i != resolutions.end( ); ++i )
	{
		std::string resolution = ( *i );

		std::stringstream resolutionStream;
		resolutionStream << resolution.substr( 0, resolution.find( ' x ' ) );

		int resolutionWidth = 0;
		resolutionStream >> resolutionWidth;
		resolutionWidths.insert( std::make_pair( resolutionWidth, resolution ) );
	}

	resolutions.clear( );

	for( std::multimap< int, std::string >::iterator i = resolutionWidths.begin( ); i != resolutionWidths.end( ); ++i )
	{
		resolutions.push_back( ( *i ).second );
	}

	return resolutions;
}

void Interface::ResetWidgetPositions(  )
{
	_gui->windowResized( _renderWindow );
}

void Interface::RegisterGlobals( lua_State* luaState )
{
	module( luaState )
	[
		def( "findWidget", &Interface::FindWidget ),
		def( "loadComponent", &Interface::LoadComponent ),
		def( "getScreenWidth", &Interface::GetScreenWidth ),
		def( "getScreenHeight", &Interface::GetScreenHeight ),
		def( "getSupportedResolutions", &Interface::GetSupportedResolutions, copy_table( result ) ),
		def( "scriptWidget", &Interface::ScriptWidget ),
		def( "executeString", &Interface::ExecuteString ),
		def( "showMouse", &Interface::ShowMouse ),
		def( "hideMouse", &Interface::HideMouse ),
		def( "setInputAllowed", &Interface::SetInputAllowed ),
		def( "changeResolution", &Interface::ChangeResolution ),
		def( "setFarClip", &Interface::SetFarClip ),
		def( "getFps", &Interface::GetFps ),

		def( "registerEventHandler", &Interface::RegisterEventHandler ),
		def( "unregisterEventHandler", &Interface::UnRegisterEventHandler ),
		def( "registerEvent", &Interface::RegisterEvent ),
		def( "unregisterEvent", &Interface::UnRegisterEvent ),
		def( "broadcastEvent", &Interface::BroadcastEvent )
	];
}