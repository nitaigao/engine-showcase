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

	FileAppender( ) { };
	~FileAppender( ) { };

	/* Initialize the Appender */
	bool Initialize( const std::string filePath );

	/* Appends a message to the Target */
	bool Append( const std::string message ) const;

private:

	FileAppender( const FileAppender & copy ) { };
	FileAppender & operator = ( const FileAppender & copy ) { return *this; };

	std::string _filePath;

};

#endif