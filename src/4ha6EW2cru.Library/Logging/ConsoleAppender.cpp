#include "ConsoleAppender.h"

#include <windows.h>
#include <iostream>

void ConsoleAppender::Append( const std::string message ) const
{
	OutputDebugStringA( message.c_str( ) );
	std::cout << message.c_str( ) << "\n";
}