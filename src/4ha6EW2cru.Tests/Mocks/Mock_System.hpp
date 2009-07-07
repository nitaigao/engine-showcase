#ifndef MOCK_SYSTEM_H
#define MOCK_SYSTEM_H

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

	Mock_System( System::Types::Type systemType )
		: MockObject( "Mock_System", 0 )
		, initialize_count( "Mock_System/Initialize", this )
		, release_count( "Mock_System/Release", this )
		, createScene_count( "Mock_System/CreateScene", this )
		, _systemType( systemType )
	{ };

	ExpectationCounter initialize_count;
	ExpectationCounter release_count;
	ExpectationCounter createScene_count;

	void Initialize( Configuration::IConfiguration* configuration )
	{
		initialize_count.inc( );
	}

	void Update( const float& deltaMilliseconds )
	{

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

	System::Types::Type GetType( ) const { return _systemType; };

	inline void Message( const std::string& message, AnyType::AnyTypeMap parameters ) { };

	AnyType::AnyTypeMap GetAttributes( ) const { return AnyType::AnyTypeMap( ); };
	inline void SetAttribute( const std::string& name, AnyType value ) { };

private:

	Mock_System & operator = ( const Mock_System & copy ) { return *this; };

	System::Types::Type _systemType;
};


#endif