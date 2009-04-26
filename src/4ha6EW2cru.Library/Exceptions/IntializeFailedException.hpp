/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   c:\Users\NK\Development\viper\src\4ha6EW2cru.Library\Exceptions\IntializeFailedException.hpp
*  @date   2009/04/25
*/
#ifndef __INITAILIZEFAILED_EXCEPTION_H
#define __INITAILIZEFAILED_EXCEPTION_H

/*! 
*  An Exception for when a component fails to initialize properly
*/
class IntializeFailedException : public std::exception
{

public:

	/*! Default Destructor
	*
	*  @return ()
	*/
	~IntializeFailedException( ) { };


	/*! Constructor with default error message
	*
	*  @return ()
	*/
	IntializeFailedException( )
		: std::exception( "Component was unable to initialize correctly" )
	{

	}


	/*! Constructor with custom error message
	*
	*  @param[in] const std::string message
	*  @return ()
	*/
	IntializeFailedException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}

private:

	IntializeFailedException( const IntializeFailedException & copy ) { };
	IntializeFailedException & operator = ( const IntializeFailedException & copy ) { return *this; };

};

#endif
