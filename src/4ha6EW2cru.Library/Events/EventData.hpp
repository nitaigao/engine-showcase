/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\Events\EventData.hpp
*  @date   2009/04/25
*/
#ifndef __EVENTDATA_H
#define __EVENTDATA_H

#include "IEventData.hpp"
#include "OIS/OISMouse.h"
#include "OIS/OISKeyboard.h"

namespace Events
{
	/*! 
	 *  Contains Key information for use with Key Events
	 */
	class KeyEventData : public IEventData
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~KeyEventData( ) { };


		/*! Default Constructor
		 *
		 *  @param[in] const OIS::KeyCode & keyCode
		 *  @param[in] const std::string keyText
		 *  @return ()
		 */
		KeyEventData( const OIS::KeyCode &keyCode, const std::string keyText )
			: _keyCode( keyCode )
			, _keyText( keyText )
		{

		};

		
		/*! Returns the Key Code of the key pressed in the Event
		 *
		 *  @return (OIS::KeyCode)
		 */
		inline OIS::KeyCode GetKeyCode( ) { return _keyCode; };

		
		/*! Returns the text of the key pressed in the Event
		 *
		 *  @return (std::string)
		 */
		inline std::string GetKeyText( ) { return _keyText; };

	private:

		OIS::KeyCode _keyCode;
		std::string _keyText;

		KeyEventData( ) { };
		KeyEventData( const KeyEventData & copy ) { };
		KeyEventData & operator = ( const KeyEventData & copy ) { return *this; };

	};

	/*!
		Contains MouseState information for use with Mouse Events
	*/
	class MouseEventData : public IEventData
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~MouseEventData( ) { };


		/*! Default Constructor
		 *
		 *  @param[in] const OIS::MouseState & mouseState
		 *  @param[in] const OIS::MouseButtonID & mouseButtonId
		 *  @return ()
		 */
		MouseEventData( const OIS::MouseState& mouseState, const OIS::MouseButtonID& mouseButtonId )
			: _mouseState( mouseState )
			, _mouseButtonId( mouseButtonId )
		{

		};

		
		/*! Returns the MouseState of the mouse firing the Event
		 *
		 *  @return (const OIS::MouseState&)
		 */
		inline const OIS::MouseState& GetMouseState( ) const { return _mouseState; };

		
		/*! Returns the Mouse Button associated with the Event
		 *
		 *  @return (const OIS::MouseButtonID&)
		 */
		inline const OIS::MouseButtonID& GetMouseButtonId( ) const { return _mouseButtonId; };

	private:

		OIS::MouseState _mouseState;
		OIS::MouseButtonID _mouseButtonId;

		MouseEventData( ) { };
		MouseEventData( const MouseEventData & copy ) { };
		MouseEventData & operator = ( const MouseEventData & copy ) { return *this; };

	};

	class ScriptCommandEventData : public IEventData
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~ScriptCommandEventData( ){ };


		/*! Default Constructor
		 *
		 *  @param[in] const std::string command
		 *  @return ()
		 */
		ScriptCommandEventData( const std::string command )
			: _command( command )
		{

		}


		/*! Returns the command that has been executed
		 *
		 *  @return (std::string)
		 */
		inline std::string GetCommand( ) { return _command; };

	private:

		std::string _command;

		ScriptCommandEventData( ) { };
		ScriptCommandEventData( const ScriptCommandEventData & copy ) { };
		ScriptCommandEventData & operator = ( const ScriptCommandEventData & copy ) { return *this; };

	};

	class LevelChangedEventData : public IEventData
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~LevelChangedEventData( ) { };


		/*! Default Constructor
		 *
		 *  @param[in] const std::string levelName
		 *  @return ()
		 */
		LevelChangedEventData( const std::string levelName )
			: _levelName( levelName )
		{

		}

		
		/*! Returns the name of the level requested
		 *
		 *  @return (std::string)
		 */
		inline std::string GetLevelName( ) { return _levelName; };

	private:

		std::string _levelName;

		LevelChangedEventData( ) { };
		LevelChangedEventData( const LevelChangedEventData & copy ) { };
		LevelChangedEventData & operator = ( const LevelChangedEventData & copy ) { return *this; };

	};

	class MessageLoggedEventData : public IEventData
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		~MessageLoggedEventData( ) { };


		/*! Default Constructor
		 *
		 *  @param[in] const std::string message
		 *  @return ()
		 */
		MessageLoggedEventData( const std::string message )
			: _message( message )
		{

		}

		
		/*! Returns the message that has been logged
		 *
		 *  @return (std::string)
		 */
		inline std::string GetMessage( ) { return _message; };

	private:

		std::string _message;

		MessageLoggedEventData( ) { };
		MessageLoggedEventData( const MessageLoggedEventData & copy ) { };
		MessageLoggedEventData & operator = ( const MessageLoggedEventData & copy ) { return *this; };


	};
};

#endif
