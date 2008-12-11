#include "KeyboardListenerFixture.h"

#include "Logging/Logger.h"
#include "Input/KeyboardListener.hpp"

#include "../Mocks/MockEventTrigger.hpp"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( KeyboardListenerFixture, Suites::InputSuite( ) );

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
	/*MockEventTrigger* eventTriggerMock = new MockEventTrigger( );
	eventTriggerMock->handle_count.setExpected( 1 );

	EventManager::GetInstance( )->AddEventListener( INPUT_KEY_UP, eventTriggerMock, &MockEventTrigger::EventHandler );

	KeyboardListener* listener = new KeyboardListener( );
	listener->keyReleased( OIS::KeyEvent( 0,  OIS::KC_RETURN, 0 ) );

	EventManager::GetInstance( )->Update( );
	EventManager::GetInstance( )->RemoveEventListener( INPUT_KEY_UP, eventTriggerMock, &MockEventTrigger::EventHandler );

	eventTriggerMock->verify( );

	delete eventTriggerMock;
	delete listener;*/
}

void KeyboardListenerFixture::Should_Fire_RETURN_KeyDown_Event( )
{
	/*MockEventTrigger* eventTriggerMock = new MockEventTrigger( );
	eventTriggerMock->handle_count.setExpected( 1 );

	EventManager::GetInstance( )->AddEventListener( INPUT_KEY_DOWN, eventTriggerMock, &MockEventTrigger::EventHandler );

	KeyboardListener* listener = new KeyboardListener( );
	listener->keyPressed( OIS::KeyEvent( 0,  OIS::KC_RETURN, 0 ) );

	EventManager::GetInstance( )->Update( );
	EventManager::GetInstance( )->RemoveEventListener( INPUT_KEY_DOWN, eventTriggerMock, &MockEventTrigger::EventHandler );

	eventTriggerMock->verify( );

	delete eventTriggerMock;
	delete listener;*/
}