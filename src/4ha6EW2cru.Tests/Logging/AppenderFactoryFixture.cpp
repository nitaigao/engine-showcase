#include "AppenderFactoryFixture.h"

#include "Logging/AppenderFactory.h"
#include "Logging/AppenderType.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION( AppenderFactoryFixture );

void AppenderFactoryFixture::Should_Return_Correct_Type( )
{
	IAppender* consoleAppender = AppenderFactory::CreateAppender( CONSOLEAPPENDER );
	CPPUNIT_ASSERT( consoleAppender != 0 );
	delete consoleAppender;

	IAppender* fileAppender = AppenderFactory::CreateAppender( FILEAPPENDER );
	CPPUNIT_ASSERT( fileAppender != 0 );
	delete fileAppender;
}