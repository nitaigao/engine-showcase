#ifndef __CONTROL_H
#define __CONTROL_H

#include <string>

#include <MyGUI.h>

#include "IGuiController.hpp"

#include "../../Events/EventType.hpp"
#include <luabind/luabind.hpp>
using namespace luabind;

class GuiController : public IGuiController
{

	typedef std::pair< object*, GuiController* > WidgetUserData;
	typedef std::vector< WidgetUserData* > WidgetUserDataList;

public:

	GuiController( const std::string name )
		: _name( name )
	{

	};

	~GuiController( );

	void AddEventListener( MyGUI::WidgetPtr target, EventType eventType, object handlerFunction );
	static void FromLua_AddEventListener( MyGUI::WidgetPtr target, GuiController* view, EventType eventType, object handlerFunction );
	static void ToLua_OnKeyReleased( MyGUI::WidgetPtr sender, MyGUI::KeyCode key );

private:

	std::string _name;
	WidgetUserDataList _activeUserData;

};

#endif