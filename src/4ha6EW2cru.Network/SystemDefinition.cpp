
#include "Configuration/IConfiguration.hpp"
using namespace Configuration;

#include "NetworkSystem.h"

extern "C" void __stdcall
Initialize( )
{

}

extern "C" ISystem* __stdcall
CreateSystem( IConfiguration* configuration )
{
	return new Network::NetworkSystem( configuration );
}

extern "C" void __stdcall
DestroySystem( ISystem* system )
{
	delete system;
}