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

	KeyEventData( const OIS::KeyCode &keyCode, const std::string keyText ) 
		: _keyCode( keyCode )
		, _keyText( keyText )
	{ 
	
	};

	/* Returns the keycode of the key pressed in the Event */
	inline OIS::KeyCode GetKeyCode( ) { return _keyCode; };

	/* Returns the text of the key pressed in the Event */
	inline std::string GetKeyText( ) { return _keyText; };

private:

	//KeyEventData( ) { };
	KeyEventData( const KeyEventData & copy ) { };
	KeyEventData & operator = ( const KeyEventData & copy ) { return *this; };

	OIS::KeyCode _keyCode;
	std::string _keyText;

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

class AppenderEventData : public IEventData
{

public:

	~AppenderEventData( ) { };

	AppenderEventData( const std::string message )
		: _message( message )
	{

	}

	/*! Returns the Message that has been Appended */
	inline std::string GetMessage( ) { return _message; };

private:

	std::string _message;

};

class ScriptCommandEventData : public IEventData
{

public:

	~ScriptCommandEventData( ){ };

	ScriptCommandEventData( const std::string command )
		: _command( command )
	{

	}

	/*! Returns the command that has been executed */
	inline std::string GetCommand( ) { return _command; };

private:

	std::string _command;

};

class LevelChangedEventData : public IEventData
{

public:

	~LevelChangedEventData( ) { };

	LevelChangedEventData( const std::string levelName )
		: _levelName( levelName )
	{

	}

	/*! Returns the name of the level requested */
	inline std::string GetLevelName( ) { return _levelName; };

private:

	std::string _levelName;

};

#endif