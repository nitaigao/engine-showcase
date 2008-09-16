#ifndef __EVENTLISTENER_FIXTURE_H
#define __EVENTLISTENER_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>

#include "Events/EventListener.h"
#include "Events/Event.h"

class EventSubjectMock
{

public:

	EventSubjectMock( )
		: eventHandler_counter( 0 )
	{ };

	void EventHandler( const IEvent* const event )
	{
		eventHandler_counter++;
	};

	void FalseEventHandler( const IEvent* const event ) { };

	int eventHandler_counter;

};


class EventListenerFixture : public CPPUNIT_NS::TestFixture
{

  CPPUNIT_TEST_SUITE( EventListenerFixture );
  CPPUNIT_TEST( EventListener_Should_Return_Correct_Event_Type );
  CPPUNIT_TEST( EventListener_Should_Handle_Event );
  CPPUNIT_TEST( Should_Return_Correct_EventHandler_Function );
  CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

  void EventListener_Should_Return_Correct_Event_Type( );
  void EventListener_Should_Handle_Event( );
  void Should_Return_Correct_EventHandler_Function( );

private:

	EventSubjectMock* _eventSubject;

};

#endif