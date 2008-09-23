#include "FileAppenderFixture.h"

#include "Logging/AppenderFactory.h"
#include "Logging/FileAppender.h"

#include "Exceptions/FileWriteException.hpp"
#include "Exceptions/AlreadyInitializedException.hpp"

#include "Logging/Logger.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( FileAppenderFixture, Suites::LoggingSuite( ) );

void FileAppenderFixture::setUp( )
{
	Logger::Initialize( );
}

void FileAppenderFixture::tearDown( )
{
	Logger::GetInstance( )->Release( );
}

void FileAppenderFixture::Should_Append_Message( )
{
	FileAppender* fileAppender = static_cast< FileAppender* >( AppenderFactory::CreateAppender( FILEAPPENDER ) );
	fileAppender->Initialize( "../logs/test.log" );

	fileAppender->Append( "Test Message" );

	delete fileAppender;
}


void FileAppenderFixture::Should_Throw_On_Initialize_Given_File_Create_Error( )
{
	FileAppender* fileAppender = static_cast< FileAppender* >( AppenderFactory::CreateAppender( FILEAPPENDER ) );
	
	CPPUNIT_ASSERT_THROW( fileAppender->Initialize( "../logs/readonly.log" ), FileWriteException );

	delete fileAppender;
}

void FileAppenderFixture::Should_Initialize_Correctly_Given_File_Can_Be_Creaed_Or_Appended( )
{
	FileAppender* fileAppender = static_cast< FileAppender* >( AppenderFactory::CreateAppender( FILEAPPENDER ) );
	
	fileAppender->Initialize( "../logs/test.log" );

	delete fileAppender;
}

void FileAppenderFixture::Should_Throw_On_Intialized_Given_Already_Intialized( )
{
	FileAppender* fileAppender = static_cast< FileAppender* >( AppenderFactory::CreateAppender( FILEAPPENDER ) );

	fileAppender->Initialize( "../logs/test.log" );
	
	CPPUNIT_ASSERT_THROW( fileAppender->Initialize( "../logs/test.log" ), AlreadyInitializedException );

	delete fileAppender;
}

void FileAppenderFixture::Should_Throw_On_Append_Given_Append_Error( )
{

}