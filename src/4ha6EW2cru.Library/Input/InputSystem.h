#ifndef __INPUTMANAGER_H
#define __INPUTMANAGER_H

#include <vector>

#include "InputSystemScene.h"

#include "../System/ISystem.hpp"
#include "../Configuration/IConfiguration.hpp"

#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>

/*!
	Receives all Input from the User(s) and translates it into Game Events
*/
class InputSystem : public ISystem, public OIS::KeyListener, public OIS::MouseListener
{

	typedef std::vector< InputSystemScene* > InputSystemSceneList;

public:

	~InputSystem( );

	InputSystem( Configuration::IConfiguration* configuration )
		: _inputAllowed( true )
		, _configuration( configuration )
		, _inputManager( 0 )
		, _keyboard( 0 )
		, _mouse( 0 )
	{

	}

	/* Initializes the Input System */
	void Initialize( );
	inline void Update( float deltaMilliseconds );

	/*! Releases the Input System */
	void Release( ) { };

	System::Types::Type GetType( ) { return System::Types::INPUT; };

	ISystemScene* CreateScene( );

	inline AnyValue::AnyValueMap GetProperties( ) { return _properties; };
	void SetProperty( const std::string& name, AnyValue value );

	ISystemComponent* CreateComponent( const std::string& name, System::Types::Type systemType ) { return 0; };

	/* Fired when the user presses a button on the keyboard */
	bool keyPressed( const OIS::KeyEvent &arg );

	/* Fired when the user releases a button on the keyboard */
	bool keyReleased( const OIS::KeyEvent &arg );

	/* Fired when the user moves the mouse */
	bool mouseMoved( const OIS::MouseEvent &arg );

	/* Fired when the user presses a button on the mouse */
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	/* Fired when the user releases a button on the mouse */
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

private:

	bool _inputAllowed;

	AnyValue::AnyValueMap _properties;
	Configuration::IConfiguration* _configuration;
	InputSystemSceneList _inputScenes;

	OIS::InputManager* _inputManager;
	OIS::Mouse* _mouse;
	OIS::Keyboard* _keyboard;

	InputSystem( ) { };
	InputSystem( const InputSystem & copy ) { };
	InputSystem & operator = ( const InputSystem & copy ) { return *this; };

};

#endif
