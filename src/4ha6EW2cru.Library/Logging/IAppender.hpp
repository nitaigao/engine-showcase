#ifndef __IAPPENDER_H
#define __IAPPENDER_H

#include <string>

/*! 
	Interface for all Logging Appenders 
*/
class IAppender
{

public:

	virtual ~IAppender( ) { };

	/* Appends a message to the Appender */
	virtual void Append( const std::string message ) const = 0;

};

#endif