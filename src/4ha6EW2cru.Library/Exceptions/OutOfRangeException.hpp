/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   OutOfRangeException.hpp
*  @date   2009/04/25
*/
#ifndef __OUTOFRANGEEXCEPTION_H
#define __OUTOFRANGEEXCEPTION_H

/*! 
*  An Exception for when the system goes out of range
*/
class OutOfRangeException : public std::exception
{

public:

	/*! Default Destructor
	*
	*  @return ()
	*/
	~OutOfRangeException( ) { };


	/*! Constructor with default error message
	*
	*  @return ()
	*/
	OutOfRangeException( )
		: std::exception( "A given argument was out of range" )
	{

	}

	/*! Constructor with custom error message
	*
	*  @param[in] const std::string message
	*  @return ()
	*/
	OutOfRangeException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}

private:

	OutOfRangeException( const OutOfRangeException & copy ) { };
	OutOfRangeException & operator = ( const OutOfRangeException & copy ) { return *this; };

};

#endif
