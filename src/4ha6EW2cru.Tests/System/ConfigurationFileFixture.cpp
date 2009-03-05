#include "ConfigurationFileFixture.h"

#include "Logging/Logger.h"
#include "IO/FileManager.h"

#include "Exceptions/OutOfRangeException.hpp"
#include "Exceptions/FileNotFoundException.hpp"
#include "Exceptions/FileWriteException.hpp"
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

void ConfigurationFileFixture::Should_Find_Default_Config_Item_On_FindConfigItemInt( )
{
	ConfigurationFile* config = ConfigurationFile::Load( "config/game.cfg" );
	int width = 0;
	width = config->FindConfigItem( "Display", "display_width", 100 );
	CPPUNIT_ASSERT( width > 0 );
	delete config;
}

void ConfigurationFileFixture::Should_Find_Default_Config_Item_On_FindConfigItemString( )
{
	ConfigurationFile* config = ConfigurationFile::Load( "config/game.cfg" );
	std::string result = "Hello World";
	std::string name = config->FindConfigItem( "Display", "blahhhh", result );
	CPPUNIT_ASSERT( name == result );
	delete config;
}

void ConfigurationFileFixture::Should_Find_Default_Config_Item_On_FindConfigItemBool( )
{
	ConfigurationFile* config = ConfigurationFile::Load( "config/game.cfg" );
	bool result = true;
	bool ok = config->FindConfigItem( "Display", "blahhhh", result );
	CPPUNIT_ASSERT( ok == result );
	delete config;
}

void ConfigurationFileFixture::Should_Find_Stored_Config_Item_On_FindConfigItemBool( )
{
	ConfigurationFile* config = ConfigurationFile::Load( "test/test.cfg" );
	bool ok = config->FindConfigItem( "Display", "fullscreen", false );
	CPPUNIT_ASSERT( ok == true );
	delete config;
}

void ConfigurationFileFixture::Should_Throw_On_Save_Given_File_UnWritable( )
{
	ConfigurationFile* config = ConfigurationFile::Load( "config/game.cfg" );
	CPPUNIT_ASSERT_THROW( config->Save( "lsdjklasdjaskldjlkas/asdas/das/d/asd/as/das/da" ), FileWriteException );
	delete config;
}

void ConfigurationFileFixture::Should_Save_Correctly_Given_File_Writable( )
{
	ConfigurationFile* config = ConfigurationFile::Load( "config/game.cfg" );
	config->Save( "config/game.cfg" );
	delete config;
}

void ConfigurationFileFixture::Should_Update_Or_Add_Key( )
{
	ConfigurationFile* config = ConfigurationFile::Load( "config/game.cfg" );
	config->Update( "TestSection", "test_key", "testing" );
	config->Save( "config/game.cfg" );
	delete config;
}
