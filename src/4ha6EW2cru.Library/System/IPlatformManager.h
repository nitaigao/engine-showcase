#ifndef __IPLATFORMMANAGER_H
#define __IPLATFORMMANAGER_H

#include <string>
#include <windows.h>

class IPlatformManager
{

public:

	virtual ~IPlatformManager( ) { };

	virtual void CreateInteractiveWindow( const std::string& title, const int& width, const int& height, const bool& fullScreen ) = 0;
	virtual void CloseWindow( ) = 0;
	virtual void CreateConsoleWindow( ) = 0;
	virtual void Update( float deltaMilliseconds ) = 0;
	virtual size_t GetHwnd( ) = 0;
};

#endif