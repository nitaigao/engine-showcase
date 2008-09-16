#include "AppenderFactory.h"

#include "ConsoleAppender.h"
#include "FileAppender.h"

IAppender* AppenderFactory::CreateAppender( const AppenderType appenderType )
{
	IAppender* appender = 0;

	switch( appenderType )
	{

	case CONSOLEAPPENDER: 

		appender = new ConsoleAppender( );

		break;

	case FILEAPPENDER:

		appender = new FileAppender( );

		break;

	}

	return appender;
}