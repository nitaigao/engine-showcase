#ifndef __MOCK_CONFIGURATION_H
#define __MOCK_CONFIGURATION_H

#include "mockpp/mockpp.h"
#include "mockpp/MockObject.h"
#include "mockpp/ExpectationCounter.h"

USING_NAMESPACE_MOCKPP

#include "System/IConfiguration.hpp"

class Mock_Configuration : public IConfiguration, public MockObject
{

public:

	Mock_Configuration( )
		: MockObject( "Mock_Configuration", 0 )
	{ };

	virtual ~Mock_Configuration( ) { };

	int Find( const std::string& section, const std::string& key, const int& defaultValue )
	{
		return defaultValue;
	}

	std::string Find( const std::string& section, const std::string& key, const std::string& defaultValue )
	{
		return defaultValue;
	}

	bool Find( const std::string& section, const std::string& key, const bool& defaultValue )
	{
		return defaultValue;
	}

	void Set( const std::string& section, const std::string& key, const std::string& value )
	{

	}

	void Set( const std::string& section, const std::string& key, const int& value )
	{

	}

	void Set( const std::string& section, const std::string& key, const bool& value )
	{

	}

private:

	Mock_Configuration & operator = ( const Mock_Configuration & copy ) { return *this; };
};

#endif