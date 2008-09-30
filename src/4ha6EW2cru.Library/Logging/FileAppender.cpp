#include "FileAppender.h"

#include <fstream>
#include <ostream>
#include <sstream>

#include "../Exceptions/AlreadyInitializedException.hpp"
#include "../Exceptions/FileWriteException.hpp"
#include "../Logging/Logger.h"

void FileAppender::Initialize( )
{
	if( _isIntialized )
	{
		AlreadyInitializedException e( "FileAppender::Initialize - Attempted to Initialized an already initailized File" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	std::ofstream logFile;
	logFile.open( _filePath.c_str( ), std::fstream::out );

	if ( !logFile.is_open( ) )
	{
		FileWriteException ioE( "FileAppender::Initialize - Cannot write to log file" );
		Logger::GetInstance( )->Fatal( ioE.what( ) );
		throw ioE;
	}

	logFile.close( );
		
	_isIntialized = true;
}

void FileAppender::Append( const std::string message ) const
{
	std::ofstream logFile;
	logFile.open( _filePath.c_str( ), std::fstream::out | std::fstream::app );

	if ( !logFile.is_open( ) )
	{
		FileWriteException ioE( "FileAppender::Initialize - Cannot write to log file" );
		Logger::GetInstance( )->Fatal( ioE.what( ) );
		throw ioE;
	}

	logFile << message.c_str( );
	logFile.close( );
}