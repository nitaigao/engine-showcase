#include "EventManager.h"

#include "../Logging/Logger.h"
using namespace Logging;

#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/AlreadyInitializedException.hpp"
namespace Events
{
	void EventManager::QueueEvent( const IEvent* event )
	{
		if ( 0 == event )
		{
			NullReferenceException e( "EventManager::QueueEvent - Attempted to add a NULL Event to the Queue" );
			Logger::Fatal( e.what ( ) );
			throw e;
		}

		_eventQueue.push( event );
	}

	void EventManager::TriggerEvent( const IEvent* event )
	{
		if ( 0 == event )
		{
			NullReferenceException e( "EventManager::TriggerEvent - Attempted to trigger a NULL Event" );
			Logger::Fatal( e.what ( ) );
			throw e;
		}

		EventListenerList eventListeners( _eventListeners );
		for ( EventListenerList::iterator i = eventListeners.begin( ); i != eventListeners.end( ); ++i )
		{
			if ( 
				( *i )->GetEventType( ) == event->GetEventType( ) && !( *i )->IsMarkedForDeletion( ) || 
				( *i )->GetEventType( ) == ALL_EVENTS && !( *i )->IsMarkedForDeletion( ) 
				)
			{
				( *i )->HandleEvent( event );
			}
		}

		delete event;
	}

	void EventManager::Update( const float& deltaMilliseconds )
	{
		while( _eventQueue.size( ) > 0 )
		{
			this->TriggerEvent( _eventQueue.front( ) );
			_eventQueue.pop( );
		}

		for ( EventListenerList::iterator i = _eventListeners.begin( ); i != _eventListeners.end( ); )
		{
			if ( ( *i )->IsMarkedForDeletion( ) )
			{
				delete ( *i );
				i = _eventListeners.erase( i );
			}
			else
			{
				++i;
			}
		}
	}

	EventManager::~EventManager()
	{
		Logger::Info( "EventManager::Release - Releasing Event Manager" );

		while( _eventQueue.size( ) > 0 )
		{
			delete _eventQueue.front( );
			_eventQueue.pop( );
		}

		for( EventListenerList::iterator i = _eventListeners.begin( ); i != _eventListeners.end( ); ++i )
		{
			delete ( *i );
		}

		_eventListeners.clear( );
	}
}