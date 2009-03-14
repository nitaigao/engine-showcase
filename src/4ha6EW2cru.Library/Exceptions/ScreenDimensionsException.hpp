#ifndef __SCREENDIMENSIONSEXCEPTION
#define __SCREENDIMENSIONSEXCEPTION

class ScreenDimensionsException : public std::exception
{

public:

	~ScreenDimensionsException( ) { };

	ScreenDimensionsException( )
		: std::exception( "The Screen parameters supplied are invalid" )
	{

	}

	ScreenDimensionsException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}

private:

	ScreenDimensionsException( const ScreenDimensionsException & copy ) { };
	ScreenDimensionsException & operator = ( const ScreenDimensionsException & copy ) { return *this; };

};

#endif
