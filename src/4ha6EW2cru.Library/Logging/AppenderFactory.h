#ifndef __APPENDERFACTORY_H
#define __APPENDERFACTORY_H

#include "IAppender.hpp"
#include "AppenderType.hpp"

/*!
	Factory class for creating IAppender instances
*/
class AppenderFactory
{

public:

	/*! Creates an instance of the given AppenderType */
	static IAppender* CreateAppender( const AppenderType appenderType );

private:

	AppenderFactory( ) { };
	~AppenderFactory( ) { };
	AppenderFactory( const AppenderFactory & copy ) { };
	AppenderFactory & operator = ( const AppenderFactory & copy ) { return *this; };

};

#endif