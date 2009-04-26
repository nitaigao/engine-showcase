#ifndef __HAVOKPHYSICS_SYSTEM
#define __HAVOKPHYSICS_SYSTEM

#include "../Service/IService.hpp"
#include "../System/ISystem.hpp"
#include "HavokPhysicsSystemScene.h"

class HavokPhysicsSystem : public ISystem, public IService
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
	inline void Update( float deltaMilliseconds ) { };
	void Release( );

	inline System::Types::Type GetType( ) { return System::Types::PHYSICS; };

	ISystemScene* CreateScene( );

	inline AnyValue::AnyValueMap GetProperties( ) { return AnyValue::AnyValueMap( ); };
	inline void SetProperty( const std::string& name, AnyValue value ) { };

	AnyValue::AnyValueMap Execute( const std::string& actionName, AnyValue::AnyValueMap parameters );
	std::vector< std::string > RayQuery( MathVector3 origin, MathVector3 direction, bool sortByDistance );

private:

	static void errorReportFunction( const char* str, void* errorOutputObject );

	hkThreadMemory* _threadMemory;
	char* _stackBuffer;

	SceneList _scenes;

	HavokPhysicsSystem( const HavokPhysicsSystem & copy ) { };
	HavokPhysicsSystem & operator = ( const HavokPhysicsSystem & copy ) { return *this; };

};

#endif