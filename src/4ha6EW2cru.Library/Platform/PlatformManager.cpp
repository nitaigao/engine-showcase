#include "PlatformManager.h"

#include "../Events/Event.h"
#include "../Management/Management.h"

#include <windows.h>

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

#ifndef _USE_OLD_IOSTREAMS

using namespace std;

#endif

LRESULT CALLBACK WindowProcedure( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{

	case WM_DESTROY:

		PostQuitMessage( 0 );
		return 0;

	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

void PlatformManager::CreateInteractiveWindow( const std::string& title, const int& width, const int& height, const bool& fullScreen )
{
	WNDCLASSEX wnd;

	wnd.cbSize = sizeof( WNDCLASSEX );
	wnd.style = CS_HREDRAW | CS_VREDRAW;

	wnd.lpfnWndProc = WindowProcedure;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = GetModuleHandle( NULL );
	wnd.hIcon = NULL;
	wnd.hCursor = NULL;
	wnd.hbrBackground = GetSysColorBrush( COLOR_BTNFACE );
	wnd.lpszMenuName = NULL;
	wnd.lpszClassName = "WindowClassName";
	wnd.hIconSm = LoadIcon( NULL,IDI_APPLICATION );

	RegisterClassEx(&wnd);

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
		if ( msg.message == WM_QUIT )
		{
			Management::GetInstance( )->GetEventManager( )->QueueEvent( new Events::Event( Events::GAME_QUIT ) );
		}

		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

void PlatformManager::CloseWindow( )
{
	DestroyWindow( ( HWND ) _hWnd );
}