#include "EventFixture.h"

#include "Events/Event.h"
#include "Events/IEventData.hpp"
using namespace Events;

#include "../Mocks/MockEventData.hpp"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( EventFixture, Suites::EventsSuite( ) );

void EventFixture::Should_Return_Correct_Data( )
{
	MockEventData* testEventData = new MockEventData( 99 );
	Event event( TEST_EVENT, testEventData );

	MockEventData* eventData = static_cast< MockEventData* >( event.GetEventData( ) ) ;
	CPPUNIT_ASSERT( testEventData->GetData( ) == eventData->GetData( ) );
}

void EventFixture::Should_Return_NULL_Data( )
{
	Event event( TEST_EVENT );
	CPPUNIT_ASSERT( 0 == event.GetEventData( ) );
}

void EventFixture::Should_Return_Correct_Type( )
{
	Event event( TEST_EVENT );
	CPPUNIT_ASSERT( TEST_EVENT == event.GetEventType( ) );
}