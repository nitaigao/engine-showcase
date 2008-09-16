#include "ConsoleAppenderFixture.h"

#include "Logging/AppenderFactory.h"

CPPUNIT_TEST_SUITE_REGISTRATION( ConsoleAppenderFixture );

void ConsoleAppenderFixture::Should_Append_Message( )
{
	IAppender* consoleAppender = AppenderFactory::CreateAppender( CONSOLEAPPENDER );
	consoleAppender->Append( "Test Message" );

	delete consoleAppender;
}
