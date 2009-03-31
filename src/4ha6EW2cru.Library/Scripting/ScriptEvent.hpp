#ifndef __SCRIPTEVENT_H
#define __SCRIPTEVENT_H

#include "../Events/IEvent.hpp"
#include "../Events/IEventData.hpp"
#include "../Events/EventType.hpp"

/*! A Script Event */
class ScriptEvent : public IEvent
{

public:

	virtual ~ScriptEvent( ) { };

	ScriptEvent( const std::string eventName, const std::string value1 = "", const std::string value2 = "" )
		: _eventName( eventName )
		, _value1( value1 )
		, _value2( value2 )
	{

	}

	/*! Returns the name of the event being passed to the Script */
	inline std::string GetEventName( ) { return _eventName; };

	/*! Returns the first value passed to the script */
	inline const std::string GetValue1( ) { return _value1; };

	/*! Returns the second value passed to the script */
	inline const std::string GetValue2( ) { return _value2; };

	/*! OBSOLETE */
	/*! Gets the Type of the Event */
	inline EventType GetEventType( ) const { return ALL_EVENTS; };

	/*! Returns Event Data attached to the event */
	inline IEventData* GetEventData( ) const { return 0; };

private:

	std::string _eventName;
	std::string _value1;
	std::string _value2;

	ScriptEvent( ) { };
	ScriptEvent( const ScriptEvent & copy ) { };
	ScriptEvent & operator = ( const ScriptEvent & copy ) { return *this; };

};

#endif
