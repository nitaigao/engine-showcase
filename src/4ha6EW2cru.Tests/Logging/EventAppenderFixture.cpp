#include "EventAppenderFixture.h"

#include "Logging/EventAppender.h"
#include "Exceptions/UnInitializedException.hpp"

#include "Events/EventManager.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( EventAppenderFixture, Suites::EventsSuite( ) );

void EventAppenderFixture::Should_Append_Message( )
{
	Logger::Initialize( );
	EventManager::Initialize( );
	EventAppender appender( EventManager::GetInstance( ) );
	appender.Append( "Event Appender Test" );
	EventManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}