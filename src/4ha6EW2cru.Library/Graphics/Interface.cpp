#include "Interface.h"

#include "../Scripting/ScriptManager.h"
#include "../Events/EventManager.h"

#include "MyGUI_Any.h"

void Interface::Initialize( )
{
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