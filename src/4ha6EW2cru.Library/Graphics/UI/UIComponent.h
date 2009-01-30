#ifndef __UICOMPONENT_H
#define __UICOMPONENT_H

#include <MyGUI.h>
using namespace MyGUI;

#include <luabind/luabind.hpp>
using namespace luabind;

#include <MyGUI.h>
using namespace MyGUI;

#include "../../Scripting/Script.h"
#include "../../Scripting/ScriptManager.h"

#include "UIController.h"

class UIComponent
{

public:

	UIComponent( std::string name, luabind::object parentController, Gui* gui  )
		: _name( name )
		, _parentController( parentController )
		, _gui( gui )
		, _script( 0 )
	{

	};

	~UIComponent( );

	void Initialize( );

private:

	std::string _name;
	luabind::object _parentController;
	Gui* _gui;
	Script* _script;

};


#endif