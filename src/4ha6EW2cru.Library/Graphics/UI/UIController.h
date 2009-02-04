#ifndef __UICONTROLLER_H
#define __UICONTROLLER_H

#include <MyGUI.h>
using namespace MyGUI;

#include "luabind/luabind.hpp"
using namespace luabind;

#include "../../Events/IEvent.hpp"

class UIController
{

	typedef std::map< const std::string, object* > WidgetUserData;
	typedef std::vector< WidgetPtr > WidgetList;

public:

	UIController( object luaObject, std::string name, Gui* gui );

	~UIController( );

	void EventHandler( const IEvent* event );
	void SetWidgetScript( WidgetPtr widget, const std::string eventName, object function );

private:

	void OnMouseReleased( MyGUI::WidgetPtr widget, int left, int top, MouseButton buttonId );

	std::string _name;
	Gui* _gui;
	object _luaObject;
	WidgetList _widgetList;

};


#endif