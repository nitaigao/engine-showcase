#include "FileAppender.h"

#include <fstream>
#include <ostream>
#include <sstream>

bool FileAppender::Initialize( const std::string filePath )
{
	_filePath = filePath;

	std::ofstream logFile;
	logFile.open( _filePath.c_str( ), std::fstream::out | std::fstream::app );

	if ( logFile.is_open( ) )
	{
		logFile.close( );
		
		return true;
	}

	return false;
}

bool FileAppender::Append( const std::string message ) const
{
	std::ofstream logFile;
	logFile.open( _filePath.c_str( ), std::fstream::out | std::fstream::app );

	if ( logFile.is_open( ) )
	{
		logFile << message.c_str( );
		logFile.close( );

		return true;
	}

	return false;
}