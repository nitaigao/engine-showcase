#ifndef __ROOTUICONTROLLER_H
#define __ROOTUICONTROLLER_H

#include <MyGUI.h>
using namespace MyGUI;

#include <luabind/luabind.hpp>
using namespace luabind;

#include "IInterface.hpp"

#include "../System/Configuration.h"
#include "../System/ISystemScene.hpp"

#include "../Events/IEvent.hpp"
#include "../Events/EventData.hpp"

#include "../Logging/Logger.h"

class Interface : MyGUI::IUnlinkWidget, public IInterface
{
	typedef std::map< const std::string, object* > WidgetUserData;

public:

	virtual ~Interface( );

	Interface( Configuration* configuration, Ogre::Root* ogreRoot )
		: _ogreRoot( ogreRoot )
		, _configuration( configuration )
		, _gui( new Gui( ) )
	{

	}

	void Initialize( );
	void Update( const float deltaMilliseconds ) const;

	virtual void _unlinkWidget ( WidgetPtr widget );

private:

	/* -- UI Script Helpers -- */

	/*! Loads a UI Component for Rendering */
	static void LoadComponent( const std::string componentName );

	/*! Retrieves a widget from the UI */
	static WidgetPtr FindWidget( const std::string widgetName );

	/*! Attaches an LUA function to a Widget Event */
	static void ScriptWidget( Widget* widget, const std::string eventName, object function );

	/*! Shows the mouse */
	static inline void ShowMouse( ) { Gui::getInstancePtr( )->showPointer( ); };

	/*! Hides the mouse */
	static inline void HideMouse( ) { Gui::getInstancePtr( )->hidePointer( ); };

	/*! Forwards Mouse Button Released Events to the subscribing Widgets in Script */
	static void OnMouseReleased( WidgetPtr widget, int left, int top, MouseButton id );

	/*! Forwards Key Up Events to the subscribing Widgets in Script */
	static void OnKeyUp( WidgetPtr widget, KeyCode key );

	/*! Casts a widget to a Button */
	static inline ButtonPtr AsButton( WidgetPtr widget ) { return static_cast< ButtonPtr >( widget ); };

	/*! Casts a widget to a ComboBox */
	static inline ComboBoxPtr AsComboBox( WidgetPtr widget ) { return static_cast< ComboBoxPtr >( widget ); };

	/*! Casts a widget to an EditBox */
	static inline EditPtr AsEditBox( WidgetPtr widget ) { return static_cast< EditPtr >( widget ); };

	/*! Casts a widget to a ProgressBar */
	static inline ProgressPtr AsProgressBar( WidgetPtr widget ) { return static_cast< ProgressPtr >( widget ); };

	/*! Converts the given type to a std::string */
	static inline std::string AsString( Ogre::UTFString input ) { return std::string( input ); };

	/*! Converts the given type to an Ogre::UTFString */
	static inline Ogre::UTFString AsUtf( std::string input ) { return Ogre::UTFString( input ); };

	/* -- Game Script Helpers -- */

	/*! Sets whether or not the Player controls can manipulate the scene */
	static void SetInputAllowed( bool inputAllowed );

	/*! Executes the specified string as if it were LUA code */
	static void ExecuteString( const std::string& input );

	/*! Registers a script method to receive all broadcast events */
	static void RegisterEventHandler( object function );

	/*! Registers a script method to receive in-game events */
	static void RegisterEvent( EventType eventType, object function );

	/*! UnRegisters a script method to receive in-game events */
	static void UnRegisterEvent( EventType eventType, object function );

	/*! Fires an in-game event */
	static void BroadcastEvent( EventType eventType );

	/*! Returns the Screen Width in pixels */
	static inline int GetScreenWidth( ) { return Ogre::Root::getSingletonPtr( )->getRenderTarget( "Interactive View" )->getWidth( ); };

	/*! Returns the Screen Height in pixels */
	static inline int GetScreenHeight( ) { return Ogre::Root::getSingletonPtr( )->getRenderTarget( "Interactive View" )->getHeight( ); };

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

	Configuration* _configuration;
	Ogre::Root* _ogreRoot;
	Gui* _gui;

	Interface( ) { };
	Interface( const Interface & copy ) { };
	Interface & operator = ( const Interface & copy ) { return *this; };

};

#endif
