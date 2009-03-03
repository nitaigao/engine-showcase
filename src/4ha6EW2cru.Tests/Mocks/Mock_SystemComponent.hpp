#ifndef __MOCK_SYSTEMENTITY_H
#define __MOCK_SYSTEMENTITY_H

#include "mockpp/mockpp.h"
#include "mockpp/MockObject.h"
#include "mockpp/ExpectationCounter.h"

USING_NAMESPACE_MOCKPP

#include "System/ISystemComponent.hpp"

class Mock_SystemComponent : public ISystemComponent, public MockObject
{

public:

	Mock_SystemComponent( )
		: MockObject( "Mock_SystemComponent", 0 )
	{ 
	
	};

	SystemType GetType( ) { return SystemType::TestSystemType; };
	void Initialize( SystemPropertyList properties ) { };

private:

	Mock_SystemComponent & operator = ( const Mock_SystemComponent & copy ) { return *this; };
};


#endif