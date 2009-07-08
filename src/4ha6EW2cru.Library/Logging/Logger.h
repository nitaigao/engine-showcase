/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   Logger.h
*  @date   2009/04/26
*/
#pragma once
#ifndef LOGGER_H
#define LOGGER_H

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

		/*! Initializes the Logger Singleton
		*
		* @return ( void )
		*/
		static void Initialize( );


		/*! Initializes the Logger Singleton from an already existing pointer
		*
		* @param[in] Logger * logger
		* @return ( void )
		*/
		static void Initialize( Logger* logger );


		/*! Releases the Logger Singleton
		*
		* @return ( void )
		*/
		static void Release( );


		/*! Returns the instance of the Logger Singleton
		*
		* @return ( Logger* )
		*/
		static Logger* Get( );

		
		/*! Logs a message with the DEBUG prefix
		 *
		 *  @param[in] const std::string message
		 *  @return (void)
		 */
		void Debug( const std::string& message );

		
		/*! Logs a message with the INFO prefix
		 *
		 *  @param[in] const std::string message
		 *  @return (void)
		 */
		void Info( const std::string& message );

		
		/*! Logs a message with the WARN prefix
		 *
		 *  @param[in] const std::string message
		 *  @return (void)
		 */
		void Warn( const std::string& message );

		
		/*! Logs a message with the FATAL prefix
		 *
		 *  @param[in] const std::string message
		 *  @return (void)
		 */
		void Fatal( const std::string& message );


		/*! Logs a message with the NET prefix
		*
		* @param[in] const std::string & message
		* @return ( void )
		*/
		void Net( const std::string& message );

		
		/*! Sets the current logging level
		 *
		 *  @param[in] LogLevel logLevel
		 *  @return (void)
		 */
		void SetLogLevel( const LogLevel& logLevel ) { m_logLevel = logLevel; };

		
		/*! Gets the current logging level
		 *
		 *  @return (LogLevel)
		 */
		LogLevel GetLogLevel( ) { return m_logLevel; };

	private:

		void LogMessage( const std::string& level, const std::string& message );

		LogLevel m_logLevel;

		~Logger( ) { };

		Logger( )
			: m_logLevel( LEVEL_FATAL )
		{

		}

		Logger( const Logger & copy ) { };
		Logger & operator = ( const Logger & copy ) { return *this; };

	};
};

#endif
