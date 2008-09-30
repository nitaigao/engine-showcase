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

	typedef std::vector< object* > HandlerList;

public:

	virtual ~GuiView( );

	GuiView( std::string name )
		: _name( name )
	{
		
	};

	virtual void Initialize( );
	MyGUI::WidgetPtr FindControl( std::string name );
	void AddEventListener( MyGUI::WidgetPtr target, EventType eventType, object handlerFunction );

	static void FromLua_AddEventListener( MyGUI::WidgetPtr target, GuiView* view, EventType eventType, object handlerFunction );
	static void ToLua_OnKeyReleased( MyGUI::WidgetPtr sender, MyGUI::KeyCode key );

private:

	std::string _name;
	HandlerList _handlers;

};

struct GuiView_Wrapper : GuiView, luabind::wrap_base
{
	virtual ~GuiView_Wrapper( ) { };

	GuiView_Wrapper( std::string name )
		: GuiView( name )
	{

	}

};

#endif