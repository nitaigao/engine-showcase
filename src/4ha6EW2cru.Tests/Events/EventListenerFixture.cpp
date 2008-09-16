#include "EventListenerFixture.h"

CPPUNIT_TEST_SUITE_REGISTRATION( EventListenerFixture );

void EventListenerFixture::setUp( )
{
	_eventSubject = 0;
	_eventSubject = new EventSubjectMock( );
}

void EventListenerFixture::tearDown( )
{
	delete _eventSubject;
	_eventSubject = 0;
}

void EventListenerFixture::EventListener_Should_Return_Correct_Event_Type( )
{
	IEventListener* eventListener = new EventListener< EventSubjectMock >( TEST_EVENT, _eventSubject, &EventSubjectMock::EventHandler );
	
	CPPUNIT_ASSERT( TEST_EVENT == eventListener->GetEventType( ) );

	delete eventListener;
}

void EventListenerFixture::EventListener_Should_Handle_Event( )
{
	IEventListener* eventListener = new EventListener< EventSubjectMock >( TEST_EVENT, _eventSubject, &EventSubjectMock::EventHandler );

	Event* event = new Event( TEST_EVENT );
	eventListener->HandleEvent( event );

	CPPUNIT_ASSERT( _eventSubject->eventHandler_counter == 1 );

	delete event;
	delete eventListener;
}

void EventListenerFixture::Should_Return_Correct_EventHandler_Function( )
{
	IEventListener* eventListener = new EventListener< EventSubjectMock >( TEST_EVENT, _eventSubject, &EventSubjectMock::EventHandler );

	Event* event = new Event( TEST_EVENT );
	eventListener->HandleEvent( event );

	EventListener< EventSubjectMock >* realEventListener = static_cast< EventListener< EventSubjectMock >* >( eventListener );

	CPPUNIT_ASSERT( &EventSubjectMock::EventHandler == realEventListener->GetHandlerFunctor( ) );
	CPPUNIT_ASSERT( _eventSubject == realEventListener->GetHandlerTarget( ) );
	CPPUNIT_ASSERT( TEST_EVENT == realEventListener->GetEventType( ) );

	delete event;
	delete eventListener;
}