#ifndef __FILEACCESSEXCEPTION_H
#define __FILEACCESSEXCEPTION_H

class FileWriteException : public std::exception
{

public:

	~FileWriteException( ) { };

	FileWriteException( )
		: std::exception( "There was an error attempting to write to a file" )
	{

	}

	FileWriteException( const std::string message )
		: std::exception( message.c_str( ) )
	{

	}

private:

	FileWriteException( const FileWriteException & copy ) { };
	FileWriteException & operator = ( const FileWriteException & copy ) { return *this; };

};

#endif
