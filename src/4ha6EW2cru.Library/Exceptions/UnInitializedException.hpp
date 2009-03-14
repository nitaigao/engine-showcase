#ifndef __UNINITIALIZEDEXCEPTION
#define __UNINITIALIZEDEXCEPTION

class UnInitializedException : public std::exception
{

public:

	~UnInitializedException( ) { };

	UnInitializedException( )
		: std::exception( "Object has not been Initialized" )
	{

	}

	UnInitializedException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}

private:

	UnInitializedException( const UnInitializedException & copy ) { };
	UnInitializedException & operator = ( const UnInitializedException & copy ) { return *this; };

};

#endif
