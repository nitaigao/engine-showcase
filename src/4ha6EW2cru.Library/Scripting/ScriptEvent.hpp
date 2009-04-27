/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ScriptEvent.hpp
*  @date   2009/04/27
*/
#ifndef __SCRIPTEVENT_HPP
#define __SCRIPTEVENT_HPP

#include "../Events/IEvent.hpp"
#include "../Events/IEventData.hpp"
#include "../Events/EventType.hpp"

namespace Script
{
	/*!
	 *  A Script Event with a string name 
	 */
	class ScriptEvent : public Events::IEvent
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~ScriptEvent( ) { };


		/*! Constructor accepting no parameters
		 *
		 *  @param[in] const std::string eventName
		 *  @return ()
		 */
		ScriptEvent( const std::string eventName )
			: _eventName( eventName )
			, _value1( "" )
			, _value2( "" )
		{

		}


		/*! Constructor accepting parameters
		 *
		 *  @param[in] const std::string eventName
		 *  @param[in] const std::string value1
		 *  @return ()
		 */
		ScriptEvent( const std::string eventName, const std::string value1 )
			: _eventName( eventName )
			, _value1( value1 )
			, _value2( "" )
		{

		}


		/*! Constructor accepting parameters
		 *
		 *  @param[in] const std::string eventName
		 *  @param[in] const int value1
		 *  @return ()
		 */
		ScriptEvent( const std::string eventName, const int value1 )
			: _eventName( eventName )
			, _value2( "" )
		{
			std::stringstream val1;
			val1 << value1;
			_value1 = val1.str( );
		}


		/*! Constructor accepting parameters
		 *
		 *  @param[in] const std::string eventName
		 *  @param[in] const std::string value1
		 *  @param[in] const std::string value2
		 *  @return ()
		 */
		ScriptEvent( const std::string eventName, const std::string value1, const std::string value2 )
			: _eventName( eventName )
			, _value1( value1 )
			, _value2( value2 )
		{

		}


		/*! Constructor accepting parameters
		 *
		 *  @param[in] const std::string eventName
		 *  @param[in] const int value1
		 *  @param[in] const std::string value2
		 *  @return ()
		 */
		ScriptEvent( const std::string eventName, const int value1, const std::string value2 )
			: _eventName( eventName )
			, _value2( value2 )
		{
			std::stringstream val1;
			val1 << value1;
			_value1 = val1.str( );
		}


		/*! Constructor accepting parameters
		 *
		 *  @param[in] const std::string eventName
		 *  @param[in] const std::string value1
		 *  @param[in] const int value2
		 *  @return ()
		 */
		ScriptEvent( const std::string eventName, const std::string value1, const int value2 )
			: _eventName( eventName )
			, _value1( value1 )
		{
			std::stringstream val2;
			val2 << value2;
			_value2 = val2.str( );
		}


		/*! Constructor accepting parameters
		 *
		 *  @param[in] const std::string eventName
		 *  @param[in] const int value1
		 *  @param[in] const int value2
		 *  @return ()
		 */
		ScriptEvent( const std::string eventName, const int value1, const int value2 )
			: _eventName( eventName )
		{
			std::stringstream val1;
			val1 << value1;
			_value1 = val1.str( );

			std::stringstream val2;
			val2 << value2;
			_value2 = val2.str( );
		}

		
		/*!  Returns the name of the event being passed to the Script
		 *
		 *  @return (std::string)
		 */
		inline std::string GetEventName( ) { return _eventName; };

		
		/*! Returns the first value passed to the script
		 *
		 *  @return (const std::string)
		 */
		inline const std::string GetValue1( ) { return _value1; };

		
		/*! Returns the second value passed to the script
		 *
		 *  @return (const std::string)
		 */
		inline const std::string GetValue2( ) { return _value2; };


		/*! Overrides the event type, so that the Script Components can forward all events to LUA
		 *
		 *  @return (Events::EventType)
		 */
		inline Events::EventType GetEventType( ) const { return Events::ALL_EVENTS; };


		/*! Returns Event Data attached to the event, this will be null as the data is contained within the value functions GetValue1( ) & GetValue2( )
		 *
		 *  @return (Events::IEventData*)
		 */
		inline Events::IEventData* GetEventData( ) const { return 0; };

	private:

		std::string _eventName;
		std::string _value1;
		std::string _value2;

		ScriptEvent( ) { };
		ScriptEvent( const ScriptEvent & copy ) { };
		ScriptEvent & operator = ( const ScriptEvent & copy ) { return *this; };

	};
};

#endif