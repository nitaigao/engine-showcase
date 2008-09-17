#ifndef __NULLREFERENCEEXCEPTION_H
#define __NULLREFERENCEEXCEPTION_H

class NullReferenceException : public std::exception
{

public:

	NullReferenceException( )
		: std::exception( "The referenced object is NULL" )
	{

	}

	NullReferenceException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}
};

#endif