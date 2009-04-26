#include "MouseListenerFixture.h"

#include "Logging/Logger.h"
#include "Input/MouseListener.hpp"

#include "ois/OISMouse.h"

using namespace Events;

#include "../Suites.h"
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MouseListenerFixture, Suites::InputSuite( ) );

void MouseListenerFixture::setUp( )
{
	Logger::Initialize( );
	_eventManager = new EventManager( );

	_eventTriggerMock = new MockEventTrigger( );
	_eventTriggerMock->handle_count.setExpected( 1 );
}

void MouseListenerFixture::tearDown( )
{
	delete _eventTriggerMock;

	delete _eventManager;
	Logger::GetInstance( )->Release( );
}

void MouseListenerFixture::Should_Fire_Left_MouseReleased_Event( )
{
	_eventManager->AddEventListener( INPUT_MOUSE_RELEASED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	MouseListener* listener = new MouseListener( );
	OIS::MouseState state;
	listener->mouseReleased( OIS::MouseEvent( 0, state ), OIS::MB_Left );

	_eventManager->Update( 0 );
	_eventManager->RemoveEventListener( INPUT_MOUSE_RELEASED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	_eventTriggerMock->verify( );

	delete listener;
}

void MouseListenerFixture::Should_Fire_Left_MousePressed_Event( )
{
	_eventTriggerMock->handle_count.setExpected( 1 );

	_eventManager->AddEventListener( INPUT_MOUSE_PRESSED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	MouseListener* listener = new MouseListener( );
	OIS::MouseState state;
	listener->mousePressed( OIS::MouseEvent( 0, state ), OIS::MB_Left );

	_eventManager->Update( 0 );
	_eventManager->RemoveEventListener( INPUT_MOUSE_PRESSED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	_eventTriggerMock->verify( );

	delete listener;
}

void MouseListenerFixture::Should_Fire_Right_MouseReleased_Event( )
{
	_eventTriggerMock->handle_count.setExpected( 1 );

	_eventManager->AddEventListener( INPUT_MOUSE_RELEASED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	MouseListener* listener = new MouseListener( );
	OIS::MouseState state;
	listener->mouseReleased( OIS::MouseEvent( 0, state ), OIS::MB_Right );

	_eventManager->Update( 0 );
	_eventManager->RemoveEventListener( INPUT_MOUSE_RELEASED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	_eventTriggerMock->verify( );

	delete listener;
}

void MouseListenerFixture::Should_Fire_Right_MousePressed_Event( )
{
	_eventTriggerMock->handle_count.setExpected( 1 );

	_eventManager->AddEventListener( INPUT_MOUSE_PRESSED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	MouseListener* listener = new MouseListener( );
	OIS::MouseState state;
	listener->mousePressed( OIS::MouseEvent( 0, state ), OIS::MB_Right );

	_eventManager->Update( 0 );
	_eventManager->RemoveEventListener( INPUT_MOUSE_PRESSED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	_eventTriggerMock->verify( );

	delete listener;
}

void MouseListenerFixture::Should_Fire_MouseMoved_Event( )
{
	_eventTriggerMock->handle_count.setExpected( 1 );

	_eventManager->AddEventListener( INPUT_MOUSE_MOVED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	MouseListener* listener = new MouseListener( );
	OIS::MouseState state;
	listener->mouseMoved( OIS::MouseEvent( 0, state ) );

	_eventManager->Update( 0 );
	_eventManager->RemoveEventListener( INPUT_MOUSE_MOVED, _eventTriggerMock, &MockEventTrigger::EventHandler );

	_eventTriggerMock->verify( );

	delete listener;
}