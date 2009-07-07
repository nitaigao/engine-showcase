#include "ISystem.hpp"
#include "../Configuration/IConfiguration.hpp"

extern "C"
{
	typedef void ( __stdcall* InitializeSystemLibFunction ) ( );
	typedef ISystem* ( __stdcall* CreateSystemFunction ) ( Configuration::IConfiguration* configuration );
	typedef void ( __stdcall* DestroySystemFunction ) ( ISystem* system );
};