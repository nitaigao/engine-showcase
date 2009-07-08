#include "LoggerFixture.h"

#include "Logging/Logger.h"
using namespace Logging;

#include "Management/Management.h"

#include "Exceptions/NullReferenceException.hpp"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( LoggerFixture, Suites::LoggingSuite( ) );

void LoggerFixture::setUp()
{

}

void LoggerFixture::tearDown()
{

}



void LoggerFixture::Should_Log_On_Debug_Given_Valid_Message( )
{
	Management::Initialize( );
	Logger::Get( )->Debug( "Debug Message" );
	Management::Release( );
}

void LoggerFixture::Should_Log_On_Info_Given_Valid_Message( )
{
	Management::Initialize( );
	Logger::Get( )->Info( "Info Message" );
	Management::Release( );
}

void LoggerFixture::Should_Log_On_Fatal_Given_Valid_Message( )
{
	Management::Initialize( );
	Logger::Get( )->Fatal( "Fatal Message" );
	Management::Release( );
}