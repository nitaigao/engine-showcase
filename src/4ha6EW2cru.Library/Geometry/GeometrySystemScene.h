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

	void Initialize( ) { };

	ISystemComponent* CreateComponent( const std::string& name, const std::string& type );
	void DestroyComponent( ISystemComponent* component );
	SystemType GetType( ) { return GeometrySystemType; };
	inline void Update( float deltaMilliseconds ) { };

private:

	ISystem* _system;

	GeometrySystemScene( ) { };
	GeometrySystemScene( const GeometrySystemScene & copy ) { };
	GeometrySystemScene & operator = ( const GeometrySystemScene & copy ) { return *this; };

};

#endif