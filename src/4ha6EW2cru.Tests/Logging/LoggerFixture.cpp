#include "LoggerFixture.h"

#include "Logging/Logger.h"
#include "Logging/ConsoleAppender.h"

#include "Management/Management.h"

#include "../Mocks/MockLogAppender.hpp"

#include "Exceptions/NullReferenceException.hpp"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( LoggerFixture, Suites::LoggingSuite( ) );

void LoggerFixture::setUp()
{

}

void LoggerFixture::tearDown()
{

}

void LoggerFixture::Should_Initialize_Correctly( )
{
	Logger::Initialize( );
	Logger::GetInstance( )->Release( );
}

void LoggerFixture::Should_Throw_On_Initialize_Given_Already_Intialized( )
{
	Logger::Initialize( );
	CPPUNIT_ASSERT_THROW( Logger::Initialize( ), AlreadyInitializedException );
	Logger::GetInstance( )->Release( );
}

void LoggerFixture::Should_Release_Given_Initialized( )
{
	Logger::Initialize( );
	Logger::GetInstance( )->Release( );
}

void LoggerFixture::Should_Throw_On_Release_Given_ALready_Released( )
{
	Logger::Initialize( );
	Logger::GetInstance( )->Release( );
	CPPUNIT_ASSERT_THROW( Logger::GetInstance( )->Release( ), UnInitializedException );
}

void LoggerFixture::Should_GetIntance_Given_Initialized( )
{
	Logger::Initialize( );
	Logger::GetInstance( );
	Logger::GetInstance( )->Release( );
}

void LoggerFixture::Should_Throw_On_GetInstance_Given_Not_Initialized( )
{
	CPPUNIT_ASSERT_THROW( Logger::GetInstance( ), UnInitializedException );
}

void LoggerFixture::Should_Log_On_Debug_Given_Valid_Message( )
{
	Logger::Initialize( );
	Management::Initialize( );
	Logger::GetInstance( )->Debug( "Debug Message" );
	Management::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void LoggerFixture::Should_Log_On_Info_Given_Valid_Message( )
{
	Logger::Initialize( );
	Management::Initialize( );
	Logger::GetInstance( )->Info( "Info Message" );
	Management::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void LoggerFixture::Should_Log_On_Fatal_Given_Valid_Message( )
{
	Logger::Initialize( );
	Management::Initialize( );
	Logger::GetInstance( )->Fatal( "Fatal Message" );
	Management::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}