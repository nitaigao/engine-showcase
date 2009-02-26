#ifndef __MOCK_WORLD_H
#define __MOCK_WORLD_H

#include "mockpp/mockpp.h"
#include "mockpp/MockObject.h"
#include "mockpp/ExpectationCounter.h"

USING_NAMESPACE_MOCKPP

#include "Logic/IWorld.hpp"

class Mock_World : public IWorld, public MockObject
{

public:

	Mock_World( )
		: MockObject( "Mock_World", 0 )
	{ };


private:

	Mock_World & operator = ( const Mock_World & copy ) { return *this; };
};


#endif