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
	typedef std::map< EventType, std::string > EventHandlerList;

public:

	GuiModel( ) { };

	GuiModel( luabind::object luaObject )
		: _luaObject( luaObject )
	{

	};

	virtual ~GuiModel( );

	void AddEventListener( EventType eventType, std::string handlerFunction );
	void EventHandler( const IEvent* event );

	static void FromLua_ExecuteCommand( GuiModel* model, std::string command );

private:

	EventHandlerList _eventHandlers;
	luabind::object _luaObject;

};


#endif