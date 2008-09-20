#ifndef __FILENOTFOUNDEXCEPTION
#define __FILENOTFOUNDEXCEPTION

class FileNotFoundException : public std::exception
{

public:

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