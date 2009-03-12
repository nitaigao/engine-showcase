#ifndef __HAVOKPHYSICS_SYSTEM
#define __HAVOKPHYSICS_SYSTEM

#include "..\System\ISystem.hpp"
#include "HavokPhysicsSystemScene.h"

class HavokPhysicsSystem : public ISystem
{

	typedef std::vector< HavokPhysicsSystemScene* > SceneList;

public:

	virtual ~HavokPhysicsSystem( ) { };

	HavokPhysicsSystem( )
		: _threadMemory( 0 )
		, _stackBuffer( 0 )
	{

	}

	void Initialize( );
	void Update( float deltaMilliseconds );
	void Release( );

	ISystemScene* CreateScene( );

	SystemType GetSystemType( ) { return PhysicsSystemType; };
	PropertyMap GetProperties( ) { return PropertyMap( ); };

private:

	HavokPhysicsSystem( const HavokPhysicsSystem & copy ) { };
	HavokPhysicsSystem & operator = ( const HavokPhysicsSystem & copy ) { return *this; };

	static void errorReportFunction( const char* str, void* errorOutputObject );

	hkThreadMemory* _threadMemory;
	char* _stackBuffer;

	SceneList _scenes;

};

#endif