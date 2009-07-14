/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ScriptEvent.hpp
*  @date   2009/04/27
*/
#ifndef SCRIPTEVENT_HPP
#define SCRIPTEVENT_HPP

#include "IEvent.hpp"
#include "IEventData.hpp"
#include "EventType.hpp"

namespace Events
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
			: m_eventName( eventName )
			, m_value1String( "" )
			, m_value2String( "" )
			, m_value1Int( 0 )
			, m_value2Int( 0 )
			, m_paramType( PARAMCOMBO_NONE )
		{

		}


		/*! Constructor accepting parameters
		 *
		 *  @param[in] const std::string eventName
		 *  @param[in] const std::string value1
		 *  @return ()
		 */
		ScriptEvent( const std::string eventName, const std::string value1 )
			: m_eventName( eventName )
			, m_value1String( value1 )
			, m_value2String( "" )
			, m_value1Int( 0 )
			, m_value2Int( 0 )
			, m_paramType( PARAMCOMBO_STRING )
		{

		}


		/*! Constructor accepting parameters
		 *
		 *  @param[in] const std::string eventName
		 *  @param[in] const int value1
		 *  @return ()
		 */
		ScriptEvent( const std::string eventName, const int value1 )
			: m_eventName( eventName )
			, m_value1String( "" )
			, m_value2String( "" )
			, m_value1Int( value1 )
			, m_value2Int( 0 )
			, m_paramType( PARAMCOMBO_INT )
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
			: m_eventName( eventName )
			, m_value1String( value1 )
			, m_value2String( value2 )
			, m_value1Int( 0 )
			, m_value2Int( 0 )
			, m_paramType( PARAMCOMBO_STRING_STRING )
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
			: m_eventName( eventName )
			, m_value1String( "" )
			, m_value2String( value2 )
			, m_value1Int( value1 )
			, m_value2Int( 0 )
			, m_paramType( PARAMCOMBO_INT_STRING )
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
			: m_eventName( eventName )
			, m_value1String( value1 )
			, m_value2String( "" )
			, m_value1Int( 0 )
			, m_value2Int( value2 )
			, m_paramType( PARAMCOMBO_STRING_INT )
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
			: m_eventName( eventName )
			, m_value1String( "" )
			, m_value2String( "" )
			, m_value1Int( value1 )
			, m_value2Int( value2 )
			, m_paramType( PARAMCOMBO_INT_INT )
		{

		}

		
		/*!  Returns the name of the event being passed to the Script
		 *
		 *  @return (std::string)
		 */
		inline std::string GetEventName( ) const { return m_eventName; };

		
		/*! Returns the first value passed to the script as a string
		 *
		 *  @return (const std::string)
		 */
		inline const std::string GetValue1AsString( ) const { return m_value1String; };


		/*! Returns the first value passed to the script as an integer
		 *
		 *  @return (const int)
		 */
		inline const int GetValue1AsInt( ) const { return m_value1Int; };
		

		/*! Returns the second value passed to the script as a string
		 *
		 *  @return (const std::string)
		 */
		inline const std::string GetValue2AsString( ) const { return m_value2String; };


		/*! Returns the second value pass to the script as an integer
		 *
		 *  @return (const int)
		 */
		inline const int GetValue2AsInt( ) const { return m_value2Int; };


		/*! Returns the type of parameter combination data this event holds
		 *
		 *  @return (const ParamComboType)
		 */
		inline const ParamComboType GetParamType( ) const { return m_paramType; };


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

		std::string m_eventName;

		std::string m_value1String;
		std::string m_value2String;

		int m_value1Int;
		int m_value2Int;

		ParamComboType m_paramType;

		ScriptEvent( ) { };
		ScriptEvent( const ScriptEvent & copy ) { };
		ScriptEvent & operator = ( const ScriptEvent & copy ) { return *this; };

	};
};

#endif