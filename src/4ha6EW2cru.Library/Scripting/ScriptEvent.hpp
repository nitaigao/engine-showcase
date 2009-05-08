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

		enum ParamComboType
		{
			PARAMCOMBO_NONE = 0,
			PARAMCOMBO_STRING = 1,
			PARAMCOMBO_INT = 2,
			PARAMCOMBO_STRING_STRING = 3,
			PARAMCOMBO_STRING_INT = 4,
			PARAMCOMBO_INT_STRING = 5,
			PARAMCOMBO_INT_INT = 6
		};

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
			, _value1String( "" )
			, _value2String( "" )
			, _value1Int( 0 )
			, _value2Int( 0 )
			, _paramType( PARAMCOMBO_NONE )
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
			, _value1String( value1 )
			, _value2String( "" )
			, _value1Int( 0 )
			, _value2Int( 0 )
			, _paramType( PARAMCOMBO_STRING )
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
			, _value1String( "" )
			, _value2String( "" )
			, _value1Int( value1 )
			, _value2Int( 0 )
			, _paramType( PARAMCOMBO_INT )
		{

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
			, _value1String( value1 )
			, _value2String( value2 )
			, _value1Int( 0 )
			, _value2Int( 0 )
			, _paramType( PARAMCOMBO_STRING_STRING )
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
			, _value1String( "" )
			, _value2String( value2 )
			, _value1Int( value1 )
			, _value2Int( 0 )
			, _paramType( PARAMCOMBO_INT_STRING )
		{

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
			, _value1String( value1 )
			, _value2String( "" )
			, _value1Int( 0 )
			, _value2Int( value2 )
			, _paramType( PARAMCOMBO_STRING_INT )
		{

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
			, _value1String( "" )
			, _value2String( "" )
			, _value1Int( value1 )
			, _value2Int( value2 )
			, _paramType( PARAMCOMBO_INT_INT )
		{

		}

		
		/*!  Returns the name of the event being passed to the Script
		 *
		 *  @return (std::string)
		 */
		inline std::string GetEventName( ) { return _eventName; };

		
		/*! Returns the first value passed to the script as a string
		 *
		 *  @return (const std::string)
		 */
		inline const std::string GetValue1AsString( ) { return _value1String; };


		/*! Returns the first value passed to the script as an integer
		 *
		 *  @return (const int)
		 */
		inline const int GetValue1AsInt( ) { return _value1Int; };
		

		/*! Returns the second value passed to the script as a string
		 *
		 *  @return (const std::string)
		 */
		inline const std::string GetValue2AsString( ) { return _value2String; };


		/*! Returns the second value pass to the script as an integer
		 *
		 *  @return (const int)
		 */
		inline const int GetValue2AsInt( ) { return _value2Int; };


		/*! Returns the type of parameter combination data this event holds
		 *
		 *  @return (const ParamComboType)
		 */
		inline const ParamComboType GetParamType( ) const { return _paramType; };


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

		std::string _value1String;
		std::string _value2String;

		int _value1Int;
		int _value2Int;

		ParamComboType _paramType;

		ScriptEvent( ) { };
		ScriptEvent( const ScriptEvent & copy ) { };
		ScriptEvent & operator = ( const ScriptEvent & copy ) { return *this; };

	};
};

#endif