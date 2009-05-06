/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Application\main.cpp
*  @date   2009/04/25
*/
#include <windows.h>
#include <mmsystem.h>
#include <crtdbg.h>
#include <string>

#include "Game.h"

#ifdef _DEBUG
#	define _CRTDBG_MAP_ALLOC 
#	define _CRTDBG_MAP_ALLOC_NEW 
#	define new new ( _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif

/*! Windows main point of Entry
 *
 *  @param[in] HINSTANCE hInstance
 *  @param[in] HINSTANCE hPrevInstance
 *  @param[in] LPSTR lpCmdLine
 *  @param[in] int nCmdShow
 *  @return (int WINAPI)
 */
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{

#ifdef _DEBUG
	//_crtBreakAlloc = 11079;
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 
#endif

	Game game;
	game.Initialize( );

	float deltaMilliseconds = 0.0f;
	DWORD endFrameTime = 0;
	DWORD startFrameTime = 0;

	while( !game.IsQuitting( ) )
	{
		deltaMilliseconds = ( endFrameTime - startFrameTime ) / 1000.0f;
		startFrameTime = timeGetTime( );
		game.Update( deltaMilliseconds );
		endFrameTime = timeGetTime( );
	}

	game.Release( );
	
	return 0;
}

// EOF