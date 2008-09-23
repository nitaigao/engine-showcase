#include "AppenderFactory.h"

#include "ConsoleAppender.h"
#include "FileAppender.h"
#include "EventAppender.h"

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

	case EVENTAPPENDER:

		appender = new EventAppender( );

		break;

	}

	return appender;
}