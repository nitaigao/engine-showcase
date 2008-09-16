#ifndef __SCREEN_FIXTURE_H
#define __SCREEN_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "MyGUI.h"
using namespace MyGUI;

class ScreenFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( ScreenFixture );

  CPPUNIT_TEST_EXCEPTION( Should_Fail_Intialization_No_Gui, std::exception );
  CPPUNIT_TEST_EXCEPTION( Should_Fail_Initialization_No_Script, std::exception );
  CPPUNIT_TEST( Should_SuccessFully_Initialize );

  CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Fail_Intialization_No_Gui( );
	void Should_Fail_Initialization_No_Script( );
	void Should_SuccessFully_Initialize( );

private:

	Gui* _gui;

};

#endif