#ifndef __EVENTLISTENER_H
#define __EVENTLISTENER_H

#include <functional>

#include "../Logging/Logger.h"
#include "../Exceptions/NullReferenceException.hpp"

#include "IEvent.hpp"
#include "EventType.hpp"
#include "IEventListener.hpp"

/*! 
	Listener for Events and handles the appropriate action 
*/
template< class T >
class EventListener : public IEventListener
{

	typedef void ( T::*HandlerFunctor ) ( const IEvent* event ) ;

public:

	EventListener( const EventType eventType, T* const handlerTarget, HandlerFunctor handlerFunctor )
		: _handlerFunctor( handlerFunctor )
		, _eventType( eventType )
		, _handlerTarget( handlerTarget )
	{

	}

	/*! Passes the incoming event function handling it */
	void HandleEvent( const IEvent* event ) const 
	{
		if ( _handlerTarget == 0 )
		{
			std::string message = "EventListener::HandleEvent - HandlerTarget is NULL";
			throw NullReferenceException( message );
			Logger::GetInstance( )->Fatal( message );
		}

		if ( _handlerFunctor == 0 )
		{
			std::string message = "EventListener::HandleEvent - HandlerFunctor is NULL";
			throw NullReferenceException( message );
			Logger::GetInstance( )->Fatal( message );
		}

		( _handlerTarget->*_handlerFunctor )( event );
	}

	/*! Retrieves the Type of Event the EventListener is listening for */
	inline const EventType GetEventType( ) const { return _eventType; };

	/*! Returns a pointer to the method assigned to handle the Event */
	inline const HandlerFunctor GetHandlerFunctor( ) const { return _handlerFunctor; };

	/*! Retrieves the object instance for which the EventListener is Listening */
	inline const T* GetHandlerTarget( ) const { return _handlerTarget; };

private:

	HandlerFunctor _handlerFunctor;
	EventType _eventType;
	T* _handlerTarget;

};

#endif