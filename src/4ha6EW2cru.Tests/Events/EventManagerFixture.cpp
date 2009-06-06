#include "EventManagerFixture.h"

#include "Logging/Logger.h"
using namespace Logging;

#include "Events/Event.h"
#include "Events/EventManager.h"
using namespace Events;

#include "Management/Management.h"

#include "../Mocks/MockEventTrigger.hpp"
#include "../Mocks/Mock_EventSubject.hpp"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( EventManagerFixture, Suites::EventsSuite( ) );

void EventManagerFixture::setUp( )
{
	Management::Initialize( );
}

void EventManagerFixture::tearDown( )
{
	Management::Release( ); 
}

void EventManagerFixture:: Should_Queue_Event_Successfully_Given_Intiailized( )
{
	IEvent* event = new Event( TEST_EVENT );

	EventManager eventManager;
	eventManager.QueueEvent( event );
}

void EventManagerFixture:: Should_Throw_Given_NULL_Event_To_QueueEvent( )
{
	EventManager eventManager;
	CPPUNIT_ASSERT_THROW( eventManager.QueueEvent( 0 ), NullReferenceException );
}

void EventManagerFixture:: Should_Update_Given_Intialized( )
{
	EventManager eventManager;
	eventManager.Update( 0 );
}

void EventManagerFixture:: Should_Throw_On_AddEventListener_Given_NULL_Target( )
{
	EventManager eventManager;
	CPPUNIT_ASSERT_THROW( eventManager.AddEventListener< Mock_EventSubject >( TEST_EVENT, 0, &Mock_EventSubject::EventHandler ), NullReferenceException );
}

void EventManagerFixture:: Should_Throw_On_AddEventListener_Given_NULL_HandlerFunctor( )
{
	Mock_EventSubject* eventSubject = new Mock_EventSubject( );
	EventManager eventManager;
	CPPUNIT_ASSERT_THROW( eventManager.AddEventListener< Mock_EventSubject >( TEST_EVENT, eventSubject, 0 ), NullReferenceException );

	delete eventSubject;
}

void EventManagerFixture:: Should_Add_Listener_Given_Correct_Listener_And_Initialized( )
{
	Mock_EventSubject* eventSubject = new Mock_EventSubject( );
	EventManager eventManager;
	eventManager.AddEventListener< Mock_EventSubject >( TEST_EVENT, eventSubject, &Mock_EventSubject::EventHandler );

	delete eventSubject;
}

void EventManagerFixture:: Should_Throw_On_RemoveEventListener_Given_NULL_Target( )
{
	EventManager eventManager;
	CPPUNIT_ASSERT_THROW( eventManager.RemoveEventListener< Mock_EventSubject > ( TEST_EVENT, 0, &Mock_EventSubject::EventHandler ), NullReferenceException );
}

void EventManagerFixture:: Should_Throw_On_RemoveEventListener_Given_NULL_HandlerFunctor( )
{
	Mock_EventSubject* eventSubject = new Mock_EventSubject( );

	EventManager eventManager;
	CPPUNIT_ASSERT_THROW( eventManager.RemoveEventListener< Mock_EventSubject > ( TEST_EVENT, eventSubject, 0 ), NullReferenceException );

	delete eventSubject;
}

void EventManagerFixture:: Should_Remove_Listener_Given_Correct_Listener_And_Initialized( )
{
	Mock_EventSubject* eventSubject = new Mock_EventSubject( );

	EventManager eventManager;
	eventManager.AddEventListener( TEST_EVENT, eventSubject, &Mock_EventSubject::EventHandler );
	eventManager.RemoveEventListener( TEST_EVENT, eventSubject, &Mock_EventSubject::EventHandler );

	delete eventSubject;
}

void EventManagerFixture:: Should_Still_Remove_Listener_Given_Non_Existant_Listener_And_Initialized( )
{
	Mock_EventSubject* eventSubject = new Mock_EventSubject( );

	EventManager eventManager;
	eventManager.RemoveEventListener( TEST_EVENT, eventSubject, &Mock_EventSubject::EventHandler );
	
	delete eventSubject;
}

void EventManagerFixture:: Should_Queue_Event_Given_Correct_Event( )
{
	EventManager eventManager;
	eventManager.QueueEvent( new Event( TEST_EVENT ) );	
}

void EventManagerFixture:: Should_Throw_On_Queue_Event_Given_NULL_Event( )
{
	EventManager eventManager;
	CPPUNIT_ASSERT_THROW( eventManager.QueueEvent( 0 ), NullReferenceException );
}

void EventManagerFixture::Should_Trigger_Event_Given_Initialized( )
{
	EventManager eventManager;
	eventManager.TriggerEvent( new Event( TEST_EVENT ) );	
}

void EventManagerFixture::Should_Throw_On_TriggerEvent_Given_Event_Is_NULL( )
{
	EventManager eventManager;
	CPPUNIT_ASSERT_THROW( eventManager.TriggerEvent( 0 ), NullReferenceException );
}