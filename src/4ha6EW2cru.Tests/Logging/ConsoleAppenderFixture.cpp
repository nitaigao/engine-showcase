#include "ConsoleAppenderFixture.h"

#include "Logging/AppenderFactory.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ConsoleAppenderFixture, Suites::LoggingSuite( ) );

void ConsoleAppenderFixture::Should_Append_Message( )
{
	IAppender* consoleAppender = AppenderFactory::CreateAppender( CONSOLEAPPENDER );
	consoleAppender->Append( "Test Message" );

	delete consoleAppender;
}
