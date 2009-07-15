#include "NetworkSystem.h"

#include "NetworkSystemScene.h"

#include <RakNetworkFactory.h>
#include <RakSleep.h>
#include <BitStream.h>
#include <MessageIdentifiers.h>

using namespace RakNet;

#include "Logging/Logger.h"
using namespace Logging;

#include "Events/Event.h"
#include "Events/EventData.hpp"
using namespace Events;

#include "System/SystemType.hpp"
using namespace Configuration;

#include "Management/Management.h"

#include "NetworkUtils.hpp"

#include "Configuration/ConfigurationTypes.hpp"

#include "ServerNetworkProvider.h"
#include "ClientNetworkProvider.h"

namespace Network
{
	NetworkSystem::~NetworkSystem( )
	{
		delete m_networkProvider;
		m_networkProvider = 0;
	}

	void NetworkSystem::Release( )
	{
		m_networkProvider->Release( );
	}

	ISystemScene* NetworkSystem::CreateScene()
	{
		m_scene = new NetworkSystemScene( this );
		return m_scene;
	}

	void NetworkSystem::Initialize( Configuration::IConfiguration* configuration )
	{
		m_configuration = configuration;

		if ( m_attributes[ System::Attributes::Network::IsServer ].As< bool >( ) )
		{
			m_networkProvider = new ServerNetworkProvider( this );
		}
		else
		{
			m_networkProvider = new ClientNetworkProvider( this );
		}

		m_networkProvider->Initialize( configuration );
	}

	void NetworkSystem::PushMessage( const std::string& componentId, const std::string& message, AnyType::AnyTypeMap parameters )
	{
		m_networkProvider->PushMessage( componentId, message, parameters );
	}

	void NetworkSystem::Update( const float& deltaMilliseconds )
	{
		m_networkProvider->Update( deltaMilliseconds );
	}

	void NetworkSystem::Message( const std::string& message, AnyType::AnyTypeMap parameters )
	{
		if( m_networkProvider )
		{
			m_networkProvider->Message( message, parameters );
		}
	}

	void NetworkSystem::MessageComponent( const std::string& componentName, const std::string& message, AnyType::AnyTypeMap parameters )
	{
		m_scene->MessageComponent( componentName, message, parameters );
	}
}