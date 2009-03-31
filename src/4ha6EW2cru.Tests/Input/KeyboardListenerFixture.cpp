#include "KeyboardListenerFixture.h"

#include "Logging/Logger.h"
#include "Input/KeyboardListener.hpp"

#include "../Mocks/MockEventTrigger.hpp"

#include "../Mocks/Mock_InputSystemComponent.hpp"
#include "../Mocks/Mock_Keyboard.hpp"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( KeyboardListenerFixture, Suites::InputSuite( ) );

void KeyboardListenerFixture::setUp( )
{
	Logger::Initialize( );
	Management::Initialize( );
}

void KeyboardListenerFixture::tearDown( )
{
	Management::GetInstance( )->Release( );
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

void KeyboardListenerFixture::Should_Call_All_Components_On_KeyDown()
{
	Mock_InputSystemComponent component( "Test" );
	component.keyDown_count.setExpected( 1 );
	Mock_Keyboard keyboard;

	KeyboardListener listener( &keyboard );
	listener.AddInputComponent( &component );

	KeyEvent event( 0, OIS::KC_0, 0 );
	listener.keyPressed( event );

	component.verify( );
}

void KeyboardListenerFixture::Should_Call_All_Components_On_KeyUp()
{
	Mock_InputSystemComponent component( "Test" );
	component.keyUp_count.setExpected( 1 );
	Mock_Keyboard keyboard;

	KeyboardListener listener( &keyboard );
	listener.AddInputComponent( &component );

	KeyEvent event( 0, OIS::KC_0, 0 );
	listener.keyReleased( event );

	component.verify( );
}