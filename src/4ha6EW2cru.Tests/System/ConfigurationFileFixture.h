#ifndef __CONFIGURATIONFILE_FIXTURE_H
#define __CONFIGURATIONFILE_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class ConfigurationFileFixture : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( ConfigurationFileFixture );
	CPPUNIT_TEST( Should_Throw_On_Load_Given_Invalid_FilePath );
	CPPUNIT_TEST( Should_Populate_Configuration_On_Load_Given_Valid_FilePath );
	CPPUNIT_TEST( Should_Throw_On_FindConfigItem_Given_InValid_Key );
	CPPUNIT_TEST( Should_Find_Config_Item_On_FindConfigItem_Given_Valid_Key );
	/*CPPUNIT_TEST( Should_Add_Config_Item_On_AddConfigItem_Given_Valid_Key_Value_Pair );*/
	
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp ( void );
	void tearDown ( void );

private:

	void Should_Throw_On_Load_Given_Invalid_FilePath( );
	void Should_Populate_Configuration_On_Load_Given_Valid_FilePath( );
	void Should_Throw_On_FindConfigItem_Given_InValid_Key( );
	void Should_Find_Config_Item_On_FindConfigItem_Given_Valid_Key( );
	/*void Should_Add_Config_Item_On_AddConfigItem_Given_Valid_Key_Value_Pair( );*/


};

#endif