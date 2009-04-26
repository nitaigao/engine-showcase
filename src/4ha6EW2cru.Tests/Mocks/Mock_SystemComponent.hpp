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

	Mock_SystemComponent( System::Types::Type systemType )
		: MockObject( "Mock_SystemComponent", 0 )
		, _systemType( systemType )
	{ 
	
	};
	
	void Initialize( AnyValue::AnyValueMap properties ) { };
	void Update( float deltaMilliseconds ){ };
	void Destroy( ) { };

	void AddObserver( IObserver* observer ) { } ;
	void PushChanges( unsigned int systemChanges ) { };
	
	inline unsigned int GetRequestedChanges( ) { return 0; };
	void Observe( ISubject* subject, unsigned int systemChanges ) { };

	inline const std::string& GetName( ) { return "test"; }
	inline System::Types::Type GetType( ) { return _systemType; };

	inline AnyValue::AnyValueMap GetProperties( ) { return AnyValue::AnyValueMap( ); };
	inline void SetProperties( AnyValue::AnyValueMap systemProperties ) { };

	inline MathVector3 GetPosition( ) { return MathVector3::Zero( ); };
	inline MathVector3 GetScale( ) { return MathVector3::Zero( ); };
	inline MathQuaternion GetOrientation( ) { return MathQuaternion::Identity( ); };

private:

	Mock_SystemComponent & operator = ( const Mock_SystemComponent & copy ) { return *this; };

	System::Types::Type _systemType;
};


#endif