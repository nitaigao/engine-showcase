#ifndef INPUTSYSTEM_FIXTURE_H
#define INPUTSYSTEM_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class InputSystemFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( InputSystemFixture );
  CPPUNIT_TEST( Should_Initialize_Properly );
  CPPUNIT_TEST( Should_Throw_Given_Already_Initialized );
  CPPUNIT_TEST( Should_Not_Initialize_With_NULL_HWND );  
  CPPUNIT_TEST_SUITE_END( );

public:

	void setUp ( void );
	void tearDown ( void );

protected:

  void Should_Initialize_Properly( );
  void Should_Throw_Given_Already_Initialized( );
  void Should_Not_Initialize_With_NULL_HWND( );

};

#endif