#ifndef __GUIVIEW_H
#define __GUIVIEW_H

#include <string>

#include "IGuiView.hpp"

#include <MyGUI.h>

#include "../../Scripting/Script.h"

#include "../../Events/EventType.hpp"
#include <luabind/luabind.hpp>
using namespace luabind;

class GuiView : public IGuiView
{

	typedef std::map< std::string, luabind::object > EventHandlerList;
	typedef std::map< MyGUI::WidgetPtr, EventHandlerList > EventListenerList;

public:

	virtual ~GuiView( ) { };

	GuiView( std::string name )
		: _name( name )
	{
		
	};

	virtual void Initialize( );
	MyGUI::WidgetPtr FindControl( std::string name );
	
	void AddEventListener( MyGUI::WidgetPtr widget, std::string widgetType, std::string eventName, luabind::object handlerFunction )
	{
		if ( widgetType == "Window" )
		{
			MyGUI::Window* window = static_cast< MyGUI::Window* >( widget );

			if( eventName == "WindowButtonPressed" )
			{
				window->eventWindowButtonPressed = MyGUI::newDelegate( this, &GuiView::WindowButtonPressed );	
			}

			if( eventName == "KeyButtonReleased" )
			{
				window->eventKeyButtonReleased = MyGUI::newDelegate( this, &GuiView::KeyButtonReleased );
			}

			_eventListeners[ widget ][ eventName ] = handlerFunction;	
		}
	};
	
	void WindowButtonPressed( MyGUI::WidgetPtr widget, const std::string& name )
	{
		luabind::call_function< void >( _eventListeners[ widget ][ "WindowButtonPressed" ], this, name );
	};

	void KeyButtonReleased( MyGUI::WidgetPtr widget, MyGUI::KeyCode keyCode )
	{
		luabind::call_function< void >( _eventListeners[ widget ][ "KeyButtonReleased" ], this, keyCode );
	};

private:

	std::string _name;
	EventListenerList _eventListeners;

};

#endif