#ifndef __ALREADYINITIALIZEDEXCEPTION_H
#define __ALREADYINITIALIZEDEXCEPTION_H

class AlreadyInitializedException : public std::exception
{

public:

	AlreadyInitializedException( )
		: std::exception( "Object has already been Initialized" )
	{

	}

	AlreadyInitializedException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}
};


#endif