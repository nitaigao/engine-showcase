#ifndef __ROOTUICONTROLLER_H
#define __ROOTUICONTROLLER_H

#include <MyGUI.h>
using namespace MyGUI;

#include <luabind/luabind.hpp>
using namespace luabind;

#include "IInterface.hpp"

#include "../Events/IEvent.hpp"
#include "../Events/EventData.hpp"

class Interface : MyGUI::IUnlinkWidget, public IInterface
{
	typedef std::map< const std::string, object* > WidgetUserData;

public:

	Interface( Ogre::Root* ogreRoot )
		: _ogreRoot( ogreRoot )
	{
		_gui = new Gui( );
	}

	void Initialize( );
	void Update( const float deltaMilliseconds ) const;

	virtual ~Interface( );

	virtual void _unlinkWidget ( WidgetPtr widget );

private:

	/* -- Script Helpers -- */

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

	/*! Forwards Mouse Button Released Events to the subscribing Widgets in Script */
	static void OnMouseReleased( WidgetPtr widget, int left, int top, MouseButton id );

	/*! Casts a widget to a Button */
	static inline ButtonPtr AsButton( WidgetPtr widget ) { return static_cast< ButtonPtr >( widget ); };

	/*! Casts a widget to a ComboBox */
	static inline ComboBoxPtr AsComboBox( WidgetPtr widget ) { return static_cast< ComboBoxPtr >( widget ); };

	/* -- Internal Event Handlers --*/

	/*! Handler to intercept OnMouseMoved Events */
	void OnMouseMoved( const IEvent* event );

	/*! Handler to intercept OnMouseMoved Events */
	void OnMousePressed( const IEvent* event );

	/*! Handler to intercept OnMouseMoved Events */
	void OnMouseReleased( const IEvent* event );

	/*! Handler to intercept OnKeyUp Events */
	void OnKeyUp( const IEvent* event );

	/*! Handler to intercept OnKeyDown Events */
	void OnKeyDown( const IEvent* event );


	Ogre::Root* _ogreRoot;
	Gui* _gui;

};


#endif
