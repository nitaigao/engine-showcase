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

	Mock_SystemScene( )
		: MockObject( "Mock_SystemScene", 0 )
		, createEntity_count( "Mock_SystemScene/CreateComponent", this )
		, destroyEntity_count( "Mock_SystemScene/DestroyComponent", this )
	{ };

	ExpectationCounter createEntity_count;
	ExpectationCounter destroyEntity_count;

	

	ISystemComponent* CreateComponent( const std::string& name )
	{
		createEntity_count.inc( );
		return new Mock_SystemComponent( );
	}

	void DestroyComponent( ISystemComponent* component )
	{
		destroyEntity_count.inc( );
		delete component;
	}

	inline SystemType GetType( ) { return SystemType::TestSystemType; };

private:

	Mock_SystemScene & operator = ( const Mock_SystemScene & copy ) { return *this; };
};

#endif