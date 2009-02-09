#include <windows.h>
#include <crtdbg.h>

#include "System/Game.h"

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#define _CRTDBG_MAP_ALLOC_NEW
	#define new new ( _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif
#include <stdio.h>

/*! Windows main point of Entry */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	//_crtBreakAlloc = 848;
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	Game game;

	game.Initialize( );
	game.StartLoop ( false );
	game.Release( );
	
	return EXIT_SUCCESS;
}

// EOF