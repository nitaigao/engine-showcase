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

void ConfigurationFixture::Should_Return_IsFullScreen_True_Given_FullScreen_Set_To_True( )
{
	bool isFullScreen = true;

	Configuration* config = Configuration::Create( );
	config->SetFullScreen( isFullScreen );

	CPPUNIT_ASSERT( config->IsFullScreen( ) == isFullScreen );

	delete config;
};

void ConfigurationFixture::Should_Return_IsFullScreen_False_Given_FullScreen_Set_To_False( )
{
	bool isFullScreen = false;

	Configuration* config = Configuration::Create( );
	config->SetFullScreen( isFullScreen );

	CPPUNIT_ASSERT( config->IsFullScreen( ) == isFullScreen );

	delete config;
};