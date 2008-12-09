#ifndef __LOGGER_H
#define __LOGGER_H

#include <vector>

#include "IAppender.hpp"

#include "LogLevel.hpp"

/*!
	Logging mechanism that logs to Console and a File
*/
class Logger
{

	typedef std::vector< IAppender* > AppenderList;

public:

	Logger( LogLevel logLevel ) 
		: _logLevel( logLevel )
	{ 
	
	};

	~Logger( );

	/*! Retrieves an instance of the Logger Singleton */
	static Logger* GetInstance( );

	/*! Releases all resources and the Logger Singleton */
	void Release( );

	/*! Initlaized the Logging mechanism */
	static bool Initialize( );

	/*! Adds a Logging Appender to the List */
	void AddAppender( IAppender* appender );

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

private:

	Logger( const Logger & copy ) { };
	Logger & operator = ( const Logger & copy ) { return *this; };

	/*! Logs a message */
	void LogMessage( const std::string level, const std::string message );

	AppenderList _appenders;
	LogLevel _logLevel;	

};

#endif