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

void ConfigurationFixture::Should_Throw_On_Load_Given_Invalid_FilePath( )
{
	CPPUNIT_ASSERT_THROW( Configuration::Load( "blah.cfg" ), FileNotFoundException );
}

void ConfigurationFixture::Should_Populate_Configuration_On_Load_Given_Valid_FilePath( )
{
	Configuration* config = Configuration::Load( "config/game.cfg" );

	delete config;
}

void ConfigurationFixture::Should_Add_Config_Item_On_AddConfigItem_Given_Valid_Key_Value_Pair( )
{
	Configuration* config = Configuration::Create( );
	config->AddConfigItem( "test", "test" );

	delete config;
}

void ConfigurationFixture::Should_Find_Config_Item_On_FindConfigItem_Given_Valid_Key( )
{
	Configuration* config = Configuration::Create( );
	std::string key = "test_key";
	std::string value = "test_value";

	config->AddConfigItem( key, value );
	std::string result = config->FindConfigItemString( key );

	CPPUNIT_ASSERT( value == result ); 

	delete config;
}

void ConfigurationFixture::Should_Throw_On_FindConfigItem_Given_InValid_Key( )
{	
	Configuration* config = Configuration::Create( );
	CPPUNIT_ASSERT_THROW( config->FindConfigItemString( "blah" ), OutOfRangeException );
	delete config;
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