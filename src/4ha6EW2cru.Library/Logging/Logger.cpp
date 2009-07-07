#include "Logger.h"

#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/AlreadyInitializedException.hpp"
#include "../Exceptions/NullReferenceException.hpp"

#include "../Scripting/ScriptEvent.hpp"
using namespace Script;

#include "../Management/Management.h"

namespace Logging
{
	LogLevel Logger::s_logLevel = LEVEL_FATAL;

	void Logger::LogMessage( const std::string& level, const std::string& message )
	{
		if ( 0 == message.c_str( ) )
		{
			NullReferenceException e( "Logger::LogMessage - Attempted to Log a NULL message" );
			throw e;
		}

		std::stringstream outputMessage;
		outputMessage << level << ": " << message << "\n";

		if( level != "DEBUG" )
		{
			Management::GetEventManager( )->QueueEvent( new ScriptEvent( "MESSAGE_LOGGED", outputMessage.str( ) ) );
		}

	//#ifdef _DEBUG
		Management::GetPlatformManager( )->OutputDebugMessage( outputMessage.str( ) );
	//#endif // _DEBUG

		Management::GetPlatformManager( )->OutputToConsole( outputMessage.str( ) );

	}

	void Logger::Info( const std::string& message )
	{
		if ( s_logLevel >= LEVEL_INFO )
		{
			s_logger.LogMessage( "INFO", message );
		}
	}

	void Logger::Debug( const std::string& message )
	{
		if ( s_logLevel >= LEVEL_DEBUG )
		{
			s_logger.LogMessage( "DEBUG", message );
		}
	}

	void Logger::Warn( const std::string& message )
	{
		if ( s_logLevel >= LEVEL_WARN )
		{
			s_logger.LogMessage( "WARN", message );
		}
	}

	void Logger::Fatal( const std::string& message )
	{
		if ( s_logLevel >= LEVEL_FATAL )
		{
			s_logger.LogMessage( "FATAL", message );
		}
	}

	void Logger::Net( const std::string& message )
	{
		if ( s_logLevel >= LEVEL_INFO )
		{
			s_logger.LogMessage( "NET", message );
		}
	}
}