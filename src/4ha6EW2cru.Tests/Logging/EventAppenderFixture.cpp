#include "EventAppenderFixture.h"

#include "Logging/EventAppender.h"
#include "Exceptions/UnInitializedException.hpp"

#include "Events/EventManager.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( EventAppenderFixture, Suites::EventsSuite( ) );

void EventAppenderFixture::Should_Throw_Given_EventManager_Not_Initialized( )
{
	EventAppender appender;
	CPPUNIT_ASSERT_THROW( appender.Append( "blah" ), UnInitializedException );
}

void EventAppenderFixture::Should_Append_Message( )
{
	Logger::Initialize( );
	EventManager::Initialize( );
	EventAppender appender;
	appender.Append( "Event Appender Test" );
	EventManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}