#include "PlatformManager.h"

#include <windows.h>

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

#ifndef _USE_OLD_IOSTREAMS

using namespace std;

#endif

LRESULT CALLBACK WindowProcedure(HWND han_Wind,UINT uint_Message,WPARAM parameter1,LPARAM parameter2)
{
	return DefWindowProc(han_Wind,uint_Message,parameter1,parameter2);
}

void PlatformManager::CreateInteractiveWindow( const std::string& title, const int& width, const int& height, const bool& fullScreen )
{
	WNDCLASSEX wnd_Structure;

	wnd_Structure.cbSize = sizeof( WNDCLASSEX );
	wnd_Structure.style = CS_HREDRAW | CS_VREDRAW;

	wnd_Structure.lpfnWndProc = WindowProcedure;
	wnd_Structure.cbClsExtra = 0;
	wnd_Structure.cbWndExtra = 0;
	wnd_Structure.hInstance = GetModuleHandle( NULL );
	wnd_Structure.hIcon = NULL;
	wnd_Structure.hCursor = NULL;
	wnd_Structure.hbrBackground = GetSysColorBrush( COLOR_BTNFACE );
	wnd_Structure.lpszMenuName = NULL;
	wnd_Structure.lpszClassName = "WindowClassName";
	wnd_Structure.hIconSm = LoadIcon( NULL,IDI_APPLICATION );

	RegisterClassEx(&wnd_Structure);

	int screenWidth = GetSystemMetrics( SM_CXSCREEN ) ;
	int screenHeight = GetSystemMetrics( SM_CYSCREEN ) ;

	_hWnd = ( size_t ) CreateWindowExA( WS_EX_CONTROLPARENT, "WindowClassName", title.c_str( ),
		( fullScreen ) ? WS_POPUP | WS_EX_TOPMOST : WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
		( screenWidth - width ) / 2,  ( screenHeight - height ) / 2, width, height, NULL, NULL, GetModuleHandle( NULL ), NULL );
}

static const WORD MAX_CONSOLE_LINES = 500;

void PlatformManager::CreateConsoleWindow( )
{
	int hConHandle;

	long lStdHandle;

	CONSOLE_SCREEN_BUFFER_INFO coninfo;

	AllocConsole();

	// set the screen buffer to be big enough to let us scroll text

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),

		&coninfo);

	coninfo.dwSize.Y = MAX_CONSOLE_LINES;

	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),

		coninfo.dwSize);

	// redirect unbuffered STDOUT to the console

	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);

	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
}

void PlatformManager::Update( float deltaMilliseconds )
{
	MSG msg;

	if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}