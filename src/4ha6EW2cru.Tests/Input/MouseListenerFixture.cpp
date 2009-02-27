#include "MouseListenerFixture.h"

#include "Logging/Logger.h"
#include "Input/MouseListener.hpp"

#include "ois/OISMouse.h"

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MouseListenerFixture, Suites::InputSuite( ) );

void MouseListenerFixture::setUp( )
{
	Logger::Initialize( );
	EventManager::Initialize( );

	_eventTriggerMock = new MockEventTrigger( );
	_eventTriggerMock->handle_count.setExpected( 1 );
}

void MouseListenerFixture::tearDown( )
{
	delete _eventTriggerMock;

	EventManager::GetInstance( )->Release( );
	Logger::GetInstance( )->Release( );
}

void MouseListenerFixture::Should_Fire_Left_MouseReleased_Event( )
{
	EventManager::GetInstance( )->AddEventListener( INPUT_MOUSE_RELEASED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	MouseListener* listener = new MouseListener( );
	OIS::MouseState state;
	listener->mouseReleased( OIS::MouseEvent( 0, state ), OIS::MB_Left );

	EventManager::GetInstance( )->Update( 0 );
	EventManager::GetInstance( )->RemoveEventListener( INPUT_MOUSE_RELEASED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	_eventTriggerMock->verify( );

	delete listener;
}

void MouseListenerFixture::Should_Fire_Left_MousePressed_Event( )
{
	_eventTriggerMock->handle_count.setExpected( 1 );

	EventManager::GetInstance( )->AddEventListener( INPUT_MOUSE_PRESSED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	MouseListener* listener = new MouseListener( );
	OIS::MouseState state;
	listener->mousePressed( OIS::MouseEvent( 0, state ), OIS::MB_Left );

	EventManager::GetInstance( )->Update( 0 );
	EventManager::GetInstance( )->RemoveEventListener( INPUT_MOUSE_PRESSED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	_eventTriggerMock->verify( );

	delete listener;
}

void MouseListenerFixture::Should_Fire_Right_MouseReleased_Event( )
{
	_eventTriggerMock->handle_count.setExpected( 1 );

	EventManager::GetInstance( )->AddEventListener( INPUT_MOUSE_RELEASED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	MouseListener* listener = new MouseListener( );
	OIS::MouseState state;
	listener->mouseReleased( OIS::MouseEvent( 0, state ), OIS::MB_Right );

	EventManager::GetInstance( )->Update( 0 );
	EventManager::GetInstance( )->RemoveEventListener( INPUT_MOUSE_RELEASED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	_eventTriggerMock->verify( );

	delete listener;
}

void MouseListenerFixture::Should_Fire_Right_MousePressed_Event( )
{
	_eventTriggerMock->handle_count.setExpected( 1 );

	EventManager::GetInstance( )->AddEventListener( INPUT_MOUSE_PRESSED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	MouseListener* listener = new MouseListener( );
	OIS::MouseState state;
	listener->mousePressed( OIS::MouseEvent( 0, state ), OIS::MB_Right );

	EventManager::GetInstance( )->Update( 0 );
	EventManager::GetInstance( )->RemoveEventListener( INPUT_MOUSE_PRESSED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	_eventTriggerMock->verify( );

	delete listener;
}

void MouseListenerFixture::Should_Fire_MouseMoved_Event( )
{
	_eventTriggerMock->handle_count.setExpected( 1 );

	EventManager::GetInstance( )->AddEventListener( INPUT_MOUSE_MOVED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	MouseListener* listener = new MouseListener( );
	OIS::MouseState state;
	listener->mouseMoved( OIS::MouseEvent( 0, state ) );

	EventManager::GetInstance( )->Update( 0 );
	EventManager::GetInstance( )->RemoveEventListener( INPUT_MOUSE_MOVED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	_eventTriggerMock->verify( );

	delete listener;
}