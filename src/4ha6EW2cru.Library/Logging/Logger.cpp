#include "Logger.h"

#include <fstream>
#include <ostream>
#include <sstream>

#include <vector>

#include "../Common/Paths.hpp"
#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/AlreadyInitializedException.hpp"

static Logger* g_loggerInstance = 0;

Logger* Logger::GetInstance( )
{
	if ( g_loggerInstance == 0 )
	{
		throw UnInitializedException( );
	}

	return g_loggerInstance;
}

void Logger::Release( )
{
	delete g_loggerInstance;
	g_loggerInstance = 0;
}

bool Logger::Initialize( )
{
	if ( g_loggerInstance != 0 )
	{
		std::string errorMessage = "Logger has already been Initialized";
		Logger::GetInstance( )->Fatal( errorMessage );
		throw AlreadyInitializedException( errorMessage );
	}

	g_loggerInstance = new Logger( );

	return true;	
}

Logger::~Logger( )
{
	for( AppenderList::iterator i = _appenders.begin( ); i != _appenders.end( ); ++i )
	{
		IAppender* appender = ( *i );
		delete appender;
	}

	_appenders.clear( );
}

void Logger::AddAppender( IAppender* appender )
{
	_appenders.push_back( appender );
}

void Logger::LogMessage( const std::string level, const std::string message )
{
	std::stringstream outputMessage;
	outputMessage << level << ": " << message << "\n";

	for( AppenderList::iterator i = _appenders.begin( ); i != _appenders.end( ); ++i )
	{
		( *i )->Append( outputMessage.str( ) );
	}
}

void Logger::Info( const std::string message )
{
	g_loggerInstance->LogMessage( "INFO", message );
}

void Logger::Debug( const std::string message )
{
	g_loggerInstance->LogMessage( "DEBUG", message );
}

void Logger::Fatal( const std::string message )
{
	g_loggerInstance->LogMessage( "FATAL", message );
}