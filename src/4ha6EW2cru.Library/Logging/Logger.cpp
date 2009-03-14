#include "Logger.h"

#include <fstream>
#include <ostream>
#include <sstream>

#include <vector>

#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/AlreadyInitializedException.hpp"
#include "../Exceptions/NullReferenceException.hpp"

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
		AlreadyInitializedException e( "Logger::Initialize - Logger has already been Initialized" );
		Logger::GetInstance( )->Fatal( e.what ( ) );
		throw e;
	}

	g_loggerInstance = new Logger( FATAL );
	g_loggerInstance->SetLogLevel( DEBUGA );

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
	if ( 0 == appender )
	{
		NullReferenceException nullAppender( "Logger::AddAppender - Attempted to add a NULL Appender" );
		Logger::GetInstance( )->Fatal( nullAppender.what( ) );
		throw nullAppender;
	}

	for( AppenderList::iterator i = _appenders.begin( ); i != _appenders.end( ); ++i )
	{
		if ( ( *i ) == appender )
		{
			AlreadyInitializedException appenderExists( "Logger::AddAppender - Attempted to Add an already existing Appender" );
			Logger::GetInstance( )->Fatal( appenderExists.what( ) );
			throw appenderExists;
		}
	}

	_appenders.push_back( appender );
}

void Logger::LogMessage( const std::string level, const std::string message )
{
	if ( 0 == message.c_str( ) )
	{
		NullReferenceException e( "Logger::LogMessage - Attempted to Log a NULL message" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	std::stringstream outputMessage;
	outputMessage << level << ": " << message << "\n";

	for( AppenderList::iterator i = _appenders.begin( ); i != _appenders.end( ); ++i )
	{
		( *i )->Append( outputMessage.str( ) );
	}
}

void Logger::Info( const std::string message )
{
	if ( _logLevel >= INFO )
	{
		g_loggerInstance->LogMessage( "INFO", message );
	}
}

void Logger::Debug( const std::string message )
{
	if ( _logLevel >= DEBUGA )
	{
		g_loggerInstance->LogMessage( "DEBUG", message );
	}
}

void Logger::Warn( const std::string message )
{
	if ( _logLevel >= WARNA )
	{
		g_loggerInstance->LogMessage( "WARN", message );
	}
}

void Logger::Fatal( const std::string message )
{
	if ( _logLevel >= FATAL )
	{
		g_loggerInstance->LogMessage( "FATAL", message );
	}
}