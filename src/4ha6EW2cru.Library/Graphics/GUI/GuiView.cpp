#include "GUIView.h"

#include "../../Common/Paths.hpp"
#include "../../io/FileManager.h"
#include "../../Exceptions/FileNotFoundException.hpp"

#include "../../Logging/Logger.h"

void GuiView::Initialize( )
{
	std::stringstream layoutPath;
	layoutPath << Paths::GetViewsPath( ) << _name << "/" << _name << ".layout";

	if ( !FileManager::GetInstance( )->FileExists( layoutPath.str( ) ) )
	{
		FileNotFoundException e( "GuiView::Initialize - Layout file not found" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	MyGUI::Gui::getInstancePtr( )->load( layoutPath.str( ) );
}

MyGUI::WidgetPtr GuiView::FindControl( std::string name )
{
	return MyGUI::Gui::getInstancePtr( )->findWidgetT( name );
}

void GuiView::AddEventListener( MyGUI::WidgetPtr widget, GuiEvents eventType, std::string handlerFunction )
{
	MyGUI::Window* window = 0;

	switch( eventType )
	{

	case WINDOW_BUTTON_PRESSED:

		window = static_cast< MyGUI::Window* >( widget );
		window->eventWindowButtonPressed = MyGUI::newDelegate( this, &GuiView::WindowButtonPressed );

		break;

	case KEY_BUTTON_RELEASED:

		window = static_cast< MyGUI::Window* >( widget );
		window->eventKeyButtonReleased = MyGUI::newDelegate( this, &GuiView::KeyButtonReleased );

		break;

		_eventListeners[ widget ][ eventType ] = handlerFunction;

	case MOUSE_BUTTON_RELEASED:

		widget->eventMouseButtonClick = MyGUI::newDelegate( this, &GuiView::MouseButtonReleased );

		break;
	}

	_eventListeners[ widget ][ eventType ] = handlerFunction;
}

void GuiView::WindowButtonPressed( MyGUI::WidgetPtr widget, const std::string& name )
{
	luabind::call_member< void >( _luaObject, _eventListeners[ widget ][ WINDOW_BUTTON_PRESSED ].c_str( ), name );
}

void GuiView::KeyButtonReleased( MyGUI::WidgetPtr widget, MyGUI::KeyCode keyCode )
{
	luabind::call_member< void >( _luaObject, _eventListeners[ widget ][ KEY_BUTTON_RELEASED ].c_str( ), keyCode );
}

void GuiView::MouseButtonReleased( MyGUI::WidgetPtr widget )
{
	luabind::call_member< void >( _luaObject, _eventListeners[ widget ][ MOUSE_BUTTON_RELEASED ].c_str( ) );
}