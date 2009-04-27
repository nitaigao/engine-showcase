#include "Color_Tests.h"

#include "Renderer/Color.hpp"
using namespace Renderer;

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( Color_Tests, Suites::GraphicsSuite( ) );

void Color_Tests::Should_Set_RGB_On_Construction()
{
	int r = 10;
	int b = 11;
	int g = 12;

	Color color( r, g, b );

	CPPUNIT_ASSERT( color.Red == r );
	CPPUNIT_ASSERT( color.Green == g );
	CPPUNIT_ASSERT( color.Blue == b );
}