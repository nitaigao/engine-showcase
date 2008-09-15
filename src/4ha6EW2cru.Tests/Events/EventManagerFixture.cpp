#include "EventManagerFixture.h"
#include "Logging/Logger.h"
#include "Events/Event.h"
#include "Events/EventManager.h"

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
}