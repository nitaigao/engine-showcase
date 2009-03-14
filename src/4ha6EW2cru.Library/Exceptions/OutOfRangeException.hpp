#ifndef __OUTOFRANGEEXCEPTION_H
#define __OUTOFRANGEEXCEPTION_H

class OutOfRangeException : public std::exception
{

public:

	~OutOfRangeException( ) { };

	OutOfRangeException( )
		: std::exception( "A given argument was out of range" )
	{

	}

	OutOfRangeException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}

private:

	OutOfRangeException( const OutOfRangeException & copy ) { };
	OutOfRangeException & operator = ( const OutOfRangeException & copy ) { return *this; };

};

#endif
