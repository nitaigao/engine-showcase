#ifndef __PROPERTY_H
#define __PROPERTY_H

#include <string>
#include <windows.h>

struct PropertyValue
{
	size_t Size;
};

class Property
{

public:

	Property( ) { };
	~Property( ) { };

	std::string GetName( ) { return _name; };
	PropertyValue GetValue( ) { return _value; };

	void SetValue( size_t value ) { _value.Size = value; };

private:

	std::string _name;
	PropertyValue _value;

};

#endif