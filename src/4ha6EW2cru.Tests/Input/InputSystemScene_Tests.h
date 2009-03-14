#ifndef __INPUT_SYSTEMSCENE_TESTS
#define __INPUT_SYSTEMSCENE_TESTS

#include <cppunit/extensions/HelperMacros.h>

class InputSystemScene_Tests : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( InputSystemScene_Tests );
	CPPUNIT_TEST( Should_Return_An_InputSystemComponent_On_CreateComponent );
	CPPUNIT_TEST( Should_Destroy_Component_On_DestroyComponent );
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Return_An_InputSystemComponent_On_CreateComponent( );
	void Should_Destroy_Component_On_DestroyComponent( );

};

#endif