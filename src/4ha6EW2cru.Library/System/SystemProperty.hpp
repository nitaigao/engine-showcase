#ifndef __ANYVALUE_H
#define __ANYVALUE_H

#include <map>
#include <string>
#include <boost/any.hpp>

class AnyValue
{

public:

	typedef std::map< std::string, AnyValue > AnyValueMap;

	~AnyValue( ) { };
	AnyValue( ) { };

	AnyValue( const int& value )
		: _value( value )
	{

	}

	AnyValue( const char* value )
		: _value( std::string( value ) )
	{

	}

	template< class T >
	AnyValue( const T& value )
		: _value( value )
	{

	}

	template< class T >
	T GetValue( ) { return boost::any_cast< T >( _value ); };

private:

	boost::any _value;

};

typedef std::map< std::string, AnyValue > AnyValueMap;

#endif