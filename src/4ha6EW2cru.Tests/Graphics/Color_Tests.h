#ifndef COLOR_FIXTURE_H
#define COLOR_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "Renderer/Color.hpp"

class Color_Tests : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( Color_Tests );
	CPPUNIT_TEST( Should_Set_RGB_On_Construction );
	CPPUNIT_TEST_SUITE_END( );

protected:

	void Should_Set_RGB_On_Construction( );
};

#endif