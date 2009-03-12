#ifndef __HAVOKPHYSICS_SYSTEM_TESTS
#define __HAVOKPHYSICS_SYSTEM_TESTS

#include <cppunit/extensions/HelperMacros.h>

#include "Exceptions/UnInitializedException.hpp"
#include "Exceptions/AlreadyInitializedException.hpp"

class HavokPhysicsSystem_Tests : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( HavokPhysicsSystem_Tests );
	CPPUNIT_TEST( Should_Initialize_Correctly );
	CPPUNIT_TEST( Should_Return_PhysicsScene_On_CreateScene );
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Initialize_Correctly( );
	void Should_Return_PhysicsScene_On_CreateScene( );

};

#endif