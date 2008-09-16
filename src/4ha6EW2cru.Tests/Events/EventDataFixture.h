#ifndef __EVENTDATA_FIXTURE_H
#define __EVENTDATA_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "Events/EventData.hpp"
#include <OIS/OISKeyboard.h>


class EventDataFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( EventDataFixture );
  CPPUNIT_TEST( Should_KeyEvent_Return_Correct_Keycode );
  CPPUNIT_TEST_SUITE_END( );

protected:

  void Should_KeyEvent_Return_Correct_Keycode( );

};

#endif