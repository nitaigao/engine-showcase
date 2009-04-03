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
	inline void Update( float deltaMilliseconds ) { };
	inline void Release( ) { };

	ISystemScene* CreateScene( );
	inline SystemType GetSystemType( ) { return GeometrySystemType; };
	
	inline AnyValueMap GetProperties( ) { return AnyValueMap( ); };
	inline void SetProperty( const std::string& name, AnyValue value ) { };

private:

	GeometrySystem( const GeometrySystem & copy ) { };
	GeometrySystem & operator = ( const GeometrySystem & copy ) { return *this; };

};


#endif