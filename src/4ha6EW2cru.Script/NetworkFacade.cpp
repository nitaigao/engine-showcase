#include "NetworkFacade.h"

using namespace luabind;

#include "Management/Management.h"

namespace Script
{
	void NetworkFacade::RegisterFunctions( lua_State* state )
	{
		module( state )
		[
			class_< NetworkFacade >( "NetworkFacade" )
				.def( "connect", &NetworkFacade::Connect )
		];
	}

	void NetworkFacade::Connect( const std::string& hostAddress, const int& port )
	{
		AnyType::AnyTypeMap parameters;
		parameters[ System::Parameters::Network::HostAddress ] = hostAddress;
		parameters[ System::Parameters::Network::Port ] = port;

		Management::Get( )->GetSystemManager( )->GetSystem( System::Types::NETWORK )->Message( System::Messages::Network::Connect, parameters );
	}
}