#include "Win32PlatformManager.h"

#include "../Events/Event.h"
#include "../Management/Management.h"

#include <windows.h>

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

#include <shellapi.h>

#include <boost/program_options.hpp>
using namespace boost::program_options;

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

namespace Platform
{
	void Win32PlatformManager::CreateInteractiveWindow( const std::string& title, const int& width, const int& height, const bool& fullScreen )
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

		m_hWnd = ( size_t ) CreateWindowExA( WS_EX_CONTROLPARENT, "WindowClassName", title.c_str( ),
			( fullScreen ) ? WS_POPUP | WS_EX_TOPMOST : WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
			( screenWidth - width ) / 2,  ( screenHeight - height ) / 2, width, height, NULL, NULL, GetModuleHandle( NULL ), NULL );
	}

	static const WORD MAX_CONSOLE_LINES = 500;

	void Win32PlatformManager::CreateConsoleWindow( )
	{
		AllocConsole( );

		long lStdHandle = ( long )GetStdHandle( STD_OUTPUT_HANDLE );
		int hConHandle = _open_osfhandle( lStdHandle, _O_TEXT );
	}

	void Win32PlatformManager::OutputToConsole( const std::string& message )
	{
		std::streambuf* existingBuffer = std::cout.rdbuf( );
		std::ofstream newBuffer( "CONOUT$" );

		std::cout.rdbuf( newBuffer.rdbuf( ) );
		std::cout.sync_with_stdio( );

		std::cout << message.c_str( );

		std::cout.rdbuf( existingBuffer );
	}

	void Win32PlatformManager::Update( const float& deltaMilliseconds )
	{
		MSG msg;

		while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if ( msg.message == WM_QUIT )
			{
				Management::Get( )->GetEventManager( )->QueueEvent( new Events::Event( Events::GAME_QUIT ) );
			}

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	void Win32PlatformManager::CloseWindow( )
	{
		DestroyWindow( ( HWND ) m_hWnd );
	}

	size_t Win32PlatformManager::GetWindowId( ) const
	{
		if ( m_hWnd == 0 )
		{
			return ( size_t ) GetConsoleWindow( );
		}

		return m_hWnd; 
	}

	void Win32PlatformManager::OutputDebugMessage( const std::string& message )
	{
		OutputDebugString( message.c_str( ) );
	}

	AnyType::AnyTypeMap Win32PlatformManager::GetProgramOptions( ) const
	{
		/*int argc = 0;
		LPWSTR* argv = CommandLineToArgvW( GetCommandLineW( ), &argc );

		options_description optionsDescription( "Allowed options" );
		optionsDescription.add_options( ) (
			System::Options::LevelName.c_str( ), 
			boost::program_options::value< std::string >( ), 
			"start the level immediately" 
			);

		optionsDescription.add_options( ) (
			System::Options::DedicatedServer.c_str( ),
			"runs the client as a dedicated server"
			);

		positional_options_description  positionalDescription;
		positionalDescription.add( System::Options::LevelName.c_str( ), 1 );
		positionalDescription.add( System::Options::DedicatedServer.c_str( ), 1 );

		variables_map variablesMap;

		store( 
			wcommand_line_parser( argc, argv )
			.options( optionsDescription )
			.positional( positionalDescription )
			.run( ), variablesMap 
			);*/
			

		AnyType::AnyTypeMap programOptions;

		/*for ( variables_map::iterator i = variablesMap.begin( ); i != variablesMap.end( ); ++i )
		{
			programOptions[ ( *i ).first ] = ( *i ).second.as< std::string >( );
		}*/

		return programOptions;
	}
}