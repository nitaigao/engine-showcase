/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   InputEventData.hpp
*  @date   2009/07/16
*/
#pragma once
#ifndef INPUTEVENTDATA_HPP
#define INPUTEVENTDATA_HPP

#include <OISMouse.h>
#include <OISKeyboard.h>

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
			: m_keyCode( keyCode )
			, m_keyText( keyText )
		{

		};


		/*! Returns the Key Code of the key pressed in the Event
		*
		*  @return (OIS::KeyCode)
		*/
		inline OIS::KeyCode GetKeyCode( ) { return m_keyCode; };


		/*! Returns the text of the key pressed in the Event
		*
		*  @return (std::string)
		*/
		inline std::string GetKeyText( ) { return m_keyText; };

	private:

		OIS::KeyCode m_keyCode;
		std::string m_keyText;

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
			: m_mouseState( mouseState )
			, m_mouseButtonId( mouseButtonId )
		{

		};


		/*! Returns the MouseState of the mouse firing the Event
		*
		*  @return (const OIS::MouseState&)
		*/
		inline const OIS::MouseState& GetMouseState( ) const { return m_mouseState; };


		/*! Returns the Mouse Button associated with the Event
		*
		*  @return (const OIS::MouseButtonID&)
		*/
		inline const OIS::MouseButtonID& GetMouseButtonId( ) const { return m_mouseButtonId; };

	private:

		OIS::MouseState m_mouseState;
		OIS::MouseButtonID m_mouseButtonId;

		MouseEventData( ) { };
		MouseEventData( const MouseEventData & copy ) { };
		MouseEventData & operator = ( const MouseEventData & copy ) { return *this; };

	};
};

#endif