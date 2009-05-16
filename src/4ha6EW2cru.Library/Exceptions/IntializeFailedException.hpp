/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IntializeFailedException.hpp
*  @date   2009/04/25
*/
#ifndef INITAILIZEFAILED_EXCEPTION_H
#define INITAILIZEFAILED_EXCEPTION_H

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
