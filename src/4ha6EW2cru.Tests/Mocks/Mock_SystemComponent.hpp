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

	Mock_SystemComponent( SystemType systemType )
		: MockObject( "Mock_SystemComponent", 0 )
		, _systemType( systemType )
	{ 
	
	};

	void Initialize( SystemPropertyList properties ) { };
	void AddObserver( IObserver* observer ) { } ;
	
	void Observe( ISubject* subject ) { };
	void PushChanges( unsigned int systemChanges ) { };

	inline const std::string& GetName( ) { return "test"; }
	inline SystemType GetType( ) { return _systemType; };

private:

	Mock_SystemComponent & operator = ( const Mock_SystemComponent & copy ) { return *this; };

	SystemType _systemType;
};


#endif