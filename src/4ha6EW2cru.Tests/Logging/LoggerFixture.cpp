#include "LoggerFixture.h"

#include "Logging/Logger.h"
#include "Logging/ConsoleAppender.h"

#include "../Mocks/MockLogAppender.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION( LoggerFixture );

void LoggerFixture::Should_Initialize_And_Release_Correctly( )
{
	bool result = Logger::Initialize( );

	CPPUNIT_ASSERT( result );
	CPPUNIT_ASSERT( Logger::GetInstance( ) != 0 );

	Logger::GetInstance( )->Release( );
}

void LoggerFixture::Should_Log_To_Appender( )
{
	MockLogAppender* appenderMock = new MockLogAppender( );	
	appenderMock->append_count.setExpected( 1 );

	Logger::Initialize( );
	Logger::GetInstance( )->AddAppender( appenderMock );
	Logger::GetInstance( )->Debug( "Logger Appender Test" );
	
	appenderMock->verify( );
	Logger::GetInstance( )->Release( );
}

void LoggerFixture::Should_Throw_On_UnInitialized_GetInstance( )
{
	Logger::GetInstance( );
}

void LoggerFixture::Should_Throw_On_Initialize_Given_Already_Intialized( )
{
	Logger::Initialize( );

	CPPUNIT_ASSERT_THROW( 
		Logger::Initialize( ),
		AlreadyInitializedException
		);

	Logger::GetInstance( )->Release( );
}