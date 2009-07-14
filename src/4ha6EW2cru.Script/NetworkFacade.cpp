#include "NetworkFacade.h"

using namespace luabind;

#include "Management/Management.h"

namespace Script
{
	scope NetworkFacade::RegisterFunctions( )
	{
		return
			class_< NetworkFacade >( "NetworkFacade" )
				.def( constructor< >( ) )
				.def( "connect", &NetworkFacade::Connect );
	}

	void NetworkFacade::Connect( const std::string& hostAddress, const int& port )
	{
		AnyType::AnyTypeMap parameters;
		parameters[ System::Parameters::Network::HostAddress ] = hostAddress;
		parameters[ System::Parameters::Network::Port ] = port;

		Management::Get( )->GetSystemManager( )->GetSystem( System::Types::NETWORK )->Message( System::Messages::Network::Connect, parameters );
	}
}