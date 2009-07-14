/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   UnInitializedException.hpp
*  @date   2009/04/25
*/
#ifndef UNINITIALIZEDEXCEPTION
#define UNINITIALIZEDEXCEPTION

namespace Exceptions{ };

/*! 
*  An Exception for when the system tries to access something that hasn't been initialized
*/
class UnInitializedException : public std::exception
{

public:

	/*! Default Destructor
	*
	*  @return ()
	*/
	~UnInitializedException( ) { };


	/*! Constructor with default error message
	*
	*  @return ()
	*/
	UnInitializedException( )
		: std::exception( "Object has not been Initialized" )
	{

	}


	/*! Constructor with custom error message
	*
	*  @param[in] const std::string message
	*  @return ()
	*/
	UnInitializedException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}

private:

	UnInitializedException( const UnInitializedException & copy ) { };
	UnInitializedException & operator = ( const UnInitializedException & copy ) { return *this; };

};

#endif
