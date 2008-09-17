#include "EventListenerFixture.h"

#include "Events/EventListener.h"
#include "Events/Event.h"

#include "Exceptions/NullReferenceException.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION( EventListenerFixture );

void EventListenerFixture::setUp( )
{
	Logger::Initialize( );

	_eventTarget = 0;
	_eventTarget = new EventSubjectMock( );

	_eventListener = new EventListener< EventSubjectMock >( TEST_EVENT, _eventTarget, &EventSubjectMock::EventHandler );
}

void EventListenerFixture::tearDown( )
{
	delete _eventListener;
	_eventListener = 0;

	delete _eventTarget;
	_eventTarget = 0;

	Logger::GetInstance( )->Release( );
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
	EventListener< EventSubjectMock >* fullEventListener = static_cast< EventListener< EventSubjectMock >* >( _eventListener );

	CPPUNIT_ASSERT( &EventSubjectMock::EventHandler == fullEventListener->GetHandlerFunctor( ) );
	CPPUNIT_ASSERT( &EventSubjectMock::FalseEventHandler != fullEventListener->GetHandlerFunctor( ) );
}

void EventListenerFixture::Should_Throw_NullReferenceException_On_HandleEvent_Given_Handler_Is_NULL( )
{
	delete _eventListener;

	_eventListener = new EventListener< EventSubjectMock >( TEST_EVENT, _eventTarget, 0 );
	CPPUNIT_ASSERT_THROW( _eventListener->HandleEvent( 0 ), NullReferenceException );
}

void EventListenerFixture::Should_Return_NULL_Handler_Given_Constructed_With_NULL_HandleFunctor( )
{
	delete _eventListener;

	_eventListener = new EventListener< EventSubjectMock >( TEST_EVENT, _eventTarget, 0 );
	EventListener< EventSubjectMock >* fullEventListener = static_cast< EventListener< EventSubjectMock >* >( _eventListener );
	CPPUNIT_ASSERT( 0 == fullEventListener->GetHandlerFunctor( ) );
}

void EventListenerFixture::Should_Return_NULL_Handler_Given_Constructed_With_NULL_HandleTarget( )
{
	delete _eventListener;

	_eventListener = new EventListener< EventSubjectMock >( TEST_EVENT, 0, &EventSubjectMock::EventHandler );
	EventListener< EventSubjectMock >* fullEventListener = static_cast< EventListener< EventSubjectMock >* >( _eventListener );
	CPPUNIT_ASSERT( 0 == fullEventListener->GetHandlerTarget( ) );
}

void EventListenerFixture::Should_Return_Correct_Handler_Target( )
{
	EventListener< EventSubjectMock >* fullEventListener = static_cast< EventListener< EventSubjectMock >* >( _eventListener );
	CPPUNIT_ASSERT( _eventTarget == fullEventListener->GetHandlerTarget( ) );
}