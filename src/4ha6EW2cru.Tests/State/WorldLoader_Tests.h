#ifndef WORLDLOADER_TESTS_H
#define WORLDLOADER_TESTS_H

#include <cppunit/extensions/HelperMacros.h>

class WorldLoader_Tests : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( WorldLoader_Tests );
	CPPUNIT_TEST( Should_Call_FileManager_On_Load_To_Load_Level_File );
	CPPUNIT_TEST( Should_Load_And_Parse_A_YAML_File );
	CPPUNIT_TEST_SUITE_END( );

protected:

	void Should_Call_FileManager_On_Load_To_Load_Level_File( );
	void Should_Load_And_Parse_A_YAML_File( );


};

#endif