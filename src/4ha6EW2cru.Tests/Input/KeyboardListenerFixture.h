#ifndef KEYBOARDLISTENER_FIXTURE_H
#define KEYBOARDLISTENER_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class KeyboardListenerFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( KeyboardListenerFixture );
  CPPUNIT_TEST( Should_Fire_RETURN_KeyUp_Event );
  CPPUNIT_TEST( Should_Fire_RETURN_KeyDown_Event );
  CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Fire_RETURN_KeyUp_Event( );
	void Should_Fire_RETURN_KeyDown_Event( );

};

#endif