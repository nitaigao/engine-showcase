#ifndef __PROPERTY_H
#define __PROPERTY_H

#include <vector>
#include <string>
#include <boost/any.hpp>

class SystemProperty
{

public:

	SystemProperty( ) { };
	
	SystemProperty( const std::string& name, const boost::any& value )
		: _name( name )
		, _value( value )
	{

	}

	~SystemProperty( ) { };

	std::string GetName( ) { return _name; };

	template< class T >
	T GetValue( ) { return boost::any_cast< T >( _value ); };

private:

	std::string _name;
	boost::any _value;

};

typedef std::vector< SystemProperty > SystemPropertyList;

#endif