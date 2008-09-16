#ifndef __SCREENDIMENSIONSEXCEPTION
#define __SCREENDIMENSIONSEXCEPTION

class ScreenDimensionsException : public std::exception
{

public:

	ScreenDimensionsException( )
		: std::exception( "The Screen parameters supplied are invalid" )
	{

	}

	ScreenDimensionsException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}

};

#endif