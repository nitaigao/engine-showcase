#include "NetworkSystemComponent.h"

#include "NetworkSystemScene.h"

#include "Maths/MathVector3.hpp"
#include "Maths/MathQuaternion.hpp"
using namespace Maths;

namespace Network
{
	AnyType NetworkSystemComponent::Message( const System::Message& message, AnyType::AnyTypeMap parameters )
	{
		INetworkSystemScene* networkScene = m_attributes[ System::Attributes::Parent ].As< NetworkSystemScene* >( );
		networkScene->GetSystem( )->PushMessage( m_attributes[ System::Attributes::Name ].As< std::string >( ), message, parameters );
		
		return AnyType( );
	}

	void NetworkSystemComponent::MessageFromNetwork( const std::string& message, AnyType::AnyTypeMap parameters )
	{
		this->PushMessage( message, parameters );
	}
}