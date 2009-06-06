#include "EventListenerFixture.h"

#include "Events/EventListener.h"
#include "Events/Event.h"
using namespace Events;

#include "Logging/Logger.h"
using namespace Logging;

#include "Management/Management.h"

#include "Exceptions/NullReferenceException.hpp"

#include "../Mocks/Mock_EventSubject.hpp"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( EventListenerFixture, Suites::EventsSuite( ) );

void EventListenerFixture::setUp( )
{
	Management::Initialize( );

	_eventTarget = 0;
	_eventTarget = new Mock_EventSubject( );

	_eventListener = new EventListener< Mock_EventSubject >( TEST_EVENT, _eventTarget, &Mock_EventSubject::EventHandler );
}

void EventListenerFixture::tearDown( )
{
	delete _eventListener;
	_eventListener = 0;

	delete _eventTarget;
	_eventTarget = 0;

	Management::Release( );
}

void EventListenerFixture::EventListener_Should_Return_Correct_Event_Type( )
{
	CPPUNIT_ASSERT( TEST_EVENT == _eventListener->GetEventType( ) );
}

void EventListenerFixture::EventListener_Should_Handle_Event( )
{
	Event* event = new Event( TEST_EVENT );
	_eventListener->HandleEvent( event );

	CPPUNIT_ASSERT( _eventTarget->eventHandler_counter == 1 );

	delete event;
}

void EventListenerFixture::Should_Return_Correct_EventHandler_Function( )
{
	EventListener< Mock_EventSubject >* fullEventListener = static_cast< EventListener< Mock_EventSubject >* >( _eventListener );

	CPPUNIT_ASSERT( &Mock_EventSubject::EventHandler == fullEventListener->GetHandlerFunctor( ) );
	CPPUNIT_ASSERT( &Mock_EventSubject::FalseEventHandler != fullEventListener->GetHandlerFunctor( ) );
}

void EventListenerFixture::Should_Throw_NullReferenceException_On_HandleEvent_Given_Handler_Is_NULL( )
{
	delete _eventListener;

	_eventListener = new EventListener< Mock_EventSubject >( TEST_EVENT, _eventTarget, 0 );
	CPPUNIT_ASSERT_THROW( _eventListener->HandleEvent( 0 ), NullReferenceException );
}

void EventListenerFixture::Should_Return_NULL_Handler_Given_Constructed_With_NULL_HandleFunctor( )
{
	delete _eventListener;

	_eventListener = new EventListener< Mock_EventSubject >( TEST_EVENT, _eventTarget, 0 );
	EventListener< Mock_EventSubject >* fullEventListener = static_cast< EventListener< Mock_EventSubject >* >( _eventListener );
	CPPUNIT_ASSERT( 0 == fullEventListener->GetHandlerFunctor( ) );
}

void EventListenerFixture::Should_Return_NULL_Handler_Given_Constructed_With_NULL_HandleTarget( )
{
	delete _eventListener;

	_eventListener = new EventListener< Mock_EventSubject >( TEST_EVENT, 0, &Mock_EventSubject::EventHandler );
	EventListener< Mock_EventSubject >* fullEventListener = static_cast< EventListener< Mock_EventSubject >* >( _eventListener );
	CPPUNIT_ASSERT( 0 == fullEventListener->GetHandlerTarget( ) );
}

void EventListenerFixture::Should_Return_Correct_Handler_Target( )
{
	EventListener< Mock_EventSubject >* fullEventListener = static_cast< EventListener< Mock_EventSubject >* >( _eventListener );
	CPPUNIT_ASSERT( _eventTarget == fullEventListener->GetHandlerTarget( ) );
}