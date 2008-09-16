#ifndef __INPUTMANAGER_H
#define __INPUTMANAGER_H

#include "OIS/OISInputManager.h"
#include "OIS/OISKeyboard.h"
#include "OIS/OISMouse.h"

#include "KeyboardListener.hpp"
#include "MouseListener.hpp"

/*!
	Receives all Input from the User(s) and translates it into Game Events
*/
class InputSystem
{

public:

	InputSystem( OIS::InputManager* inputManager )
		: _inputManager( inputManager )
		, _keyboard( 0 )
		, _keyboardListener( 0 )
		, _mouse( 0 )
		, _mouseListener( 0 )
	{ };

	~InputSystem( );

	/* Initializes the Input System */
	bool Initialize( );

	/* Performs a capture loop on all Input Devices */
	void Update( ) const;

	/*! Resets the input capture area of the Input System */
	void SetCaptureArea( int width, int height );

	/* Releases all Sub systems */
	void Release( );

private:

	OIS::InputManager* _inputManager;

	OIS::Mouse* _mouse;
	MouseListener* _mouseListener;

	OIS::Keyboard* _keyboard;
	KeyboardListener* _keyboardListener;

};

#endif