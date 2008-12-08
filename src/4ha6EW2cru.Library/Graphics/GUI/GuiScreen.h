#ifndef __GUISCREEN_H
#define __GUISCREEN_H

#include "GuiComponent.h"

#include <string>

#include "../../Scripting/Script.h"
#include <MyGUI.h>
using namespace MyGUI;

/*!
	Represents a Screen Layout that is displayed to the User
*/
class GuiScreen : public GuiComponent
{

public: 

	GuiScreen( const std::string name )
		: GuiComponent( name )
	{

	};

	virtual ~GuiScreen( ) { };

	/*! Initializes the Screen */
	void Initialize( );

private:

	static void FromLua_ChangeScreen( std::string screenName );
	static void FromLua_ShowMouse( );
	static void FromLua_AddControl( ); 
	static int FromLua_GetScreenWidth( );
	static int FromLua_GetScreenHeight( );

};

#endif