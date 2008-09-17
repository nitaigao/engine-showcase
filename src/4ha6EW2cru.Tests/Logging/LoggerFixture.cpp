#include "LoggerFixture.h"

#include "Logging/Logger.h"
#include "Logging/ConsoleAppender.h"

#include "../Mocks/MockLogAppender.hpp"

#include "Exceptions/NullReferenceException.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION( LoggerFixture );

void LoggerFixture::Should_Initialize_Correctly( )
{
	Logger::Initialize(	);
	Logger::GetInstance( )->Release( );
}

void LoggerFixture::Should_Throw_On_Initialize_Given_Already_Intialized( )
{
	Logger::Initialize(	);
	CPPUNIT_ASSERT_THROW( Logger::Initialize( ), AlreadyInitializedException );
	Logger::GetInstance( )->Release( );
}

void LoggerFixture::Should_Release_Given_Initialized( )
{
	Logger::Initialize(	);
	Logger::GetInstance( )->Release( );
}

void LoggerFixture::Should_Throw_On_Release_Given_ALready_Released( )
{
	Logger::Initialize(	);
	Logger::GetInstance( )->Release( );
	CPPUNIT_ASSERT_THROW( Logger::GetInstance( )->Release( ), UnInitializedException );
}

void LoggerFixture::Should_GetIntance_Given_Initialized( )
{
	Logger::Initialize(	);
	Logger::GetInstance( );
	Logger::GetInstance( )->Release( );
}

void LoggerFixture::Should_Throw_On_GetInstance_Given_Not_Initialized( )
{
	CPPUNIT_ASSERT_THROW( Logger::GetInstance( ), UnInitializedException );
}

void LoggerFixture::Should_Add_Appender_Given_Intialized_And_Valid_Appender( )
{
	MockLogAppender* appender = new MockLogAppender( );	
	
	Logger::Initialize( );
	Logger::GetInstance( )->AddAppender( appender );
	Logger::GetInstance( )->Release( );
}

void LoggerFixture::Should_Throw_On_Add_Appender_Given_Not_Initialized( )
{
	MockLogAppender* appender = new MockLogAppender( );	

	CPPUNIT_ASSERT_THROW( Logger::GetInstance( )->AddAppender( appender ), UnInitializedException );
	
	delete appender;
}

void LoggerFixture::Should_Throw_On_Add_Appender_Given_NULL_Appender( )
{
	Logger::Initialize( );
	CPPUNIT_ASSERT_THROW( Logger::GetInstance( )->AddAppender( 0 ), NullReferenceException );
	Logger::GetInstance( )->Release( );
}

void LoggerFixture::Should_Throw_On_Add_Appender_Given_Appender_Already_Added( )
{
	MockLogAppender* appender = new MockLogAppender( );	

	Logger::Initialize( );
	Logger::GetInstance( )->AddAppender( appender );
	CPPUNIT_ASSERT_THROW( Logger::GetInstance( )->AddAppender( appender ), AlreadyInitializedException );
	Logger::GetInstance( )->Release( );
}

void LoggerFixture::Should_Log_On_Debug_Given_Valid_Message( )
{
	Logger::Initialize( );
	Logger::GetInstance( )->Debug( "Debug Message" );
	Logger::GetInstance( )->Release( );
}

void LoggerFixture::Should_Log_On_Info_Given_Valid_Message( )
{
	Logger::Initialize( );
	Logger::GetInstance( )->Info( "Info Message" );
	Logger::GetInstance( )->Release( );
}

void LoggerFixture::Should_Log_On_Fatal_Given_Valid_Message( )
{
	Logger::Initialize( );
	Logger::GetInstance( )->Fatal( "Fatal Message" );
	Logger::GetInstance( )->Release( );
}

void LoggerFixture::Should_Log_To_Appender( )
{
	MockLogAppender* appenderMock = new MockLogAppender( );	
	appenderMock->append_count.setExpected( 3 );

	Logger::Initialize( );
	Logger::GetInstance( )->AddAppender( appenderMock );
	Logger::GetInstance( )->Debug( "Logger Appender Test" );
	Logger::GetInstance( )->Info( "Logger Appender Test" );
	Logger::GetInstance( )->Fatal( "Logger Appender Test" );

	appenderMock->verify( );
	Logger::GetInstance( )->Release( );
}
