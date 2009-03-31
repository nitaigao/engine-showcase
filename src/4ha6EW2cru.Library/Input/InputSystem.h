#ifndef __INPUTMANAGER_H
#define __INPUTMANAGER_H

#include "../System/ISystem.hpp"
#include "../System/Configuration.h"

/*!
	Receives all Input from the User(s) and translates it into Game Events
*/
class InputSystem : public ISystem
{

public:

	~InputSystem( );

	InputSystem( Configuration* configuration )
		: _configuration( configuration )
	{

	};

	/* Initializes the Input System */
	void Initialize( );

	/*! Releases the Input System */
	void Release( ) { };

	ISystemScene* CreateScene( );

	SystemType GetSystemType( ) { return InputSystemType; };

	inline PropertyMap GetProperties( ) { return _properties; };
	inline void SetProperties( PropertyMap properties );

	ISystemComponent* CreateComponent( const std::string& name, SystemType systemType ) { return 0; };

private:

	PropertyMap _properties;
	Configuration* _configuration;

	SystemSceneList _systemScenes;

	InputSystem( ) { };
	InputSystem( const InputSystem & copy ) { };
	InputSystem & operator = ( const InputSystem & copy ) { return *this; };
};

#endif
