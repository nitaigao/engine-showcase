#ifndef __ARCHIVENOTFOUNDEXCEPTION_H
#define __ARCHIVENOTFOUNDEXCEPTION_H

class ArchiveNotFoundException : public std::exception
{

public:

	ArchiveNotFoundException( )
		: std::exception( "There was an error attempting to open an BAD Archive" )
	{

	}

	ArchiveNotFoundException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}
};

#endif