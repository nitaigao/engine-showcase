#ifndef __CONFIGURATIONFILE_FIXTURE_H
#define __CONFIGURATIONFILE_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class ConfigurationFileFixture : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( ConfigurationFileFixture );
	CPPUNIT_TEST( Should_Throw_On_Load_Given_Invalid_FilePath );
	CPPUNIT_TEST( Should_Populate_Configuration_On_Load_Given_Valid_FilePath );
	CPPUNIT_TEST( Should_Find_Config_Item_On_FindConfigItemInt );
	CPPUNIT_TEST( Should_Find_Config_Item_On_FindConfigItemString );
	CPPUNIT_TEST( Should_Find_Config_Item_On_FindConfigItemBool );
	CPPUNIT_TEST( Should_Throw_On_Save_Given_File_UnWritable );
	CPPUNIT_TEST( Should_Save_Correctly_Given_File_Writable );
	CPPUNIT_TEST( Should_Update_Or_Add_Key );
	
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp ( void );
	void tearDown ( void );

private:

	void Should_Throw_On_Load_Given_Invalid_FilePath( );
	void Should_Populate_Configuration_On_Load_Given_Valid_FilePath( );
	void Should_Find_Config_Item_On_FindConfigItemInt( );
	void Should_Find_Config_Item_On_FindConfigItemString( );
	void Should_Find_Config_Item_On_FindConfigItemBool( );
	void Should_Throw_On_Save_Given_File_UnWritable( );
	void Should_Save_Correctly_Given_File_Writable( );
	void Should_Update_Or_Add_Key( );

};

#endif