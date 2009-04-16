#ifndef __SCRIPT_SYSTEM_TESTS
#define __SCRIPT_SYSTEM_TESTS

#include <cppunit/extensions/HelperMacros.h>

class ScriptSystem_Tests : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( ScriptSystem_Tests );
	CPPUNIT_TEST( Should_Initialize_Correctly );
	CPPUNIT_TEST( Should_Return_Scene_On_CreateScene );
	CPPUNIT_TEST( Should_Return_ScriptSystemType_On_GetType );
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Initialize_Correctly( );
	void Should_Return_Scene_On_CreateScene( );
	void Should_Return_ScriptSystemType_On_GetType( );

};

#endif
