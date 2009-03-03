#ifndef __OGRESYSTEMSCENE_H
#define __OGRESYSTEMSCENE_H

#include "../System/ISystem.hpp"

class OgreSystemScene : public ISystemScene
{

public:

	virtual ~OgreSystemScene( ) { };

	OgreSystemScene( ISystem* system )
		: _system( system )
	{

	}

	ISystemComponent* CreateComponent( const std::string& name );
	void DestroyComponent( ISystemComponent* component );
	SystemType GetType( ) { return RenderSystemType; };

private:

	ISystem* _system;

};

#endif