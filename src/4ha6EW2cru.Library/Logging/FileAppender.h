#ifndef __FILEAPPENDER_H
#define __FILEAPPENDER_H

#include <string>

#include "IAppender.hpp"

/*! 
	Appends a Log Message to a file 
*/
class FileAppender : public IAppender
{

public:

	FileAppender( const std::string filePath )
		: _isIntialized( false )
		, _filePath( filePath )
	{

	}

	~FileAppender( )
	{

	}

	/* Initialize the Appender */
	void Initialize( );

	/* Appends a message to the Target */
	void Append( const std::string message ) const;

private:

	FileAppender( const FileAppender & copy ) { };
	FileAppender & operator = ( const FileAppender & copy ) { return *this; };

	std::string _filePath;
	bool _isIntialized;

};

#endif