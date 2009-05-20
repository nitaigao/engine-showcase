#ifndef MOCK_SYSTEMENTITY_H
#define MOCK_SYSTEMENTITY_H

#include "mockpp/mockpp.h"
#include "mockpp/MockObject.h"
#include "mockpp/ExpectationCounter.h"

USING_NAMESPACE_MOCKPP

#include "System/ISystemComponent.hpp"
using namespace Maths;

class Mock_SystemComponent : public ISystemComponent, public MockObject
{

public:

	Mock_SystemComponent( System::Types::Type systemType )
		: MockObject( "Mock_SystemComponent", 0 )
		, _systemType( systemType )
	{ 
	
	};
	
	void Initialize( ) { };
	void Update( const float& deltaMilliseconds ){ };
	void Destroy( ) { };

	void AddObserver( IObserver* observer ) { } ;
	void PushChanges( const unsigned int& systemChanges ) { };
	
	inline unsigned int GetRequestedChanges( ) const  { return 0; };
	void Observe( ISubject* subject, const unsigned int& systemChanges ) { };

	inline const std::string& GetName( ) const { return "test"; }
	inline void SetId( const unsigned int& id ) {  };
	inline unsigned int GetId( ) const { return 0; };
	inline System::Types::Type GetType( ) const { return _systemType; };

	AnyValue::AnyValueKeyMap GetAttributes( ) const { return AnyValue::AnyValueKeyMap( ); };

	inline void SetAttribute( const unsigned int& attributeId, const AnyValue& value ) {  };

	inline MathVector3 GetPosition( ) const { return MathVector3::Zero( ); };
	inline MathVector3 GetScale( ) const { return MathVector3::Zero( ); };
	inline MathQuaternion GetOrientation( ) const { return MathQuaternion::Identity( ); };

	inline AnyValue PushMessage( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters ) { return AnyValue( ); };
	AnyValue Message( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters ) { return AnyValue( 0 ); };

private:

	Mock_SystemComponent & operator = ( const Mock_SystemComponent & copy ) { return *this; };

	System::Types::Type _systemType;
};


#endif