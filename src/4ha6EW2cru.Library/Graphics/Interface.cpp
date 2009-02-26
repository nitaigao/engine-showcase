#include "Interface.h"

#include "../Scripting/ScriptManager.h"
#include "../Events/EventManager.h"

#include "MyGUI_Any.h"

Interface::~Interface( )
{
	EventManager::GetInstance( )->RemoveEventListener( INPUT_MOUSE_PRESSED, this, &Interface::OnMousePressed );
	EventManager::GetInstance( )->RemoveEventListener( INPUT_MOUSE_MOVED, this, &Interface::OnMouseMoved );
	EventManager::GetInstance( )->RemoveEventListener( INPUT_MOUSE_RELEASED, this, &Interface::OnMouseReleased );
	EventManager::GetInstance( )->RemoveEventListener( INPUT_KEY_DOWN, this, &Interface::OnKeyDown );
	EventManager::GetInstance( )->RemoveEventListener( INPUT_KEY_UP, this, &Interface::OnKeyUp );

	if ( _gui != 0 )
	{
		_gui->shutdown( );
		delete _gui;
		_gui = 0;
	}
}

void Interface::Initialize( )
{
	_gui->initialise( _ogreRoot->getAutoCreatedWindow( ), "/data/interface/core/core.xml" );

	EventManager::GetInstance( )->AddEventListener( INPUT_MOUSE_PRESSED, this, &Interface::OnMousePressed );
	EventManager::GetInstance( )->AddEventListener( INPUT_MOUSE_MOVED, this, &Interface::OnMouseMoved );
	EventManager::GetInstance( )->AddEventListener( INPUT_MOUSE_RELEASED, this, &Interface::OnMouseReleased );
	EventManager::GetInstance( )->AddEventListener( INPUT_KEY_DOWN, this, &Interface::OnKeyDown );
	EventManager::GetInstance( )->AddEventListener( INPUT_KEY_UP, this, &Interface::OnKeyUp );

	lua_State* luaState = ScriptManager::GetInstance( )->LoadScript( "/data/interface/interface.lua" );

	module( luaState )
	[
		def( "findWidget", &Interface::FindWidget ),
		def( "loadComponent", &Interface::LoadComponent ),
		def( "getScreenWidth", &Interface::GetScreenWidth ),
		def( "getScreenHeight", &Interface::GetScreenHeight ),
		def( "scriptWidget", &Interface::ScriptWidget ),
		def( "showMouse", &Interface::ShowMouse ),
		def( "hideMouse", &Interface::HideMouse ),

		class_< Widget >( "Widget" )
			.def( "getDimensions", &MyGUI::Widget::getClientCoord )
			.def( "setPosition", ( void( MyGUI::Widget::* )( int, int ) ) &MyGUI::Widget::setPosition )
			.def( "getType", &MyGUI::Widget::getClassTypeName )
			.def( "setVisible", &MyGUI::Widget::setVisible )
			.def( "asButton", &Interface::AsButton )
			.def( "asComboBox", &Interface::AsComboBox ),

		class_< Button >( "Button" )
			.def( "setChecked", &Button::setStateCheck )
			.def( "getChecked", &Button::getStateCheck ),

		class_< ComboBox, Widget >( "ComboBox" )
			.def( "addItem", ( void ( ComboBox::* ) ( const std::string&, const std::string& ) ) &ComboBox::addItem )
			.def( "getValueAt", ( const std::string& ( ComboBox::* ) ( int ) ) &ComboBox::getItemNameAt )
			.def( "getSelectedIndex", &ComboBox::getIndexSelected )
			.def( "setSelectedIndex", &ComboBox::setIndexSelected ),

		class_< IntCoord >( "IntCoord" )
			.def_readonly( "x" , &MyGUI::IntCoord::left )
			.def_readonly( "y" , &MyGUI::IntCoord::top )
			.def_readonly( "width" , &MyGUI::IntCoord::width )
			.def_readonly( "height" , &MyGUI::IntCoord::height ),

		class_< Ogre::UTFString >( "utf" )
			.def( constructor< std::string >( ) ),

		class_< Any >( "Any" )
			.def( constructor<>( ) )
	];

	lua_pcall( luaState, 0, 0, 0 );

	MyGUI::WidgetManager::getInstancePtr( )->registerUnlinker( this );
}

void Interface::Update( const float deltaMilliseconds ) const
{
	_gui->injectFrameEntered( deltaMilliseconds );
}

void Interface::LoadComponent( const std::string componentName )
{
	std::stringstream layoutPath;
	layoutPath << "/data/interface/components/" << componentName << ".layout";

	MyGUI::LayoutManager::getInstance().load( layoutPath.str( ) );

	std::stringstream scriptPath;
	scriptPath << "/data/interface/components/" << componentName << ".lua";

	lua_State* luaState = ScriptManager::GetInstance( )->LoadScript( scriptPath.str( ) );
	lua_pcall( luaState, 0, 0, 0 );
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

void Interface::OnMouseMoved( const IEvent* event )
{
	MouseEventData* eventData = static_cast< MouseEventData* >( event->GetEventData( ) );
	_gui->injectMouseMove( eventData->GetMouseState( ).X.abs, eventData->GetMouseState( ).Y.abs, eventData->GetMouseState( ).Z.abs );
}

void Interface::OnMousePressed( const IEvent* event )
{
	MouseEventData* eventData = static_cast< MouseEventData* >( event->GetEventData( ) );
	_gui->injectMousePress( eventData->GetMouseState( ).X.abs, eventData->GetMouseState( ).Y.abs, MyGUI::MouseButton::Enum( eventData->GetMouseButtonId( ) ) );
}

void Interface::OnMouseReleased( const IEvent* event )
{
	MouseEventData* eventData = static_cast< MouseEventData* >( event->GetEventData( ) );
	_gui->injectMouseRelease( eventData->GetMouseState( ).X.abs, eventData->GetMouseState( ).Y.abs, MyGUI::MouseButton::Enum( eventData->GetMouseButtonId( ) ) );
}

void Interface::OnKeyUp( const IEvent* event )
{
	KeyEventData* eventData = static_cast< KeyEventData* >( event->GetEventData( ) );
	_gui->injectKeyRelease( MyGUI::KeyCode::Enum( eventData->GetKeyCode( ) ) );
}

void Interface::OnKeyDown( const IEvent* event )
{
	KeyEventData* eventData = static_cast< KeyEventData* >( event->GetEventData( ) );
	_gui->injectKeyPress( MyGUI::KeyCode::Enum( eventData->GetKeyCode( ) ) );
}
