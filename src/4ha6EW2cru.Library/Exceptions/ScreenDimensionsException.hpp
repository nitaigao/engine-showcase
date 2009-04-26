/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   c:\Users\NK\Development\viper\src\4ha6EW2cru.Library\Exceptions\ScreenDimensionsException.hpp
*  @date   2009/04/25
*/
#ifndef __SCREENDIMENSIONSEXCEPTION
#define __SCREENDIMENSIONSEXCEPTION

/*! 
*  An Exception for when the game cannotachieve a given resolution
*/
class ScreenDimensionsException : public std::exception
{

public:

	/*! Default Destructor
	*
	*  @return ()
	*/
	~ScreenDimensionsException( ) { };


	/*! Constructor with default error message
	*
	*  @return ()
	*/
	ScreenDimensionsException( )
		: std::exception( "The Screen parameters supplied are invalid" )
	{

	}


	/*! Constructor with custom error message
	*
	*  @param[in] const std::string message
	*  @return ()
	*/
	ScreenDimensionsException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}

private:

	ScreenDimensionsException( const ScreenDimensionsException & copy ) { };
	ScreenDimensionsException & operator = ( const ScreenDimensionsException & copy ) { return *this; };

};

#endif
