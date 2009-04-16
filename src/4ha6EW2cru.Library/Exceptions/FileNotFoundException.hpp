#ifndef __FILENOTFOUNDEXCEPTION
#define __FILENOTFOUNDEXCEPTION

#include <string>

class FileNotFoundException : public std::exception
{

public:

	~FileNotFoundException( ) { };

	FileNotFoundException( const FileNotFoundException & copy ) { };
	FileNotFoundException & operator = ( const FileNotFoundException & copy ) { return *this; };

	FileNotFoundException( )
		: std::exception( "There was an error attempting to open an File" )
	{

	}

	FileNotFoundException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}
};

#endif
