#ifndef __INPUTSYSTEMSCENE_H
#define __INPUTSYSTEMSCENE_H

class InputSystemScene : public ISystemScene
{

public:

	virtual ~InputSystemScene( ) { };

	InputSystemScene( ISystem* system )
		: _system( system )
	{

	}

	ISystemComponent* CreateComponent( const std::string& name ) { return 0; };
	void DestroyComponent( ISystemComponent* component ) { };
	SystemType GetType( ) { return InputSystemType; };

private:

	ISystem* _system;

};

#endif