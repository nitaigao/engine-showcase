#ifndef __LEVELLOADER_TESTS_H
#define __LEVELLOADER_TESTS_H

#include <cppunit/extensions/HelperMacros.h>

class LevelLoader_Tests : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( LevelLoader_Tests );

	CPPUNIT_TEST( Should_Call_World_AddEntity_On_LoadLevel );

	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Call_World_AddEntity_On_LoadLevel( );

};

#endif