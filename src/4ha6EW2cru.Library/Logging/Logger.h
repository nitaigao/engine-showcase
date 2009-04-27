/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   Logger.h
*  @date   2009/04/26
*/
#ifndef __LOGGER_H
#define __LOGGER_H

#include "LogLevel.hpp"
#include <string>
#include <sstream>

namespace Logging
{
	/*! 
	 *  Logging mechanism that logs to Console and a File
	 */
	class Logger
	{

	public:
		
		/*! Logs a message with the DEBUG prefix
		 *
		 *  @param[in] const std::string message
		 *  @return (void)
		 */
		static void Debug( const std::string& message );

		
		/*! Logs a message with the INFO prefix
		 *
		 *  @param[in] const std::string message
		 *  @return (void)
		 */
		static void Info( const std::string& message );

		
		/*! Logs a message with the WARN prefix
		 *
		 *  @param[in] const std::string message
		 *  @return (void)
		 */
		static void Warn( const std::string& message );

		
		/*! Logs a message with the FATAL prefix
		 *
		 *  @param[in] const std::string message
		 *  @return (void)
		 */
		static void Fatal( const std::string& message );

		
		/*! Sets the current logging level
		 *
		 *  @param[in] LogLevel logLevel
		 *  @return (void)
		 */
		static void SetLogLevel( const LogLevel& logLevel ) { _logLevel = logLevel; };

		
		/*! Gets the current logging level
		 *
		 *  @return (LogLevel)
		 */
		static LogLevel GetLogLevel( ) { return _logLevel; };

	private:

		static void LogMessage( const std::string& level, const std::string& message );

		static LogLevel _logLevel;
		static Logger _logger;

		~Logger( ) { };
		Logger( ) { };
		Logger( const Logger & copy ) { };
		Logger & operator = ( const Logger & copy ) { return *this; };

	};
};

#endif
