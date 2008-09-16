#ifndef CONSOLEAPPENDER_H
#define CONSOLEAPPENDER_H

#include "IAppender.hpp"

/*! 
	Appends a message to the Console 
*/
class ConsoleAppender : public IAppender
{

public:

	ConsoleAppender( ) { };
	~ConsoleAppender( ) { };

	/* Appends a message to the Appender */
	bool Append( const std::string message ) const;

private:

	ConsoleAppender( const ConsoleAppender & copy ) { };
	ConsoleAppender & operator = ( const ConsoleAppender & copy ) { return *this; };

};

#endif