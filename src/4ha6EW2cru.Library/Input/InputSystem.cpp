#include "InputSystem.h"

#include "../Logging/Logger.h"

bool InputSystem::Initialize( ) 
{ 	
	Logger::GetInstance( )->Info( "Initializing Input System" );

	if ( _inputManager != 0 )
	{
		_keyboard = static_cast< OIS::Keyboard* >( _inputManager->createInputObject( OIS::OISKeyboard, true ) );
		_keyboardListener = new KeyboardListener( );
		_keyboard->setEventCallback( _keyboardListener );

		_mouse = static_cast< OIS::Mouse* >( _inputManager->createInputObject( OIS::OISMouse, true ) );
		_mouseListener = new MouseListener( );
		_mouse->setEventCallback( _mouseListener );

		return true;
	}

	Logger::GetInstance( )->Fatal( "Input System Failed to Initialize" );
	return false;
}

InputSystem::~InputSystem( )
{
	delete _keyboardListener;
	delete _mouseListener;
}

void InputSystem::SetCaptureArea( int width, int height )
{
	_mouse->getMouseState( ).width = width;
	_mouse->getMouseState( ).height = height;
}

void InputSystem::Update( ) const
{
	_mouse->capture( );
	_keyboard->capture( );
}

void InputSystem::Release( )
{
	_inputManager->destroyInputSystem( _inputManager );
}