#ifndef __INITAILIZEFAILED_EXCEPTION_H
#define __INITAILIZEFAILED_EXCEPTION_H

class IntializeFailedException : public std::exception
{

public:

	~IntializeFailedException( ) { };

	IntializeFailedException( )
		: std::exception( "Component was unable to initialize correctly" )
	{

	}

	IntializeFailedException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}

private:

	IntializeFailedException( const IntializeFailedException & copy ) { };
	IntializeFailedException & operator = ( const IntializeFailedException & copy ) { return *this; };

};

#endif
