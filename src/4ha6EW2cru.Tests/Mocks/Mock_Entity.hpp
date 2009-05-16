#ifndef MOCK_ENTITY_H
#define MOCK_ENTITY_H

#include "mockpp/mockpp.h"
#include "mockpp/MockObject.h"
#include "mockpp/ExpectationCounter.h"

USING_NAMESPACE_MOCKPP


class Mock_Entity : public IWorldEntity, public MockObject
{

public:

	~Mock_Entity( ) { };

	Mock_Entity( )
		: MockObject( "Mock_Entity", 0 )
	{

	}

	inline unsigned int GetId( ) const { return 0; };

	const std::string& GetName( ) const { return "test_entity"; };
	void AddComponent( ISystemComponent* component ) { _components.push_back( component ); }
	ISystemComponent* FindComponent( const System::Types::Type& systemType ) const { return 0; };
	SystemComponentList GetComponents( ) const { return _components; };

private:

	Mock_Entity & operator = ( const Mock_Entity & copy ) { return *this; };

	SystemComponentList _components;
};

#endif