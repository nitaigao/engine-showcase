#ifndef __NULLREFERENCEEXCEPTION_H
#define __NULLREFERENCEEXCEPTION_H

#include <exception>
#include <string>

class NullReferenceException : public std::exception
{

public:

	~NullReferenceException( ) { };

	NullReferenceException( )
		: std::exception( "The referenced object is NULL" )
	{

	}

	NullReferenceException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}

private:

	NullReferenceException( const NullReferenceException & copy ) { };
	NullReferenceException & operator = ( const NullReferenceException & copy ) { return *this; };

};

#endif
