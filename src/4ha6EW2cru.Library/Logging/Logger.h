#ifndef __LOGGER_H
#define __LOGGER_H

#include <vector>
#include <sstream>

#include "IAppender.hpp"

#include "LogLevel.hpp"

/*!
	Logging mechanism that logs to Console and a File
*/
class Logger
{

public:

	~Logger( ) { };

	Logger( )
		: _logLevel( FATAL )
	{

	}

	/*! Retrieves an instance of the Logger Singleton */
	static Logger* GetInstance( );

	/*! Releases all resources and the Logger Singleton */
	void Release( );

	/*! Initialized the Logging mechanism */
	static bool Initialize( );

	/*! Adds a Logging Appender to the List */
	void AddAppender( IAppender* appender ) { };

	/*! Logs a message with the DEBUG prefix */
	void Debug( const std::string message );

	/*! Logs a message with the INFO prefix */
	void Info( const std::string message );

	/*! Logs a message with the WARN prefix */
	void Warn( const std::string message );

	/*! Logs a message with the FATAL prefix */
	void Fatal( const std::string message );

	/*! Sets the current logging level */
	inline void SetLogLevel( LogLevel logLevel ) { _logLevel = logLevel; };

	/*! Gets the current logging level */
	inline LogLevel GetLogLevel( ) { return _logLevel; };

private:

	/*! Logs a message */
	void LogMessage( const std::string level, const std::string message );

	LogLevel _logLevel;

	Logger( const Logger & copy ) { };
	Logger & operator = ( const Logger & copy ) { return *this; };

};

#endif
