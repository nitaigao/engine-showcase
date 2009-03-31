#ifndef __INPUTSYSTEMSCENE_H
#define __INPUTSYSTEMSCENE_H

#include "../System/ISystem.hpp"
#include "../System/ISystemScene.hpp"

#include "OIS/OISInputManager.h"
#include "OIS/OISKeyboard.h"
#include "OIS/OISMouse.h"

#include "KeyboardListener.hpp"
#include "MouseListener.hpp"

class InputSystemScene : public ISystemScene, public OIS::KeyListener, public OIS::MouseListener
{
	typedef std::vector< InputSystemComponent* > InputSystemComponentList;

public:

	virtual ~InputSystemScene( );

	InputSystemScene( const int& screenWidth, const int& screenHeight );

	ISystemComponent* CreateComponent( const std::string& name, const std::string& type );
	void DestroyComponent( ISystemComponent* component );
	SystemType GetType( ) { return InputSystemType; }

	void Update( float deltaMilliseconds );

	void SetInputAllowed( bool inputAllowed ) { _inputAllowed = inputAllowed; };

	/* Fired when the user presses a button on the keyboard */
	bool keyPressed( const KeyEvent &arg );

	/* Fired when the user releases a button on the keyboard */
	bool keyReleased( const KeyEvent &arg );

	/* Fired when the user moves the mouse */
	bool mouseMoved( const MouseEvent &arg );

	/* Fired when the user presses a button on the mouse */
	bool mousePressed( const MouseEvent &arg, MouseButtonID id );

	/* Fired when the user releases a button on the mouse */
	bool mouseReleased( const MouseEvent &arg, MouseButtonID id );

private:

	bool _inputAllowed;

	OIS::InputManager* _inputManager;

	OIS::Mouse* _mouse;
	OIS::Keyboard* _keyboard;

	InputSystemComponentList _inputComponents;

	InputSystemScene( ) { };
	InputSystemScene( const InputSystemScene & copy ) { };
	InputSystemScene & operator = ( const InputSystemScene & copy ) { return *this; };

};

#endif