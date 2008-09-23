#include "InputSystem.h"

#include <sstream>
#include "../Logging/Logger.h"

#include "../Exceptions/IntializeFailedException.hpp"
#include "../Exceptions/OutOfRangeException.hpp"
#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/AlreadyInitializedException.hpp"

void InputSystem::Initialize( ) 
{ 	
	if( _inputManager != 0 )
	{
		Logger::GetInstance( )->Fatal( "InputManager::Initialize - InputManager has already been intitialized" );
		throw AlreadyInitializedException( "InputManager::Initialize - InputManager has already been intitialized" );
	}

	if ( _hWnd < 1 )
	{
		Logger::GetInstance( )->Fatal( "InputSystem::Initialize - hWnd was 0" );
		throw IntializeFailedException( "InputSystem::Initialize - hWnd was 0" );
	}

	Logger::GetInstance( )->Info( "Initializing Input System" );

	_inputManager = OIS::InputManager::createInputSystem( _hWnd );

	_keyboard = static_cast< OIS::Keyboard* >( _inputManager->createInputObject( OIS::OISKeyboard, true ) );
	_keyboardListener = new KeyboardListener( );
	_keyboard->setEventCallback( _keyboardListener );

	_mouse = static_cast< OIS::Mouse* >( _inputManager->createInputObject( OIS::OISMouse, true ) );
	_mouseListener = new MouseListener( );
	_mouse->setEventCallback( _mouseListener );
}

InputSystem::~InputSystem( )
{
	_inputManager->destroyInputSystem( _inputManager );

	delete _keyboardListener;
	delete _mouseListener;
}

void InputSystem::SetCaptureArea( int width, int height )
{
	if ( _inputManager == 0 )
	{
		Logger::GetInstance( )->Fatal( "InputManager::SetCaptureArea - InputManager has not been intitialized" );
		throw UnInitializedException( "InputManager::SetCaptureArea - InputManager has not been intitialized" );
	}

	if ( width < 1 && height < 1 )
	{
		Logger::GetInstance( )->Fatal( "InputSystem::SetCaptureArea - Either width or height was out of range" );
		throw OutOfRangeException( "InputSystem::SetCaptureArea - Either width or height was out of range" );
	}

	_mouse->getMouseState( ).width = width;
	_mouse->getMouseState( ).height = height;
}

void InputSystem::Update( ) const
{
	if ( _inputManager == 0 )
	{
		Logger::GetInstance( )->Fatal( "InputManager::Update - InputManager has not been intitialized" );
		throw UnInitializedException( "InputManager::Update - InputManager has not been intitialized" );
	}

	_mouse->capture( );
	_keyboard->capture( );
}