#ifndef __SCREEN_H
#define __SCREEN_H

#include <string>

#include "../Scripting/Script.h"
#include <MyGUI.h>
using namespace MyGUI;

/*!
	Represents a pre-defined View displayed to the User
*/
class Screen
{

public:

	Screen( const std::string screenName, const unsigned int visibilityMask = 0 )
		: _screenName( screenName )
		, _visibilityMask( visibilityMask )
		, _script( 0 )
		, _gui( 0 )
	{ };

	virtual ~Screen( );

	/*! Initializes the Screen */
	bool Initialize( Gui* gui, Script* script );

	/*! Returns the name of the Screen */
	inline const std::string GetScreenName( ) const { return _screenName; };

	/*! Returns the Visibility Flag of the Screen */
	inline const unsigned int GetVisibilityMask( ) const { return _visibilityMask; };

private:

	static void FromLua_ChangeScreen( std::string screenName );
	static void FromLua_ShowMouse( );

	bool loadGUI( );
	bool loadScript( );

	const std::string _screenName;
	const unsigned int _visibilityMask;

	Gui* _gui;
	Script* _script;
};


#endif