#include "KeyboardListenerFixture.h"

#include "Logging/Logger.h"
#include "Input/KeyboardListener.hpp"

#include "../Mocks/MockEventTrigger.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION( KeyboardListenerFixture );

void KeyboardListenerFixture::setUp( )
{
	Logger::Initialize( );
	EventManager::Initialize( );
}

void KeyboardListenerFixture::tearDown( )
{
	EventManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void KeyboardListenerFixture::Should_Fire_RETURN_KeyUp_Event( )
{
	MockEventTrigger* eventTriggerMock = new MockEventTrigger( );
	eventTriggerMock->handle_count.setExpected( 1 );

	EventManager::GetInstance( )->AddEventListener( KEY_UP, eventTriggerMock, &MockEventTrigger::EventHandler );

	KeyboardListener* listener = new KeyboardListener( );
	listener->keyReleased( OIS::KeyEvent( 0,  OIS::KC_RETURN, 0 ) );

	EventManager::GetInstance( )->Update( );
	EventManager::GetInstance( )->RemoveEventListener( KEY_UP, eventTriggerMock, &MockEventTrigger::EventHandler );

	eventTriggerMock->verify( );

	delete eventTriggerMock;
	delete listener;
}

void KeyboardListenerFixture::Should_Fire_RETURN_KeyDown_Event( )
{
	MockEventTrigger* eventTriggerMock = new MockEventTrigger( );
	eventTriggerMock->handle_count.setExpected( 1 );

	EventManager::GetInstance( )->AddEventListener( KEY_DOWN, eventTriggerMock, &MockEventTrigger::EventHandler );

	KeyboardListener* listener = new KeyboardListener( );
	listener->keyPressed( OIS::KeyEvent( 0,  OIS::KC_RETURN, 0 ) );

	EventManager::GetInstance( )->Update( );
	EventManager::GetInstance( )->RemoveEventListener( KEY_DOWN, eventTriggerMock, &MockEventTrigger::EventHandler );

	eventTriggerMock->verify( );

	delete eventTriggerMock;
	delete listener;
}