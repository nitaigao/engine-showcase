#ifndef PHYSICSSYSTEMCOMPONENT_TESTS
#define PHYSICSSYSTEMCOMPONENT_TESTS

#include <cppunit/extensions/HelperMacros.h>

class PhysicsSystemComponent_Tests : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( PhysicsSystemComponent_Tests );
	CPPUNIT_TEST( Should_Observe_A_Geometry_Change );
	CPPUNIT_TEST_SUITE_END( );

public:

	void Should_Observe_A_Geometry_Change( );

};

#endif