#ifndef __MOCK_WORLD_H
#define __MOCK_WORLD_H

#include "mockpp/mockpp.h"
#include "mockpp/MockObject.h"
#include "mockpp/ExpectationCounter.h"

USING_NAMESPACE_MOCKPP

#include "State/IWorld.hpp"

#include "Mock_SystemComponent.hpp"
#include "Mock_Entity.hpp"

class Mock_World : public IWorld, public MockObject
{

public:

	~Mock_World( ) { };

	Mock_World( )
		: MockObject( "Mock_World", 0 )
		, createEntity_count( "Mock_World/CreateComponent", this )
		, destroyEntity_count( "Mock_World/DestroyComponent", this )
		, extend_count( "Mock_World/RegisterSystem", this )
	{ };

	ExpectationCounter createEntity_count;
	ExpectationCounter destroyEntity_count;
	ExpectationCounter extend_count;

	IEntity* CreateEntity( const std::string& name )
	{
		return new Mock_Entity( );
	}

	void DestroyEntity( IEntity* entity )
	{

	}

	void RegisterSystem( ISystem* system )
	{
		_systemScenes[ system->GetSystemType( ) ] = system->CreateScene( );
		extend_count.inc( );
	}

	const SystemSceneList& GetSystemScenes( ) { return _systemScenes; };

private:

	SystemSceneList _systemScenes;

private:

	Mock_World & operator = ( const Mock_World & copy ) { return *this; };
};

#endif