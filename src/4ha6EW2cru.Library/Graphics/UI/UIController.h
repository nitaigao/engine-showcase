#ifndef __UICONTROLLER_H
#define __UICONTROLLER_H

#include <MyGUI.h>
using namespace MyGUI;

#include "luabind/luabind.hpp"
using namespace luabind;

#include "../../Events/IEvent.hpp"

class UIController
{

public:

	UIController( object luaObject, std::string name, Gui* gui );

	~UIController( ) { };

	void EventHandler( const IEvent* event );

private:

	std::string _name;
	Gui* _gui;
	object _luaObject;

};


#endif