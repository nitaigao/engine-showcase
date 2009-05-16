#ifndef AI_SYSTEMSCENE_TESTS
#define AI_SYSTEMSCENE_TESTS

#include <cppunit/extensions/HelperMacros.h>

class AISystemScene_Tests : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( AISystemScene_Tests );
	CPPUNIT_TEST( Should_Return_Component_On_CreateComponent );
	CPPUNIT_TEST( Should_Delete_Component_On_DestroyComponent );
	CPPUNIT_TEST( Should_Create_ScriptScene_On_Initialize );
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Return_Component_On_CreateComponent( );
	void Should_Delete_Component_On_DestroyComponent( );
	void Should_Create_ScriptScene_On_Initialize( );


};

#endif