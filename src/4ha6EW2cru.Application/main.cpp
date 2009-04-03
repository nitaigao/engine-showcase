#include <windows.h>
#include <mmsystem.h>
#include <crtdbg.h>

#include "System/Game.h"

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC 
	#define _CRTDBG_MAP_ALLOC_NEW 
	#define new new ( _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif

/*! Windows main point of Entry */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	//_crtBreakAlloc = 152742;
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	Game game;
	game.Initialize( );

	float deltaMilliseconds = 0.0f;
	DWORD endFrameTime = 0;
	DWORD startFrameTime = 0;

	int frameCount = 0;

	while( !game.IsQuitting( ) )
	{
		deltaMilliseconds = ( endFrameTime - startFrameTime ) / 1000.0f;
		startFrameTime = timeGetTime( );
		game.Update( deltaMilliseconds );
		endFrameTime = timeGetTime( );

		float fps = 1.0f / deltaMilliseconds;

		if ( ++frameCount >= 50 )
		{
			std::stringstream fpsStream;
			fpsStream << "Framerate: " << fps << "\n";
			OutputDebugStringA( fpsStream.str( ).c_str( ) );
			frameCount = 0;
		}

	}

	game.Release( );
	
	return EXIT_SUCCESS;
}

// EOF