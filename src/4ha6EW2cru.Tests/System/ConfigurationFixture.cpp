#include "ConfigurationFixture.h"

#include "Logging/Logger.h"
#include "IO/FileManager.h"

#include "Exceptions/OutOfRangeException.hpp"
#include "Exceptions/FileNotFoundException.hpp"
#include "System/Configuration.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ConfigurationFixture, Suites::SystemSuite( ) );

void ConfigurationFixture::setUp( )
{
	Logger::Initialize( );
	FileManager::Initialize( );
}

void ConfigurationFixture::tearDown( )
{
	FileManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}