#include "EventManagerFixture.h"
#include "Logging/Logger.h"
#include "Events/Event.h"
#include "Events/EventManager.h"
#include "../Mocks/MockEventSubject.hpp"

#include "../Mocks/MockEventTrigger.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION( EventManagerFixture );

void EventManagerFixture::setUp( )
{
	Logger::Initialize( );
}

void EventManagerFixture::tearDown( )
{
	Logger::GetInstance( )->Release( );
}
/*
void EventManagerFixture::EventManager_Should_Initialize_And_Release_Correctly( )
{
	bool result = EventManager::Initialize( );

	CPPUNIT_ASSERT( result );

	EventManager::GetInstance( )->Release( );
}

void EventManagerFixture::EventManager_Should_Perform_Update( )
{
	bool result = EventManager::Initialize( );

	CPPUNIT_ASSERT( result );

	EventManager::GetInstance( )->Update( );
	EventManager::GetInstance( )->Release( );
}

void EventManagerFixture::EventManager_Should_Handle_Event( )
{
	MockEventTrigger* eventTriggerMock = new MockEventTrigger( );
	eventTriggerMock->handle_count.setExpected( 1 );

	EventManager::Initialize( );
	EventManager::GetInstance( )->AddEventListener( TEST_EVENT, eventTriggerMock, &MockEventTrigger::EventHandler );
	EventManager::GetInstance( )->QueueEvent( new Event( TEST_EVENT ) );
	EventManager::GetInstance( )->Update( );
	EventManager::GetInstance( )->RemoveEventListener( TEST_EVENT, eventTriggerMock, &MockEventTrigger::EventHandler );
	EventManager::GetInstance( )->Update( );

	EventManager::GetInstance( )->Release( );	

	eventTriggerMock->verify( );

	delete eventTriggerMock;
}

void EventManagerFixture::Should_Throw_On_UnInitialized_GetInstance( )
{
	EventManager::GetInstance( );
}

void EventManagerFixture::Should_Add_And_Remove_Listener_Successfully( )
{
	MockEventTrigger* eventTriggerMock = new MockEventTrigger( );
	
	EventManager::Initialize( );
	EventManager::GetInstance( )->AddEventListener( TEST_EVENT, eventTriggerMock, &MockEventTrigger::EventHandler );
	EventManager::GetInstance( )->Update( );
	EventManager::GetInstance( )->RemoveEventListener( TEST_EVENT, eventTriggerMock, &MockEventTrigger::EventHandler );
	EventManager::GetInstance( )->Release( );	

	delete eventTriggerMock;
}

void EventManagerFixture::Should_Throw_On_Initialize_Given_Already_Intialized( )
{
	EventManager::Initialize( );

	CPPUNIT_ASSERT_THROW( 
		EventManager::Initialize( ),
		AlreadyInitializedException
		);

	EventManager::GetInstance( )->Release( );
}*/

void EventManagerFixture:: Should_Initialize_And_Release_Correctly( )
{
	EventManager::Initialize( );
	EventManager::GetInstance( )->Release( );
}

void EventManagerFixture:: Should_Throw_On_Initialize_Given_Already_Intialized( )
{
	EventManager::Initialize( );
	CPPUNIT_ASSERT_THROW( EventManager::Initialize( ), AlreadyInitializedException );
	EventManager::GetInstance( )->Release( );
}

void EventManagerFixture:: Should_Throw_On_Uninitialized_Release( )
{
	CPPUNIT_ASSERT_THROW( EventManager::GetInstance( )->Release( ), UnInitializedException );
}

void EventManagerFixture:: Should_Throw_On_UnInitialized_GetInstance( )
{
	CPPUNIT_ASSERT_THROW( EventManager::GetInstance( ), UnInitializedException );
}

void EventManagerFixture:: Should_GetInstance_Correctly_Given_Intiailized( )
{
	EventManager::Initialize( );
	CPPUNIT_ASSERT( 0 != EventManager::GetInstance( ) );
	EventManager::GetInstance( )->Release( );
}

void EventManagerFixture:: Should_Throw_On_QueueEvent_Uninitialized( )
{
	CPPUNIT_ASSERT_THROW( EventManager::GetInstance( )->QueueEvent( 0 ), UnInitializedException );
}

void EventManagerFixture:: Should_Queue_Event_Successfully_Given_Intiailized( )
{
	IEvent* event = new Event( TEST_EVENT );

	EventManager::Initialize( );
	EventManager::GetInstance( )->QueueEvent( event );
	EventManager::GetInstance( )->Release( );
}

void EventManagerFixture:: Should_Throw_Given_NULL_Event_To_QueueEvent( )
{
	EventManager::Initialize( );
	CPPUNIT_ASSERT_THROW( EventManager::GetInstance( )->QueueEvent( 0 ), NullReferenceException );
	EventManager::GetInstance( )->Release( );
}

void EventManagerFixture:: Should_Update_Given_Intialized( )
{
	EventManager::Initialize( );
	EventManager::GetInstance( )->Update( );
	EventManager::GetInstance( )->Release( );
}

