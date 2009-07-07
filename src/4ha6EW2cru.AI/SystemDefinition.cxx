#include "AISystem.h"

#include "Management/Management.h"

BOOL DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
	return true;
}

extern "C" void __stdcall Initialize( Management* management )
{
	Management::Initialize( management );
}

extern "C" ISystem* __stdcall CreateSystem( )
{
	return new AI::AISystem( );
}

extern "C" void __stdcall DestroySystem( ISystem* system )
{
	delete system;
	system = 0;
}