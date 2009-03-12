#ifndef __INPUTSYSTEMSCENE_H
#define __INPUTSYSTEMSCENE_H

#include "../System/ISystem.hpp"
#include "../System/ISystemScene.hpp"

class InputSystemScene : public ISystemScene
{

public:

	virtual ~InputSystemScene( ) { };

	InputSystemScene( )
	{

	}

	ISystemComponent* CreateComponent( const std::string& name );
	void DestroyComponent( ISystemComponent* component );
	SystemType GetType( ) { return InputSystemType; };

};

#endif