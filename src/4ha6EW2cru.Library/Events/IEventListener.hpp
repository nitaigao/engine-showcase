#ifndef __IEVENTLISTENER_H
#define __IEVENTLISTENER_H

#include <boost/function.hpp>

#include "EventType.hpp"

/*! 
	Interface that listenens for Events and handles the appropriate action 
*/
class IEventListener
{

public:

	virtual ~IEventListener( ) { };

	/*! Handles the Event that is being listened for */
	virtual void HandleEvent( const IEvent* ) const = 0;

	/*! Gets the EventType that is being listened for */
	virtual inline const EventType GetEventType( ) const = 0;

};

#endif