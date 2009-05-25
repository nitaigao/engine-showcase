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

	AnyType::AnyTypeMap GetAttributes( ) const { return AnyType::AnyTypeMap( ); };

	inline void SetAttribute( const System::Attribute& attributeId, const AnyType& value ) {  };

	inline MathVector3 GetPosition( ) const { return MathVector3::Zero( ); };
	inline MathVector3 GetScale( ) const { return MathVector3::Zero( ); };
	inline MathQuaternion GetOrientation( ) const { return MathQuaternion::Identity( ); };

	inline AnyType PushMessage( const System::Message& message, AnyType::AnyTypeMap parameters ) { return AnyType( ); };
	AnyType Message( const System::Message& message, AnyType::AnyTypeMap parameters ) { return AnyType( 0 ); };

private:

	Mock_SystemComponent & operator = ( const Mock_SystemComponent & copy ) { return *this; };

	System::Types::Type _systemType;
};


#endif