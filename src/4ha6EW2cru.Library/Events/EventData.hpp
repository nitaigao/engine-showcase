#ifndef __EVENTDATA_H
#define __EVENTDATA_H

#include "IEventData.hpp"
#include "ois/OISMouse.h"
#include "OIS/OISKeyboard.h"

/*! 
	Contains Key information for use with Key Events 
*/
class KeyEventData : public IEventData
{

public:

	~KeyEventData( ) { };

	KeyEventData( const OIS::KeyCode& keyCode ) 
		: _keyCode( keyCode )
	{ 
	
	};

	/* Returns the keycode of the key pressed in the Event */
	inline const OIS::KeyCode& GetKeyCode( ) const { return _keyCode; };

private:

	KeyEventData( ) { };
	KeyEventData( const KeyEventData & copy ) { };
	KeyEventData & operator = ( const KeyEventData & copy ) { return *this; };

	OIS::KeyCode _keyCode;

};

/*! 
	Contains MouseState information for use with Mouse Events 
*/
class MouseEventData : public IEventData
{

public:

	~MouseEventData( ) { };

	MouseEventData( const OIS::MouseState& mouseState, const OIS::MouseButtonID& mouseButtonId ) 
		: _mouseState( mouseState )
		, _mouseButtonId( mouseButtonId )
	{ 
	
	};

	/* Returns the MouseState of the mouse firing the Event */
	inline const OIS::MouseState& GetMouseState( ) const { return _mouseState; };

	/*! Returns the Mouse Button associated with the Event */
	inline const OIS::MouseButtonID& GetMouseButtonId( ) const { return _mouseButtonId; };

private:

	MouseEventData( ) { };
	MouseEventData( const MouseEventData & copy ) { };
	MouseEventData & operator = ( const MouseEventData & copy ) { return *this; };

	OIS::MouseState _mouseState;
	OIS::MouseButtonID _mouseButtonId;

};


/*!
	Contains the information required to change the current screen 
*/
class ChangeScreenEventData : public IEventData
{

public:

	~ChangeScreenEventData( ) { };

	ChangeScreenEventData( const std::string screenName )
		: _screenName( screenName )
	{

	}

	/*! Returns the name of the new Screen */
	inline const std::string GetScreenName( ) const { return _screenName; };
	
private:

	ChangeScreenEventData( ) { };
	ChangeScreenEventData( const ChangeScreenEventData & copy ) { };
	ChangeScreenEventData & operator = ( const ChangeScreenEventData & copy ) { return *this; };

	std::string _screenName;

};

#endif