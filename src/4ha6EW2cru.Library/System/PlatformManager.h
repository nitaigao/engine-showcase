#ifndef __PLATFORMMANAGER_H
#define __PLATFORMMANAGER_H

#include "IPlatformManager.h"

class PlatformManager : public IPlatformManager
{

public:

	virtual ~PlatformManager( ) { };

	PlatformManager( )
		: _hWnd( 0 )
	{

	}

	void CreateInteractiveWindow( const std::string& title, const int& width, const int& height, const bool& fullScreen );
	void CreateConsoleWindow( );
	void Update( float deltaMilliseconds );
	
	inline size_t GetHwnd( )
	{ 
		if ( _hWnd == 0 )
		{
			return ( size_t ) GetConsoleWindow( );
		}

		return _hWnd; 
	};

private:

	size_t _hWnd;

	PlatformManager( const PlatformManager & copy ) { };
	PlatformManager & operator = ( const PlatformManager & copy ) { return *this; };

};

#endif