#include "Logger.h"

#include <fstream>
#include <ostream>
#include <iostream>
#include <streambuf>
#include <sstream>
#include <vector>

#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/AlreadyInitializedException.hpp"
#include "../Exceptions/NullReferenceException.hpp"

#include "../Scripting/ScriptEvent.hpp"
#include "../Events/EventData.hpp"
#include "../System/Management.h"

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
		throw e;
	}

	g_loggerInstance = new Logger( FATAL );
	g_loggerInstance->SetLogLevel( DEBUGA );

	return true;	
}

void Logger::LogMessage( const std::string level, const std::string message )
{
	if ( 0 == message.c_str( ) )
	{
		NullReferenceException e( "Logger::LogMessage - Attempted to Log a NULL message" );
		throw e;
	}

	std::stringstream outputMessage;
	outputMessage << level << ": " << message << "\n";

	OutputDebugString( outputMessage.str( ).c_str( ) );
	Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "MESSAGE_LOGGED", outputMessage.str( ) ) );
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