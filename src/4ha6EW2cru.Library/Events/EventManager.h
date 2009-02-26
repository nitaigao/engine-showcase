#ifndef __EVENTMANAGER_H
#define __EVENTMANAGER_H

#include <queue>
#include <map>

#include "IEvent.hpp"
#include "EventListener.h"
#include "IEventListener.hpp"

/*!
	Manages all Events distributed thoughout the Game
*/
class EventManager 
{

	typedef std::queue< const IEvent* > EventQueue;
	typedef std::vector< IEventListener* > EventListenerList;

public:

	/*! Retrieves an instance of the EventManager Singleton */
	static EventManager* GetInstance( );

	/*! Releases all resources and the EventManager Singleton */
	void Release( );

	/*! Initializes the Event Managment mechanism */
	static bool Initialize( );

	/*! Queues an Event for processing on the next Tick */
	void QueueEvent( const IEvent* event );

	/*! Processes an Event immediately */
	void TriggerEvent( const IEvent* event );

	/*! Fires all events in the Event Queue */
	void Update( );

	/*! Adds an EventListener for Event processing */
	template< class AT >
	void AddEventListener( const EventType eventType, AT* handlerTarget, void ( AT::*handlerFunctor ) ( const IEvent* event ) )
	{
		if ( 0 == handlerTarget )
		{
			NullReferenceException nullTarget( "EventManager::AddEventListener - Event Target is NULL" );
			Logger::GetInstance( )->Fatal( nullTarget.what( ) );
			throw nullTarget;
		}

		if ( 0 == handlerFunctor )
		{
			NullReferenceException nullFunctor( "EventManager::AddEventListener - Handler Functor is NULL" );
			Logger::GetInstance( )->Fatal( nullFunctor.what( ) );
			throw nullFunctor;
		}

		EventListener< AT >* eventListener = new EventListener< AT >( eventType, handlerTarget, handlerFunctor );
		_eventListeners.push_back( eventListener );
	}

	/*! Removed an EventListener and destroys it */
	template< class RT >
	void RemoveEventListener( const EventType eventType, RT* handlerTarget, void ( RT::*handlerFunctor ) ( const IEvent* event ) )
	{
		if ( 0 == handlerTarget )
		{
			NullReferenceException nullTarget( "EventManager::RemoveEventListener - Event Target is NULL" );
			Logger::GetInstance( )->Fatal( nullTarget.what( ) );
			throw nullTarget;
		}

		if ( 0 == handlerFunctor )
		{
			NullReferenceException nullFunctor( "EventManager::RemoveEventListener - Handler Functor is NULL" );
			Logger::GetInstance( )->Fatal( nullFunctor.what( ) );
			throw nullFunctor;
		}

		for ( EventListenerList::iterator i = _eventListeners.begin( ); i != _eventListeners.end( ); ++i )
		{
			EventListener< RT >* eventListener = static_cast< EventListener< RT >* >( ( *i ) );

			if (
				handlerFunctor == eventListener->GetHandlerFunctor( ) &&
				eventType == eventListener->GetEventType( ) &&
				handlerTarget == eventListener->GetHandlerTarget( )
				)
			{
				delete ( *i );
				_eventListeners.erase( i );
				return;
			}
		}
	}

private:

	EventManager( ) { };
	~EventManager( ) { };
	EventManager( const EventManager & copy ) { };
	EventManager & operator = ( const EventManager & copy ) { return *this; };

	EventQueue _eventQueue;
	EventListenerList _eventListeners;

};

#endif
