#include "FileAppenderFixture.h"

#include "Logging/AppenderFactory.h"
#include "Logging/FileAppender.h"

CPPUNIT_TEST_SUITE_REGISTRATION( FileAppenderFixture );

void FileAppenderFixture::Should_Not_Initialize_On_File_Create_Error( )
{	
	FileAppender* fileAppender = static_cast< FileAppender* >( AppenderFactory::CreateAppender( FILEAPPENDER ) );
	bool result = fileAppender->Initialize( "../logs/readonly.log" );

	CPPUNIT_ASSERT( !result );

	delete fileAppender;
}

void FileAppenderFixture::Should_Initialize_Correctly( )
{
	FileAppender* fileAppender = static_cast< FileAppender* >( AppenderFactory::CreateAppender( FILEAPPENDER ) );
	bool result = fileAppender->Initialize( "../logs/test.log" );

	CPPUNIT_ASSERT( result );

	delete fileAppender;
}

void FileAppenderFixture::Should_Append_Message( )
{
	FileAppender* fileAppender = static_cast< FileAppender* >( AppenderFactory::CreateAppender( FILEAPPENDER ) );
	fileAppender->Initialize( "../logs/test.log" );

	bool result = fileAppender->Append( "Test Message" );

	CPPUNIT_ASSERT( result );

	delete fileAppender;
}
