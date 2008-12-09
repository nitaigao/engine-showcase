#ifndef __GUIMODEL_H
#define __GUIMODEL_H

#include "IGuiModel.hpp"

#include <MyGUI.h>
#include "../../Scripting/Script.h"
#include "../../Events/EventType.hpp"

#include <luabind/luabind.hpp>
using namespace luabind;

class GuiModel : public IGuiModel
{

	typedef std::map< EventType, object > EventHandlers;

public:

	virtual ~GuiModel( );

	static void FromLua_AddEventListener( GuiModel* eventTarget, EventType eventType, luabind::object handlerFunction );
	void AddEventListener( EventType eventType, luabind::object handlerFunction );

	void EventHandler( const IEvent* event );

	static void FromLua_ExecuteCommand( GuiModel* model, std::string command );

private:

	EventHandlers _eventHandlers;

};


#endif