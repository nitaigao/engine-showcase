#ifndef __ROOTUICONTROLLER_H
#define __ROOTUICONTROLLER_H

#include <MyGUI.h>
using namespace MyGUI;

#include <luabind/luabind.hpp>
using namespace luabind;

#include "../../Scripting/Script.h"

class RootUIController
{

	typedef std::vector< Script* > UIComponentList;
	typedef std::queue< std::string > UIEventQueue;

public:

	RootUIController( luabind::object luaObject, Gui* gui )
		: _gui( gui )
		, _luaObject( luaObject )
	{

	};

	~RootUIController( );

	/*! Loads a UI Component for Rendering */
	void LoadComponent( const std::string componentName );

	/*! Destroys all active UI Components */
	void DestroyAllComponents( );

	/*! Processes the Message Queue for the GUI */
	void Update( int deltaMilliseconds );

	inline int GetScreenWidth( )
	{
		return _gui->getRenderWindow( )->getWidth( );
	}

	inline int GetScreenHeight( )
	{
		return _gui->getRenderWindow( )->getHeight( );
	}

	inline void QueueEvent( const std::string eventName )
	{
		_eventQueue.push( eventName );
	}

	inline void ShowMouse( )
	{
		_gui->showPointer( );
	}

	inline void HideMouse( )
	{
		_gui->hidePointer( );
	}

private:

	Gui* _gui;
	UIComponentList _components;
	luabind::object _luaObject;
	UIEventQueue _eventQueue;

};


#endif