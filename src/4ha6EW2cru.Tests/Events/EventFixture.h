#ifndef __EVENT_FIXTURE_H
#define __EVENT_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

class EventFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( EventFixture );
  CPPUNIT_TEST( Should_Return_Correct_Type );
  CPPUNIT_TEST( Should_Return_NULL_Data );
  CPPUNIT_TEST( Should_Return_Correct_Data );
  CPPUNIT_TEST_SUITE_END( );

protected:

  void Should_Return_Correct_Type( );
  void Should_Return_NULL_Data( );
  void Should_Return_Correct_Data( );

};

#endif