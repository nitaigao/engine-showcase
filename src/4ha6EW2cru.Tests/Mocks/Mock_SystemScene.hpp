#ifndef __MOCK_SYSTEMWORLD_H
#define __MOCK_SYSTEMWORLD_H

#include "mockpp/mockpp.h"
#include "mockpp/MockObject.h"
#include "mockpp/ExpectationCounter.h"

USING_NAMESPACE_MOCKPP

#include "System/ISystemScene.hpp"

#include "Mock_SystemComponent.hpp"

class Mock_SystemScene : public ISystemScene, public MockObject
{

public:

	~Mock_SystemScene( ) { };

	Mock_SystemScene( SystemType systemType )
		: MockObject( "Mock_SystemScene", 0 )
		, createEntity_count( "Mock_SystemScene/CreateComponent", this )
		, destroyEntity_count( "Mock_SystemScene/DestroyComponent", this )
		, update_count( "Mock_SystemScene/Update", this )
		, _systemType( systemType )
	{ 
	
	};

	ExpectationCounter createEntity_count;
	ExpectationCounter destroyEntity_count;
	ExpectationCounter update_count;

	ISystemComponent* CreateComponent( const std::string& name )
	{
		createEntity_count.inc( );
		return new Mock_SystemComponent( _systemType );
	}

	void DestroyComponent( ISystemComponent* component )
	{
		destroyEntity_count.inc( );
		delete component;
	}

	inline SystemType GetType( ) { return _systemType; };
	inline void Update( float deltaMilliseconds )
	{
		update_count.inc( );
	}

private:

	Mock_SystemScene & operator = ( const Mock_SystemScene & copy ) { return *this; };

	SystemType _systemType;
};

#endif