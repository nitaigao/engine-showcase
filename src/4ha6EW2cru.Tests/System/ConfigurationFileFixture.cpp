#include "ConfigurationFileFixture.h"

#include "Logging/Logger.h"
#include "IO/FileManager.h"

#include "Exceptions/OutOfRangeException.hpp"
#include "Exceptions/FileNotFoundException.hpp"
#include "System/ConfigurationFile.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ConfigurationFileFixture, Suites::SystemSuite( ) );

void ConfigurationFileFixture::setUp( )
{
	Logger::Initialize( );
	FileManager::Initialize( );
}

void ConfigurationFileFixture::tearDown( )
{
	FileManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void ConfigurationFileFixture::Should_Throw_On_Load_Given_Invalid_FilePath( )
{
	CPPUNIT_ASSERT_THROW( ConfigurationFile::Load( "blah.cfg" ), FileNotFoundException );
}

void ConfigurationFileFixture::Should_Populate_Configuration_On_Load_Given_Valid_FilePath( )
{
	ConfigurationFile* config = ConfigurationFile::Load( "config/game.cfg" );

	CPPUNIT_ASSERT( config != 0 );

	delete config;
}

void ConfigurationFileFixture::Should_Throw_On_FindConfigItem_Given_InValid_Key( )
{
	ConfigurationFile* config = ConfigurationFile::Load( "config/game.cfg" );
	CPPUNIT_ASSERT_THROW( config->FindConfigItem< int >( "blah" ), OutOfRangeException );
	delete config;
}

void ConfigurationFileFixture::Should_Find_Config_Item_On_FindConfigItem_Given_Valid_Key( )
{
	ConfigurationFile* config = ConfigurationFile::Load( "config/game.cfg" );
	int width = 0;
	width = config->FindConfigItem< int >( "Display.display_width" );
	CPPUNIT_ASSERT( width > 0 );
	delete config;
}

/*
void ConfigurationFileFixture::Should_Add_Config_Item_On_AddConfigItem_Given_Valid_Key_Value_Pair( )
{
	ConfigurationFile* config = ConfigurationFile::Create( );
	config->AddConfigItem( "test", "test" );

	delete config;
}

void ConfigurationFileFixture::Should_Find_Config_Item_On_FindConfigItem_Given_Valid_Key( )
{
	ConfigurationFile* config = ConfigurationFile::Create( );
	std::string key = "test_key";
	std::string value = "test_value";

	config->AddConfigItem( key, value );
	std::string result = config->FindConfigItemString( key );

	CPPUNIT_ASSERT( value == result ); 

	delete config;
}

void ConfigurationFileFixture::Should_Throw_On_FindConfigItem_Given_InValid_Key( )
{	
	ConfigurationFile* config = ConfigurationFile::Create( );
	CPPUNIT_ASSERT_THROW( config->FindConfigItemString( "blah" ), OutOfRangeException );
	delete config;
}*/