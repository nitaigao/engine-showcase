#ifndef __EVENT_H
#define __EVENT_H

#include "IEvent.hpp"
#include "IEventData.hpp"
#include "EventType.hpp"

/*!
	A Game Event
*/
class Event : public IEvent
{

public:

	virtual ~Event( )
	{
		if ( _eventData != 0 )
		{
			delete _eventData;
		}
	}

	Event( const EventType& eventType, IEventData* eventData = 0 )
		: _eventType( eventType )
		, _eventData( eventData )
	{

	};

	/*! Gets the Type of the Event */
	inline const EventType& GetEventType( ) const { return _eventType; };

	/*! Returns Event Data attached to the event */
	inline IEventData* GetEventData( ) const { return _eventData; };

private:

	EventType _eventType;
	IEventData* _eventData;

	Event( ) { };
	Event( const Event & copy ) { };
	Event & operator = ( const Event & copy ) { return *this; };

};



#endif
