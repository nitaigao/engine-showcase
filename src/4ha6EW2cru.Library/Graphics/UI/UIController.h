#ifndef __UICONTROLLER_H
#define __UICONTROLLER_H

#include <MyGUI.h>
using namespace MyGUI;

#include "luabind/luabind.hpp"
using namespace luabind;

#include "../../Events/IEvent.hpp"

class UIController
{

	typedef std::map< const std::string, object* > EventHandlers;
	typedef std::pair< EventHandlers*, UIController* > WidgetUserData;
	typedef std::vector< WidgetUserData* > WidgetUserDataList;

public:

	UIController( object luaObject, std::string name, Gui* gui );

	~UIController( ) { };

	void EventHandler( const IEvent* event );
	void SetWidgetScript( WidgetPtr widget, const std::string eventName, object function );

private:

	std::string _name;
	Gui* _gui;
	object _luaObject;

};


#endif