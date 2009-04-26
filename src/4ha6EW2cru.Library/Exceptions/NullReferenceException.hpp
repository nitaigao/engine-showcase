/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   c:\Users\NK\Development\viper\src\4ha6EW2cru.Library\Exceptions\NullReferenceException.hpp
*  @date   2009/04/25
*/
#ifndef __NULLREFERENCEEXCEPTION_H
#define __NULLREFERENCEEXCEPTION_H

#include <exception>
#include <string>

/*! 
*  An Exception for when the system encounters a Null Reference
*/
class NullReferenceException : public std::exception
{

public:

	/*! Default Destructor
	*
	*  @return ()
	*/
	~NullReferenceException( ) { };


	/*! Constructor with default error message
	*
	*  @return ()
	*/
	NullReferenceException( )
		: std::exception( "The referenced object is NULL" )
	{

	}


	/*! Constructor with custom error message
	*
	*  @param[in] const std::string message
	*  @return ()
	*/
	NullReferenceException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}

private:

	NullReferenceException( const NullReferenceException & copy ) { };
	NullReferenceException & operator = ( const NullReferenceException & copy ) { return *this; };

};

#endif
