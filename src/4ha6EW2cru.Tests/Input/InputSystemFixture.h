#ifndef __INPUTSYSTEM_FIXTURE_H
#define __INPUTSYSTEM_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class InputSystemFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( InputSystemFixture );
  CPPUNIT_TEST( Should_Initialize_Properly );
  CPPUNIT_TEST( Should_Not_Initialize_With_Null_OISInputManager );
  CPPUNIT_TEST_SUITE_END( );

public:

	void setUp ( void );
	void tearDown ( void );

protected:

  void Should_Initialize_Properly( );
  void Should_Not_Initialize_With_Null_OISInputManager( );

};

#endif