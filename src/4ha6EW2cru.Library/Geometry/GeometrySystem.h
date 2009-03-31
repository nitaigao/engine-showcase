#ifndef __GEOMETRYSYSTEM_H
#define __GEOMETRYSYSTEM_H

#include "../System/SystemType.hpp"
#include "../System/ISystem.hpp"
#include "../System/ISystemScene.hpp"

class GeometrySystem : public ISystem
{

public:

	virtual ~GeometrySystem( ) { };
	GeometrySystem( ) { };

	inline void Initialize( ) { };
	inline void Release( ) { };

	ISystemScene* CreateScene( );
	inline SystemType GetSystemType( ) { return GeometrySystemType; };
	
	inline PropertyMap GetProperties( ) { return PropertyMap( ); };
	inline void SetProperties( PropertyMap properties ) { };

private:

	GeometrySystem( const GeometrySystem & copy ) { };
	GeometrySystem & operator = ( const GeometrySystem & copy ) { return *this; };

};


#endif