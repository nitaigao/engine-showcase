#ifndef MOCK_INPUTSYSTEMCOMPONENT_H
#define MOCK_INPUTSYSTEMCOMPONENT_H

#include "mockpp/mockpp.h"
#include "mockpp/MockObject.h"
#include "mockpp/ExpectationCounter.h"

USING_NAMESPACE_MOCKPP

#include "System/ISystemComponent.hpp"

class Mock_InputSystemComponent : public InputSystemComponent, public MockObject
{

public:

	Mock_InputSystemComponent( const std::string& name )
		: MockObject( "Mock_InputSystemComponent", 0 )
		, keyDown_count( "Mock_InputSystemComponent/KeyDown", this )
		, keyUp_count( "Mock_InputSystemComponent/KeyUp", this )
		, _systemType( System::Types::INPUT )
		, InputSystemComponent( name )
	{ 

	};

	ExpectationCounter keyDown_count;
	ExpectationCounter keyUp_count;

	void KeyDown( const OIS::KeyCode& keyCode, const std::string& keyText )
	{
		keyDown_count.inc( );
	}

	void KeyUp( const OIS::KeyCode& keyCode, const std::string& keyText )
	{
		keyUp_count.inc( );
	}

	void Initialize( AnyType::AnyTypeMap& properties ) { };
	void Destroy( ) { };

	void AddObserver( IObserver* observer ) { } ;
	void PushChanges( const unsigned int& systemChanges ) { };

	inline unsigned int GetRequestedChanges( ) const  { return 0; };
	void Observe( ISubject* subject, const unsigned int& systemChanges ) { };

	inline const std::string& GetName( ) const { return "test"; }
	inline System::Types::Type GetType( ) const { return _systemType; };

private:

	Mock_InputSystemComponent & operator = ( const Mock_InputSystemComponent & copy ) { return *this; };

	System::Types::Type _systemType;
	std::string m_name;
};


#endif