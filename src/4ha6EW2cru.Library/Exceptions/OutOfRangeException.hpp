#ifndef __OUTOFRANGEEXCEPTION_H
#define __OUTOFRANGEEXCEPTION_H

class OutOfRangeException : public std::exception
{

public:

	OutOfRangeException( )
		: std::exception( "A given argument was out of range" )
	{

	}

	OutOfRangeException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}
};

#endif