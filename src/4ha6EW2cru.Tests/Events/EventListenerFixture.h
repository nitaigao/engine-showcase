#ifndef EVENTLISTENER_FIXTURE_H
#define EVENTLISTENER_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "Events/IEventListener.hpp"
#include "../Mocks/Mock_EventSubject.hpp"

class EventListenerFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( EventListenerFixture );
  CPPUNIT_TEST( EventListener_Should_Return_Correct_Event_Type );
  CPPUNIT_TEST( EventListener_Should_Handle_Event );
  CPPUNIT_TEST( Should_Throw_NullReferenceException_On_HandleEvent_Given_Handler_Is_NULL );
  CPPUNIT_TEST( Should_Return_Correct_EventHandler_Function );
  CPPUNIT_TEST( Should_Return_NULL_Handler_Given_Constructed_With_NULL_HandleFunctor );
  CPPUNIT_TEST( Should_Return_NULL_Handler_Given_Constructed_With_NULL_HandleTarget );
  CPPUNIT_TEST( Should_Return_Correct_Handler_Target );
  CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

  void EventListener_Should_Return_Correct_Event_Type( );
  void EventListener_Should_Handle_Event( );
  void Should_Throw_NullReferenceException_On_HandleEvent_Given_Handler_Is_NULL( );
  void Should_Return_Correct_EventHandler_Function( );
  void Should_Return_NULL_Handler_Given_Constructed_With_NULL_HandleFunctor( );
  void Should_Return_NULL_Handler_Given_Constructed_With_NULL_HandleTarget( );
  void Should_Return_Correct_Handler_Target( );

private:

	Mock_EventSubject* _eventTarget;
	Events::IEventListener* _eventListener;
};

#endif