void EventManagerFixture:: Should_Throw_On_Update_Given_UnInitialized( )
{
	CPPUNIT_ASSERT_THROW( EventManager::GetInstance( )->Update( ), UnInitializedException );
}

void EventManagerFixture:: Should_Throw_On_AddEventListener_Given_Uninitialized( )
{
	CPPUNIT_ASSERT_THROW( EventManager::GetInstance( )->AddEventListener< EventSubjectMock >( TEST_EVENT, 0, &EventSubjectMock::EventHandler ), UnInitializedException );
}

void EventManagerFixture:: Should_Throw_On_AddEventListener_Given_NULL_Target( )
{
	EventManager::Initialize( );
	CPPUNIT_ASSERT_THROW( EventManager::GetInstance( )->AddEventListener< EventSubjectMock >( TEST_EVENT, 0, &EventSubjectMock::EventHandler ), NullReferenceException );
	EventManager::GetInstance( )->Release( );
}

void EventManagerFixture:: Should_Throw_On_AddEventListener_Given_NULL_HandlerFunctor( )
{
	EventSubjectMock* eventSubject = new EventSubjectMock( );
	EventManager::Initialize( );
	CPPUNIT_ASSERT_THROW( EventManager::GetInstance( )->AddEventListener< EventSubjectMock >( TEST_EVENT, eventSubject, 0 ), NullReferenceException );
	EventManager::GetInstance( )->Release( );

	delete eventSubject;
}

void EventManagerFixture:: Should_Add_Listener_Given_Correct_Listener_And_Initialized( )
{
	EventSubjectMock* eventSubject = new EventSubjectMock( );
	EventManager::Initialize( );
	EventManager::GetInstance( )->AddEventListener< EventSubjectMock >( TEST_EVENT, eventSubject, &EventSubjectMock::EventHandler );
	EventManager::GetInstance( )->Release( );

	delete eventSubject;
}

void EventManagerFixture:: Should_Throw_On_RemoveEventListener_Given_Uninitialized( )
{
	CPPUNIT_ASSERT_THROW( EventManager::GetInstance( )->RemoveEventListener< EventSubjectMock > ( TEST_EVENT, 0, 0 ), UnInitializedException );
}

void EventManagerFixture:: Should_Throw_On_RemoveEventListener_Given_NULL_Target( )
{
	EventManager::Initialize( );
	CPPUNIT_ASSERT_THROW( EventManager::GetInstance( )->RemoveEventListener< EventSubjectMock > ( TEST_EVENT, 0, &EventSubjectMock::EventHandler ), NullReferenceException );
	EventManager::GetInstance( )->Release( );
}

void EventManagerFixture:: Should_Throw_On_RemoveEventListener_Given_NULL_HandlerFunctor( )
{
	EventSubjectMock* eventSubject = new EventSubjectMock( );

	EventManager::Initialize( );
	CPPUNIT_ASSERT_THROW( EventManager::GetInstance( )->RemoveEventListener< EventSubjectMock > ( TEST_EVENT, eventSubject, 0 ), NullReferenceException );
	EventManager::GetInstance( )->Release( );

	delete eventSubject;
}

void EventManagerFixture:: Should_Remove_Listener_Given_Correct_Listener_And_Initialized( )
{
	EventSubjectMock* eventSubject = new EventSubjectMock( );

	EventManager::Initialize( );
	EventManager::GetInstance( )->AddEventListener( TEST_EVENT, eventSubject, &EventSubjectMock::EventHandler );
	EventManager::GetInstance( )->RemoveEventListener( TEST_EVENT, eventSubject, &EventSubjectMock::EventHandler );
	EventManager::GetInstance( )->Release( );

	delete eventSubject;
}

void EventManagerFixture:: Should_Still_Remove_Listener_Given_Non_Existant_Listener_And_Initialized( )
{
	EventSubjectMock* eventSubject = new EventSubjectMock( );

	EventManager::Initialize( );
	EventManager::GetInstance( )->RemoveEventListener( TEST_EVENT, eventSubject, &EventSubjectMock::EventHandler );
	EventManager::GetInstance( )->Release( );
	
	delete eventSubject;
}

void EventManagerFixture:: Should_Queue_Event_Given_Correct_Event( )
{
	EventManager::Initialize( );
	EventManager::GetInstance( )->QueueEvent( new Event( TEST_EVENT ) );	
	EventManager::GetInstance( )->Release( );
}

void EventManagerFixture:: Should_Throw_On_Queue_Event_Given_NULL_Event( )
{
	EventManager::Initialize( );
	CPPUNIT_ASSERT_THROW( EventManager::GetInstance( )->QueueEvent( 0 ), NullReferenceException );
	EventManager::GetInstance( )->Release( );
}

void EventManagerFixture:: Should_Throw_On_Queue_Event_Given_Uninitialized( )
{
	CPPUNIT_ASSERT_THROW( EventManager::GetInstance( )->QueueEvent( 0 ), UnInitializedException );
}