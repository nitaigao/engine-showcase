#ifndef __GUIVIEW_H
#define __GUIVIEW_H

#include <string>

#include "IGuiView.hpp"
#include "GuiEvents.hpp"

#include <MyGUI.h>

#include "../../Scripting/Script.h"

#include "../../Events/EventType.hpp"
#include <luabind/luabind.hpp>
using namespace luabind;

class GuiView : public IGuiView
{

	typedef std::map< GuiEvents, std::string > EventHandlerList;
	typedef std::map< MyGUI::WidgetPtr, EventHandlerList > EventListenerList;

public:

	virtual ~GuiView( ) { };

	GuiView( luabind::object luaObject, std::string name )
		: _name( name )
		, _luaObject( luaObject )
	{
		
	};

	virtual void Initialize( );
	MyGUI::WidgetPtr FindControl( std::string name );

	void AddEventListener( MyGUI::WidgetPtr widget, GuiEvents eventType, std::string handlerFunction );
	void WindowButtonPressed( MyGUI::WidgetPtr widget, const std::string& name );
	void MouseButtonReleased( MyGUI::WidgetPtr widget );
	void KeyButtonReleased( MyGUI::WidgetPtr widget, MyGUI::KeyCode keyCode );

private:

	std::string _name;
	luabind::object _luaObject;
	EventListenerList _eventListeners;

};

#endif