#ifndef __PLATFORMMANAGER_H
#define __PLATFORMMANAGER_H

#include "IPlatformManager.h"

class PlatformManager : public IPlatformManager
{

public:

	PlatformManager( ) { };
	~PlatformManager( ) { };

	void CreateInteractiveWindow( const std::string& title, const int& width, const int& height, const bool& fullScreen );
	void CreateConsoleWindow( );
	void Update( float deltaMilliseconds );
	
	inline size_t GetHwnd( ) { return _hWnd; };

private:

	size_t _hWnd;

};

#endif