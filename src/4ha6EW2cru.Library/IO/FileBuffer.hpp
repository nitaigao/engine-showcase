#ifndef __FILEBUFFER_H
#define __FILEBUFFER_H

#include <string>

/*!
	A container for file stream information
*/
struct FileBuffer
{

public:

	FileBuffer( )
		: fileBytes( new char[ ] )
		, fileSize( 0 )
		, filePath( "" )
	{

	}

	~FileBuffer( )
	{
		delete fileBytes;
	}

	char* fileBytes;
	int fileSize;
	std::string filePath;


private:

	FileBuffer( const FileBuffer & copy ) { };
	FileBuffer & operator = ( const FileBuffer & copy ) { return *this; };

};

#endif