#ifndef __ROOTUICONTROLLER_H
#define __ROOTUICONTROLLER_H

#include <MyGUI.h>
using namespace MyGUI;

#include <luabind/luabind.hpp>
using namespace luabind;

#include "IInterface.hpp"

#include "../Configuration/IConfiguration.hpp"
#include "../System/ISystemScene.hpp"

#include "../Events/IEvent.hpp"
#include "../Events/EventData.hpp"

#include "../Logging/Logger.h"

class Interface : MyGUI::IUnlinkWidget, public IInterface
{
	typedef std::map< const std::string, object* > WidgetUserData;

public:

	virtual ~Interface( );

	Interface( Configuration::IConfiguration* configuration, Ogre::RenderWindow* renderWindow )
		: _renderWindow( renderWindow )
		, _configuration( configuration )
		, _gui( new Gui( ) )
		, _updateSkipCount( 0 )
	{

	}

	void Initialize( );
	void Update( const float deltaMilliseconds );

	void ResetWidgetPositions( );

	virtual void _unlinkWidget ( WidgetPtr widget );

private:

	/* -- UI Script Helpers -- */

	/*! Loads a UI Component for Rendering */
	static void LoadComponent( const std::string componentName );

	/*! Registers the Global UI functions for the State */
	static void RegisterGlobals( lua_State* luaState );

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

	/*! Sets a Widget Caption */
	static inline void SetText( WidgetPtr widget, const std::string& text ) { widget->setCaption( text ); };

	/*! Gets a WidgetCaption */
	static inline std::string GetText( WidgetPtr widget ) { return widget->getCaption( ).asUTF8( ); };

	static inline void AddText( EditPtr editWidget, const std::string& text ) { editWidget->addText( text ); };

	/*! Sets the focus to the given Widget */
	static void SetFocus( WidgetPtr widget, bool focus );

	/* -- Game Script Helpers -- */

	/*! Sets whether or not the Player controls can manipulate the scene */
	static void SetInputAllowed( bool inputAllowed );

	/*! Executes the specified string as if it were LUA code */
	static void ExecuteString( const std::string& input );

	/*! Registers a script method to receive all broadcast events */
	static void RegisterEventHandler( object function );

	/*! Unregisters a script method that was receiving all broadcast events */
	static void UnRegisterEventHandler( object function );

	/*! Fires a Script event */
	static void BroadcastEvent( const std::string& eventName );

	/*! Causes the Renderer to adjust the Resolution to match the Configuration */
	static void ChangeResolution( int width, int height, bool isFullScreen );

	/*! Sets the Far Clip Distance of the Camera */
	static void SetFarClip( const float& farClip );

	/*! Registers a script method to receive Script events */
	static void RegisterEvent( Events::EventType eventType, object function );

	/*! UnRegisters a script method to receive Script events */
	static void UnRegisterEvent( Events::EventType eventType, object function );

	/*! Returns the Screen Width in pixels */
	static inline int GetScreenWidth( ) { return Ogre::Root::getSingletonPtr( )->getRenderTarget( "Interactive View" )->getWidth( ); };

	/*! Returns the Screen Height in pixels */
	static inline int GetScreenHeight( ) { return Ogre::Root::getSingletonPtr( )->getRenderTarget( "Interactive View" )->getHeight( ); };

	/*! Returns the Average FPS from the renderer */
	static inline float GetFps( ) { return static_cast< int >( Ogre::Root::getSingletonPtr( )->getRenderTarget( "Interactive View" )->getAverageFPS( ) ); }

	/*! Returns a list of supported Video Resolutions */
	static std::vector< std::string > GetSupportedResolutions( );

	/* -- Internal Event Handlers --*/

	/*! Handler to intercept OnMouseMoved Events */
	void OnMouseMoved( const Events::IEvent* event );

	/*! Handler to intercept OnMouseMoved Events */
	void OnMousePressed( const Events::IEvent* event );

	/*! Handler to intercept OnMouseMoved Events */
	void OnMouseReleased( const Events::IEvent* event );

	/*! Handler to intercept OnKeyUp Events */
	void OnKeyUp( const Events::IEvent* event );

	/*! Handler to intercept OnKeyDown Events */
	void OnKeyDown( const Events::IEvent* event );

	Configuration::IConfiguration* _configuration;
	Ogre::RenderWindow* _renderWindow;
	Gui* _gui;
	int _updateSkipCount;

	Interface( ) { };
	Interface( const Interface & copy ) { };
	Interface & operator = ( const Interface & copy ) { return *this; };

};

#endif
