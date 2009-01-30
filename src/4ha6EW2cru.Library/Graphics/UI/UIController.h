#ifndef __UICONTROLLER_H
#define __UICONTROLLER_H

#include <MyGUI.h>
using namespace MyGUI;

#include <luabind/luabind.hpp>
using namespace luabind;

#include "UIComponent.h"

class UIController
{

	typedef std::vector< UIComponent* > UIComponentList;

public:

	UIController( luabind::object luaObject )
		: _luaObject( luaObject )
		, _gui( Gui::getInstancePtr( ) )
	{

	};

	~UIController( );

	/*! Loads a UI Component for Rendering */
	void LoadComponent( const std::string componentName, luabind::object parentController );

	/*! Destroys all active UI Components */
	void DestroyAllComponents( );

private:

	luabind::object _luaObject;
	Gui* _gui;
	UIComponentList _components;

};


#endif