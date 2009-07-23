#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum.hpp>

#define LOGGER_MAX_ARITY 10
#define LOG_PARAMS(z, n, data) << _##n << " "
#define LOG_FUNCTIONPARAMS(z, n, data) const T##n& _##n

/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   Logger.h
*  @date   2009/04/26
*/

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <sstream>

#include "../Export.hpp"
#include "LogLevel.hpp"

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
		static GAMEAPI void Initialize( );


		/*! Initializes the Logger Singleton from an already existing pointer
		*
		* @param[in] Logger * logger
		* @return ( void )
		*/
		static GAMEAPI void Initialize( Logger* logger );


		/*! Releases the Logger Singleton
		*
		* @return ( void )
		*/
		static GAMEAPI void Release( );


		/*! Returns the instance of the Logger Singleton
		*
		* @return ( Logger* )
		*/
		static GAMEAPI Logger* Get( );
		

		/*! Logs a message with the DEBUG prefix
		*
		*  @param[in] const std::string message
		*  @return (void)
		*/
#define LOG_DEBUG_FUNCTION(z, n, data)																						\
		template< class A BOOST_PP_COMMA_IF( n ) BOOST_PP_ENUM_PARAMS( n, class T ) >										\
		GAMEAPI void Debug( const A& start BOOST_PP_COMMA_IF( n ) BOOST_PP_ENUM( n, LOG_FUNCTIONPARAMS, data ) )			\
		{																													\
			if ( m_logLevel >= LEVEL_DEBUG )																				\
			{																												\
				std::stringstream message;																					\
				message << start << " " BOOST_PP_REPEAT( n, LOG_PARAMS, data );												\
																															\
				this->LogMessage( "DEBUG", message.str( ) );																\
			}																												\
		};																													\

		BOOST_PP_REPEAT( LOGGER_MAX_ARITY, LOG_DEBUG_FUNCTION, data )
		
		/*! Logs a message with the INFO prefix			
		*
		*  @param[in] const std::string message
		*  @return (void)
		*/
#define LOG_INFO_FUNCTION(z, n, data)																						\
		template< class A BOOST_PP_COMMA_IF( n ) BOOST_PP_ENUM_PARAMS( n, class T ) >										\
		GAMEAPI void Info( const A& start BOOST_PP_COMMA_IF( n ) BOOST_PP_ENUM( n, LOG_FUNCTIONPARAMS, data ) )				\
		{																													\
			if ( m_logLevel >= LEVEL_INFO )																					\
			{																												\
				std::stringstream message;																					\
				message << start << " " BOOST_PP_REPEAT( n, LOG_PARAMS, data );												\
																															\
				this->LogMessage( "INFO", message.str( ) );																	\
			}																												\
		};																													\

		BOOST_PP_REPEAT( LOGGER_MAX_ARITY, LOG_INFO_FUNCTION, data )
		

		/*! Logs a message with the WARN prefix			
		*
		*  @param[in] const std::string message
		*  @return (void)
		*/
#define LOG_WARN_FUNCTION(z, n, data)																						\
		template< class A BOOST_PP_COMMA_IF( n ) BOOST_PP_ENUM_PARAMS( n, class T ) >										\
		GAMEAPI void Warn( const A& start BOOST_PP_COMMA_IF( n ) BOOST_PP_ENUM( n, LOG_FUNCTIONPARAMS, data ) )				\
		{																													\
			if ( m_logLevel >= LEVEL_WARN )																					\
			{																												\
				std::stringstream message;																					\
				message << start << " " BOOST_PP_REPEAT( n, LOG_PARAMS, data );												\
																															\
				this->LogMessage( "WARN", message.str( ) );																	\
			}																												\
		};																													\

		BOOST_PP_REPEAT( LOGGER_MAX_ARITY, LOG_WARN_FUNCTION, data )


		/*! Logs a message with the FATAL prefix			
		*
		*  @param[in] const std::string message
		*  @return (void)
		*/
#define LOG_FATAL_FUNCTION(z, n, data)																						\
		template< class A BOOST_PP_COMMA_IF( n ) BOOST_PP_ENUM_PARAMS( n, class T ) >										\
		GAMEAPI void Fatal( const A& start BOOST_PP_COMMA_IF( n ) BOOST_PP_ENUM( n, LOG_FUNCTIONPARAMS, data ) )			\
		{																													\
			if ( m_logLevel >= LEVEL_FATAL )																				\
			{																												\
				std::stringstream message;																					\
				message << start << " " BOOST_PP_REPEAT( n, LOG_PARAMS, data );												\
																															\
				this->LogMessage( "FATAL", message.str( ) );																\
			}																												\
		};																													\

		BOOST_PP_REPEAT( LOGGER_MAX_ARITY, LOG_FATAL_FUNCTION, data )


		/*! Logs a message with the NET prefix			
		*
		*  @param[in] const std::string message
		*  @return (void)
		*/
#define LOG_NET_FUNCTION(z, n, data)																						\
		template< class A BOOST_PP_COMMA_IF( n ) BOOST_PP_ENUM_PARAMS( n, class T ) >										\
		GAMEAPI void Net( const A& start BOOST_PP_COMMA_IF( n ) BOOST_PP_ENUM( n, LOG_FUNCTIONPARAMS, data ) )				\
		{																													\
			if ( m_logLevel >= LEVEL_NET )																					\
			{																												\
				std::stringstream message;																					\
				message << start << " " BOOST_PP_REPEAT( n, LOG_PARAMS, data );												\
																															\
				this->LogMessage( "NET", message.str( ) );																	\
			}																												\
		};																													\

		BOOST_PP_REPEAT( LOGGER_MAX_ARITY, LOG_NET_FUNCTION, data )


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

		GAMEAPI void LogMessage( const std::string& level, const std::string& message );

		LogLevel m_logLevel;

		~Logger( ) { };

		Logger( )
			: m_logLevel( LEVEL_FATAL )
		{

		}

		Logger( const Logger & copy ) { };
		Logger & operator = ( const Logger & copy ) { return *this; };

	};

#define Debug( ... ) Logging::Logger::Get( )->Debug( __FUNCTION__, "-", ##__VA_ARGS__ )
#define Info( ... ) Logging::Logger::Get( )->Info( ##__VA_ARGS__ )
#define Warn( ... ) Logging::Logger::Get( )->Warn( ##__VA_ARGS__ )
#define Fatal( ... ) Logging::Logger::Get( )->Fatal( ##__VA_ARGS__ )
#define Net( ... ) Logging::Logger::Get( )->Net( ##__VA_ARGS__ )

};

#endif