#include <windows.h>
#include <crtdbg.h>

#include "System/Game.h"
#include "System/Configuration.h"
#include "Graphics/OgreRenderSystem.h"
#include "Scripting/ScriptSystem.h"

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#define _CRTDBG_MAP_ALLOC_NEW
	#define new new ( _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif
#include <stdio.h>

/*! Windows main point of Entry */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	//_crtBreakAlloc = 64081;
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	IConfiguration* configuration = Configuration::Load( "game/config.cfg" );

	Game game( configuration );
	game.Initialize( );

	MSG msg;
	float deltaMilliseconds = 0.0f;
	DWORD endFrameTime = 0;
	DWORD startFrameTime = 0;

	while( !game.IsQuitting( ) )
	{
		deltaMilliseconds = ( endFrameTime - startFrameTime ) / 1000.0f;
		startFrameTime = timeGetTime( );

		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		game.Update( deltaMilliseconds );
		endFrameTime = timeGetTime( );
	}

	game.Release( );

	delete configuration;
	
	return EXIT_SUCCESS;
}

// EOF