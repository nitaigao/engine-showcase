#ifndef __GEOMETRYSYSTEMSCENE_H
#define __GEOMETRYSYSTEMSCENE_H

#include "../System/ISystem.hpp"

class GeometrySystemScene : public ISystemScene
{

public:

	virtual ~GeometrySystemScene( ) { };

	GeometrySystemScene( ISystem* system )
		: _system( system )
	{

	}

	ISystemComponent* CreateComponent( const std::string& name );
	void DestroyComponent( ISystemComponent* component ) { };
	SystemType GetType( ) { return GeometrySystemType; };

private:

	ISystem* _system;

};

#endif