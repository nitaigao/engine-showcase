#ifndef __ROOTUICONTROLLER_H
#define __ROOTUICONTROLLER_H

#include <MyGUI.h>
using namespace MyGUI;

#include <luabind/luabind.hpp>
using namespace luabind;

#include "../../Scripting/Script.h"

class RootUIController
{
	typedef std::map< const std::string, object* > WidgetUserData;
	typedef std::vector< WidgetPtr > WidgetList;

public:

	RootUIController( Gui*  gui )
		: _gui( gui )
	{

	}

	void Initialize( );

	~RootUIController( );

private:

	/*! Loads a UI Component for Rendering */
	static void LoadComponent( const std::string componentName );

	/*! Retrieves a widget from the UI */
	static WidgetPtr FindWidget( const std::string widgetName );
	
	/*! Attaches an LUA function to a Widget Event */
	static void ScriptWidget( Widget* widget, const std::string eventName, object function );
	
	/*! Returns the Screen Width in pixels */
	static inline int GetScreenWidth( ) { return Gui::getInstancePtr( )->getRenderWindow( )->getWidth( ); };
	
	/*! Returns the Screen Height in pixels */
	static inline int GetScreenHeight( ) { return Gui::getInstancePtr( )->getRenderWindow( )->getHeight( ); };
	
	/*! Shows the mouse */
	static inline void ShowMouse( ) { Gui::getInstancePtr( )->showPointer( ); };
	
	/*! Hides the mouse */
	static inline void HideMouse( ) { Gui::getInstancePtr( )->hidePointer( ); };

	void OnMouseReleased( Widget* widget, int left, int top );
	
	Gui* _gui;
	Script* _script;
	WidgetList _widgetList;

};


#endif