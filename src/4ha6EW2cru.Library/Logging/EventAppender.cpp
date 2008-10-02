#include "EventAppender.h"

#include "../Events/Event.h"
#include "../Events/EventData.hpp"
#include "../Events/EventManager.h"

#include "../Exceptions/UnInitializedException.hpp"

void EventAppender::Append( const std::string message ) const
{
	if ( _eventManager != 0 )
	{
		AppenderEventData* eventData = new AppenderEventData( message );
		IEvent* event = new Event( LOG_MESSAGE_APPENDED, eventData );
		_eventManager->QueueEvent( event );
	}
}