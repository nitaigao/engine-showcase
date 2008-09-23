#ifndef __SCREEN_FIXTURE_H
#define __SCREEN_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "MyGUI.h"
using namespace MyGUI;

class ScreenFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( ScreenFixture );

  CPPUNIT_TEST( Should_Throw_On_Initialize_Given_NULL_Gui );
  CPPUNIT_TEST( Should_Throw_On_Initialize_Given_NULL_Script );
  CPPUNIT_TEST( Should_Throw_On_Initialize_Given_Already_Initialized );
  CPPUNIT_TEST( Should_Initialize_Given_Valid_Parameters );

  CPPUNIT_TEST( Should_Return_ScreenName );
  CPPUNIT_TEST( Should_Return_VisibilityMask );

  CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Throw_On_Initialize_Given_NULL_Gui( );
	void Should_Throw_On_Initialize_Given_NULL_Script( );;
	void Should_Throw_On_Initialize_Given_Already_Initialized( );
	void Should_Initialize_Given_Valid_Parameters( );

	void Should_Return_ScreenName( );
	void Should_Return_VisibilityMask( );

private:

	Gui* _gui;

};

#endif