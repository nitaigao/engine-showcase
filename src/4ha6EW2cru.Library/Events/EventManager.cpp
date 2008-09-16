#include "EventManager.h"
#include "../Logging/Logger.h"
#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/AlreadyInitializedException.hpp"

static EventManager* g_EventManagerInstance = 0;

EventManager* EventManager::GetInstance( )
{
	if ( g_EventManagerInstance == 0 )
	{
		throw UnInitializedException( "EventManager");
	}

	return g_EventManagerInstance;
}

void EventManager::Release( )
{
	while( _eventQueue.size( ) > 0 )
	{
		delete _eventQueue.front( );
		_eventQueue.pop( );
	}

	for( EventListenerList::iterator i = _eventListeners.begin( ); i != _eventListeners.end( ); ++i )
	{
		delete ( *i ).second;
	}

	_eventListeners.erase( _eventListeners.begin( ), _eventListeners.end( ) );

	Logger::GetInstance( )->Info( "Releasing Event Manager" );

	delete g_EventManagerInstance;
	g_EventManagerInstance = 0;
}

bool EventManager::Initialize( )
{
	Logger::GetInstance( )->Info( "Initializing Event Manager" );

	if ( g_EventManagerInstance != 0 )
	{
		std::string errorMessage = "EventManager has already been Intialized";
		Logger::GetInstance( )->Fatal( errorMessage );
		throw AlreadyInitializedException( errorMessage );
	}

	g_EventManagerInstance = new EventManager( );

	return true;	
}

void EventManager::QueueEvent( const IEvent* event )
{
	if ( !event )
	{
		Logger::GetInstance( )->Fatal( "Attempted to add a NULL Event to the Event Queue" );
		return;
	}

	_eventQueue.push( event );
}

void EventManager::Update( )
{
	std::pair< 
			EventListenerList::iterator, 
			EventListenerList::iterator
		> listeners;

	std::multimap< const EventType, IEventListener* > eventListeners( _eventListeners );

	while( _eventQueue.size( ) > 0 )
	{
		int eventType = _eventQueue.front( )->GetEventType( );

		listeners = eventListeners.equal_range( ( EventType ) eventType ); 

		for ( EventListenerList::iterator i = listeners.first; i != listeners.second; ++i )
		{
			( *i ).second->HandleEvent( _eventQueue.front( ) );
		}

		delete _eventQueue.front( );
		_eventQueue.pop( );
	}
}