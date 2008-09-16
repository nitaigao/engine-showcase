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

protected:

	typedef std::tr1::function< void ( const IEvent* ) > HandlerFunction;

public:

	Event( const EventType& eventType ) 
		: _eventType( eventType ) 
		, _eventData( 0 )
	{ 
	
	};

	Event( const EventType& eventType, IEventData* eventData )
		: _eventType( eventType )
		, _eventData( eventData )
	{ 
	
	};

	~Event( )
	{
		if ( _eventData != 0 )
		{
			delete _eventData;
		}
	}

	/*! Gets the Type of the Event */
	inline const EventType& GetEventType( ) const { return _eventType; };

	/*! Returns Event Data attached to the event */
	inline IEventData* GetEventData( ) const { return _eventData; };

private:

	Event( ) { };
	Event( const Event & copy ) { };
	Event & operator = ( const Event & copy ) { return *this; };

	EventType _eventType;
	IEventData* _eventData;

};

#endif