#ifndef __IEVENT_H
#define __IEVENT_H

#include "EventType.hpp"
#include "IEventData.hpp"

/*!
	Interface for all Events
*/
class IEvent
{

public:

	virtual ~IEvent( ) { };

	/*! Gets the type of the Event */
	virtual EventType GetEventType( ) const = 0;

	/*! Returns Event Data attached to the event */
	virtual IEventData* GetEventData( ) const = 0;

};

#endif
