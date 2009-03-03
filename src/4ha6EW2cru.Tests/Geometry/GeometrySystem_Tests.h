#ifndef __GEOMETRYSYSTEM_TESTS_H
#define __GEOMETRYSYSTEM_TESTS_H

#include <cppunit/extensions/HelperMacros.h>

class GeometrySystem_Tests : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( GeometrySystem_Tests );
	CPPUNIT_TEST( Should_Return_GeometrySystemScene_On_CreateScene );
	CPPUNIT_TEST_SUITE_END( );

protected:

	void Should_Return_GeometrySystemScene_On_CreateScene( );

};

#endif