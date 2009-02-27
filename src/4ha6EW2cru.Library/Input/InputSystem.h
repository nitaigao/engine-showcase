#ifndef __INPUTMANAGER_H
#define __INPUTMANAGER_H

#include "../System/ISystem.hpp"
#include "../System/IConfiguration.hpp"

#include "OIS/OISInputManager.h"
#include "OIS/OISKeyboard.h"
#include "OIS/OISMouse.h"

#include "KeyboardListener.hpp"
#include "MouseListener.hpp"

/*!
	Receives all Input from the User(s) and translates it into Game Events
*/
class InputSystem : public ISystem
{

public:

	InputSystem( IConfiguration* configuration )
		: _inputManager( 0 )
		, _keyboard( 0 )
		, _keyboardListener( 0 )
		, _mouse( 0 )
		, _mouseListener( 0 )
		, _configuration( configuration )
		, _hWnd( 0 )
	{
		_renderSystem = Management::GetInstance( )->GetSystemManager( )->GetSystem( RenderSystemType );
	};

	~InputSystem( );

	/* Initializes the Input System */
	void Initialize( );

	/* Performs a capture loop on all Input Devices */
	void Update( float deltaMilliseconds );

	/*! Releases the Input System */
	void Release( ) { };

	SystemType GetSystemType( ) { return InputSystemType; };

	inline PropertyMap GetProperties( ) { return _properties; };

	ISystemObject* CreateObject( const std::string& name, SystemType systemType ) { return 0; };

private:

	std::size_t _hWnd;
	PropertyMap _properties;
	IConfiguration* _configuration;

	OIS::InputManager* _inputManager;

	OIS::Mouse* _mouse;
	MouseListener* _mouseListener;

	OIS::Keyboard* _keyboard;
	KeyboardListener* _keyboardListener;

	ISystem* _renderSystem;

};

#endif
