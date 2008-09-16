#include "EventFixture.h"
#include "../Mocks/MockEventData.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION( EventFixture );

void EventFixture::Should_Return_Correct_Data( )
{
	TestEventData* testEventData = new TestEventData( 99 );

	Event event( TEST_EVENT, testEventData );

	const TestEventData* eventData = static_cast< const TestEventData* >( event.GetEventData( ) ) ;

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