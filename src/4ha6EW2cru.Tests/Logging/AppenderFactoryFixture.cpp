#include "AppenderFactoryFixture.h"

#include "Logging/AppenderFactory.h"
#include "Logging/AppenderType.hpp"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( AppenderFactoryFixture, Suites::LoggingSuite( ) );

void AppenderFactoryFixture::Should_Return_ConsoleAppender( )
{
	IAppender* consoleAppender = AppenderFactory::CreateAppender( CONSOLEAPPENDER );
	CPPUNIT_ASSERT( consoleAppender != 0 );
	delete consoleAppender;
}

void AppenderFactoryFixture::Should_Return_FileAppender( )
{
	IAppender* fileAppender = AppenderFactory::CreateAppender( FILEAPPENDER );
	CPPUNIT_ASSERT( fileAppender != 0 );
	delete fileAppender;
}

void AppenderFactoryFixture::Should_Return_EventAppender( )
{
	IAppender* eventAppender = AppenderFactory::CreateAppender( EVENTAPPENDER );
	CPPUNIT_ASSERT( eventAppender != 0 );
	delete eventAppender;
}