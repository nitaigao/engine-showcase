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

public:

	virtual ~GuiView( ) { };

	GuiView( std::string name )
		: _name( name )
	{
		
	};

	virtual void Initialize( );
	MyGUI::WidgetPtr FindControl( std::string name );

private:

	std::string _name;

};

#endif