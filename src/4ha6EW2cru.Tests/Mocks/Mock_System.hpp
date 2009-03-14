#ifndef __MOCK_SYSTEM_H
#define __MOCK_SYSTEM_H

#include "mockpp/mockpp.h"
#include "mockpp/MockObject.h"
#include "mockpp/ExpectationCounter.h"

USING_NAMESPACE_MOCKPP

#include "System/ISystem.hpp"

#include "../Mocks/Mock_SystemScene.hpp"

class Mock_System : public ISystem, public MockObject
{

public:

	virtual ~Mock_System( ) { };

	Mock_System( SystemType systemType )
		: MockObject( "Mock_System", 0 )
		, initialize_count( "Mock_System/Initialize", this )
		, release_count( "Mock_System/Release", this )
		, createScene_count( "Mock_System/CreateScene", this )
		, _systemType( systemType )
	{ };

	ExpectationCounter initialize_count;
	ExpectationCounter release_count;
	ExpectationCounter createScene_count;

	void Initialize( )
	{
		initialize_count.inc( );
	}

	void Release( )
	{
		release_count.inc( );
	}

	ISystemScene* CreateScene( )
	{
		createScene_count.inc( );
		return new Mock_SystemScene( _systemType );
	}

	SystemType GetSystemType( ) { return _systemType; };
	PropertyMap GetProperties( ) { return PropertyMap( ); };

private:

	Mock_System & operator = ( const Mock_System & copy ) { return *this; };

	SystemType _systemType;
};


#endif