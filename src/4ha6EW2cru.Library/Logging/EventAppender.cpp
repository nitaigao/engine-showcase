#include "EventAppender.h"

#include "../Events/Event.h"
#include "../Events/EventData.hpp"
#include "../Events/EventManager.h"

#include "../Exceptions/UnInitializedException.hpp"

void EventAppender::Append( const std::string message ) const
{
	AppenderEventData* eventData = new AppenderEventData( message );
	IEvent* event = new Event( LOG_MESSAGE_LOGGED, eventData );

	try
	{
		EventManager::GetInstance( )->QueueEvent( event );
	}
	catch ( UnInitializedException e )
	{
		delete event;
		throw;
	}
}