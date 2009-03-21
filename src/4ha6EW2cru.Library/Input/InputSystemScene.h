#ifndef __INPUTSYSTEMSCENE_H
#define __INPUTSYSTEMSCENE_H

#include "../System/ISystem.hpp"
#include "../System/ISystemScene.hpp"

#include "OIS/OISInputManager.h"
#include "OIS/OISKeyboard.h"
#include "OIS/OISMouse.h"

#include "KeyboardListener.hpp"
#include "MouseListener.hpp"

class InputSystemScene : public ISystemScene
{

public:

	virtual ~InputSystemScene( );

	InputSystemScene( const int& screenWidth, const int& screenHeight );

	ISystemComponent* CreateComponent( const std::string& name, const std::string& type );
	void DestroyComponent( ISystemComponent* component );
	SystemType GetType( ) { return InputSystemType; }
	void Update( float deltaMilliseconds );

private:

	OIS::InputManager* _inputManager;

	OIS::Mouse* _mouse;
	MouseListener* _mouseListener;

	OIS::Keyboard* _keyboard;
	KeyboardListener* _keyboardListener;

	InputSystemScene( ) { };
	InputSystemScene( const InputSystemScene & copy ) { };
	InputSystemScene & operator = ( const InputSystemScene & copy ) { return *this; };

};

#endif