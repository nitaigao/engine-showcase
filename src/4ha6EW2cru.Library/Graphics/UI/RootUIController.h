#ifndef __ROOTUICONTROLLER_H
#define __ROOTUICONTROLLER_H

#include <MyGUI.h>
using namespace MyGUI;

#include <luabind/luabind.hpp>
using namespace luabind;

#include "UIComponent.h"

class RootUIController
{

	typedef std::vector< UIComponent* > UIComponentList;

public:

	RootUIController( Gui* gui )
		: _gui( gui )
	{

	};

	~RootUIController( );

	/*! Loads a UI Component for Rendering */
	void LoadComponent( const std::string componentName );

	/*! Destroys all active UI Components */
	void DestroyAllComponents( );

private:

	Gui* _gui;
	UIComponentList _components;

};


#endif