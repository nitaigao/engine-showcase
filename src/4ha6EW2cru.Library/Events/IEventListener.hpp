#ifndef __IEVENTLISTENER_H
#define __IEVENTLISTENER_H

#include <boost/function.hpp>

#include "IEvent.hpp"
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

	/*! Marks the EventHandler for Deletion on the Next Update */
	virtual inline void MarkForDeletion( ) = 0;

	/*! Gets whether the EventHandler is marked for deletion */
	virtual inline bool IsMarkedForDeletion( ) const = 0;

};

#endif
