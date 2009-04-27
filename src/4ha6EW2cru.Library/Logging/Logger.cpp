#include "Logger.h"

#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/AlreadyInitializedException.hpp"
#include "../Exceptions/NullReferenceException.hpp"

#include "../Scripting/ScriptEvent.hpp"
#include "../Management/Management.h"

namespace Logging
{
	LogLevel Logger::_logLevel = LEVEL_FATAL;

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
			//Management::GetInstance( )->GetEventManager( )->QueueEvent( new ScriptEvent( "MESSAGE_LOGGED", outputMessage.str( ) ) );
		}

	#ifdef _DEBUG
		Management::GetInstance( )->GetPlatformManager( )->OutputDebugMessage( outputMessage.str( ) );
	#endif // _DEBUG

	}

	void Logger::Info( const std::string& message )
	{
		if ( _logLevel >= LEVEL_INFO )
		{
			_logger.LogMessage( "INFO", message );
		}
	}

	void Logger::Debug( const std::string& message )
	{
		if ( _logLevel >= LEVEL_DEBUG )
		{
			_logger.LogMessage( "DEBUG", message );
		}
	}

	void Logger::Warn( const std::string& message )
	{
		if ( _logLevel >= LEVEL_WARN )
		{
			_logger.LogMessage( "WARN", message );
		}
	}

	void Logger::Fatal( const std::string& message )
	{
		if ( _logLevel >= LEVEL_FATAL )
		{
			_logger.LogMessage( "FATAL", message );
		}
	}
}