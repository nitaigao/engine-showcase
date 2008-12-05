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

	GuiScreen( const std::string name, const unsigned int visibilityMask = 0 )
		: GuiComponent( name )
		, _visibilityMask( visibilityMask )
		, _gui( 0 )
	{
		_componentType = SCREEN;
	};

	virtual ~GuiScreen( );

	/*! Initializes the Screen */
	//void Initialize( Gui* gui, Script* script );

	/*! Returns the Visibility Flag of the Screen */
	inline const unsigned int GetVisibilityMask( ) const { return _visibilityMask; };

private:

	static void FromLua_ChangeScreen( std::string screenName );
	static void FromLua_ShowMouse( );
	static void FromLua_AddControl( ); 
	static int FromLua_GetScreenWidth( );
	static int FromLua_GetScreenHeight( );

	bool loadGUI( );
	bool loadScript( );

	const unsigned int _visibilityMask;

	Gui* _gui;

};

#endif