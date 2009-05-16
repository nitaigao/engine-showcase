#ifndef SCRIPT_SYSTEMSCENE_TESTS
#define SCRIPT_SYSTEMSCENE_TESTS

#include <cppunit/extensions/HelperMacros.h>

class ScriptSystemScene_Tests : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( ScriptSystemScene_Tests );
	CPPUNIT_TEST( Should_Initialize_Correctly );
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Initialize_Correctly( );

};

#endif
