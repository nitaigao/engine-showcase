#ifndef __INITAILIZEFAILED_EXCEPTION_H
#define __INITAILIZEFAILED_EXCEPTION_H

class IntializeFailedException : public std::exception
{

public:

	IntializeFailedException( )
		: std::exception( "Component was unable to initialize correctly" )
	{

	}

	IntializeFailedException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}
};

#endif
