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

	Logger::GetInstance( )->Info( "Initializing Input System" );

	_inputManager = OIS::InputManager::createInputSystem( _renderSystem->GetProperties( )[ "hwnd" ].GetValue( ).Size );

	_keyboard = static_cast< OIS::Keyboard* >( _inputManager->createInputObject( OIS::OISKeyboard, true ) );
	_keyboardListener = new KeyboardListener( _keyboard );
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

void InputSystem::Update( float deltaMilliseconds )
{
	if ( _inputManager == 0 )
	{
		Logger::GetInstance( )->Fatal( "InputManager::Update - InputManager has not been intitialized" );
		throw UnInitializedException( "InputManager::Update - InputManager has not been intitialized" );
	}

	_mouse->getMouseState( ).width = _configuration->Find( "Graphics", "Width", 640 );
	_mouse->getMouseState( ).height = _configuration->Find( "Graphics", "Height", 480 );

	_mouse->capture( );
	_keyboard->capture( );
}