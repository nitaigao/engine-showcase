#include "EventManager.h"

#include "../Logging/Logger.h"
using namespace Logging;

#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/AlreadyInitializedException.hpp"
using namespace Exceptions;

namespace Events
{
	void EventManager::QueueEvent( const IEvent* event )
	{
		if ( 0 == event )
		{
			NullReferenceException e( "EventManager::QueueEvent - Attempted to add a NULL Event to the Queue" );
			Logger::Get( )->Fatal( e.what ( ) );
			throw e;
		}

		m_eventQueue.push( event );
	}

	void EventManager::TriggerEvent( const IEvent* event )
	{
		if ( 0 == event )
		{
			NullReferenceException e( "EventManager::TriggerEvent - Attempted to trigger a NULL Event" );
			Logger::Get( )->Fatal( e.what ( ) );
			throw e;
		}

		EventListenerList eventListeners( m_eventListeners );

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
		while( m_eventQueue.size( ) > 0 )
		{
			this->TriggerEvent( m_eventQueue.front( ) );
			m_eventQueue.pop( );
		}

		for ( EventListenerList::iterator i = m_eventListeners.begin( ); i != m_eventListeners.end( ); )
		{
			if ( ( *i )->IsMarkedForDeletion( ) )
			{
				delete ( *i );
				i = m_eventListeners.erase( i );
			}
			else
			{
				++i;
			}
		}
	}

	EventManager::~EventManager()
	{
		Info( "Releasing Event Manager" );

		while( m_eventQueue.size( ) > 0 )
		{
			delete m_eventQueue.front( );
			m_eventQueue.pop( );
		}

		for( EventListenerList::iterator i = m_eventListeners.begin( ); i != m_eventListeners.end( ); ++i )
		{
			delete ( *i );
		}

		m_eventListeners.clear( );
	}
}