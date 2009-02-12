#ifndef __INPUTMANAGER_H
#define __INPUTMANAGER_H

#include "IInputSystem.hpp"

#include "OIS/OISInputManager.h"
#include "OIS/OISKeyboard.h"
#include "OIS/OISMouse.h"

#include "KeyboardListener.hpp"
#include "MouseListener.hpp"

/*!
	Receives all Input from the User(s) and translates it into Game Events
*/
class InputSystem : public IInputSystem
{

public:

	InputSystem( std::size_t hWnd )
		: _inputManager( 0 )
		, _keyboard( 0 )
		, _keyboardListener( 0 )
		, _mouse( 0 )
		, _mouseListener( 0 )
		, _hWnd( hWnd )
	{

	};

	~InputSystem( );

	/* Initializes the Input System */
	void Initialize( );

	/* Performs a capture loop on all Input Devices */
	void Update( ) const;

	/*! Resets the input capture area of the Input System */
	void SetCaptureArea( int width, int height );

private:

	std::size_t _hWnd;

	OIS::InputManager* _inputManager;

	OIS::Mouse* _mouse;
	MouseListener* _mouseListener;

	OIS::Keyboard* _keyboard;
	KeyboardListener* _keyboardListener;

};

#endif
