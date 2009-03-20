#ifndef __PROPERTY_H
#define __PROPERTY_H

#include <map>
#include <string>
#include <boost/any.hpp>

class SystemProperty
{

public:

	virtual ~SystemProperty( ) { };
	
	SystemProperty( ) { };

	SystemProperty( const std::string& name, const boost::any& value )
		: _name( name )
		, _value( value )
	{

	}

	std::string GetName( ) { return _name; };

	template< class T >
	T GetValue( ) { return boost::any_cast< T >( _value ); };

private:

	std::string _name;
	boost::any _value;

};

typedef std::map< std::string, SystemProperty > SystemPropertyList;

#endif