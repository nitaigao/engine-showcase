/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file  LogLevel.hpp
*  @date   2009/04/26
*/
#ifndef LOGLEVEL_H
#define LOGLEVEL_H

namespace Logging
{
	/*! 
	 *  Logging Level
	 */
	enum LogLevel
	{
		LEVEL_OFF = 0,
		LEVEL_FATAL = 1,
		LEVEL_INFO = 2,
		LEVEL_WARN = 3,
		LEVEL_DEBUG = 4,
		LEVEL_ALL = 99
	};
};

#endif
