#include "ConsoleAppenderFixture.h"

#include "Logging/IAppender.hpp"
#include "Logging/ConsoleAppender.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ConsoleAppenderFixture, Suites::LoggingSuite( ) );

void ConsoleAppenderFixture::Should_Append_Message( )
{
	IAppender* consoleAppender = new ConsoleAppender( );
	consoleAppender->Append( "Test Message" );

	delete consoleAppender;
}
