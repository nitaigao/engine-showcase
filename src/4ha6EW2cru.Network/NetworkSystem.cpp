#include "NetworkSystem.h"

#include "NetworkSystemScene.h"

#include <RakNetworkFactory.h>
#include <RakSleep.h>
#include <MessageIdentifiers.h>

#include "Logging/Logger.h"
using namespace Logging;

namespace Network
{
	struct NetworkMessage
	{
		unsigned char message;
		System::Message messageId;
		AnyType::AnyTypeMap parameters;
	};

	NetworkSystem::~NetworkSystem( )
	{
		if ( m_networkInterface != 0 )
		{
			delete m_networkInterface;
		}
	}

	void NetworkSystem::Release( )
	{
		m_networkInterface->Shutdown( 0 );
	}

	ISystemScene* NetworkSystem::CreateScene()
	{
		return new NetworkSystemScene( );
	}

	void NetworkSystem::Initialize( Configuration::IConfiguration* configuration )
	{
		m_configuration = configuration;

		m_configuration->SetDefault( System::ConfigSections::Network, "port", 8989 );
		m_configuration->SetDefault( System::ConfigSections::Network, "max_server_connections", 10 );
		m_configuration->SetDefault( System::ConfigSections::Network, "max_client_connections", 1 );
		m_configuration->SetDefault( System::ConfigSections::Network, "sleeptime", 30 );

		m_networkInterface = RakNetworkFactory::GetRakPeerInterface( );

		if ( m_attributes[ System::Attributes::Network::IsServer ].As< bool >( ) )
		{
			SocketDescriptor socketDescriptor( m_configuration->Find( System::ConfigSections::Network, "port" ).As< int >( ), 0 );
			m_networkInterface->SetMaximumIncomingConnections( m_configuration->Find( System::ConfigSections::Network, "max_server_connections" ).As< int >( ) );
			m_networkInterface->Startup( m_configuration->Find( System::ConfigSections::Network, "max_server_connections" ).As< int >( ), 0, &socketDescriptor, 1 );
		}
		else
		{
			m_networkInterface->Startup( m_configuration->Find( System::ConfigSections::Network, "max_client_connections" ).As< int >( ), 0, &SocketDescriptor( ), 1 );
		}
	}

	void NetworkSystem::Update( const float& deltaMilliseconds )
	{
		RakSleep( m_configuration->Find( System::ConfigSections::Network, "sleeptime" ).As< int >( ) );

		Packet* packet = m_networkInterface->Receive( );

		std::stringstream logMessage;
		
		if ( packet )
		{
			unsigned char packetId = this->GetPacketIdentifier( packet );
			DefaultMessageIDTypes messageType = static_cast< DefaultMessageIDTypes >( packetId );

			switch( packetId )
			{

			// Server

			case ID_NEW_INCOMING_CONNECTION:
				{
					logMessage << packet->systemAddress.ToString( false ) << " connected";

					NetworkMessage message;
					message.message = ID_USER_PACKET_ENUM;
					message.messageId = "changelevel";
					message.parameters[ "levelName" ] = "mylevel";

					m_networkInterface->Send( ( char* ) &message, sizeof( message ), MEDIUM_PRIORITY, RELIABLE, 0, packet->systemAddress, false );

					break;
				}

			case ID_REMOTE_DISCONNECTION_NOTIFICATION:

				logMessage << packet->systemAddress.ToString( false ) << " has disconnected";

				break;

			case ID_REMOTE_CONNECTION_LOST:

				logMessage << packet->systemAddress.ToString( false ) << " has lost connection";

				break;

			// Client

			case ID_NO_FREE_INCOMING_CONNECTIONS:

				logMessage << packet->systemAddress.ToString( ) << " is full";

				break;

			case ID_CONNECTION_REQUEST_ACCEPTED:

				logMessage << packet->systemAddress.ToString( ) << " accepted the connection";

				break;

			case ID_CONNECTION_LOST:

				logMessage << packet->systemAddress.ToString( ) << " disconnected";

				break;

			case ID_CONNECTION_ATTEMPT_FAILED:

				logMessage << "Failed to connect to " << packet->systemAddress.ToString( );

				break;

			case ID_USER_PACKET_ENUM:
				{
					NetworkMessage* message = reinterpret_cast< NetworkMessage* >( packet->data );

					logMessage << message->messageId;

					break;
				}
			}

			if( logMessage.str( ).length( ) > 0 )
			{
				Logger::Get( )->Net( logMessage.str( ) );
			}

			m_networkInterface->DeallocatePacket( packet );
		}
	}

	void NetworkSystem::Message( const std::string& message, AnyType::AnyTypeMap parameters )
	{
		if ( message == System::Messages::Network::Connect )
		{
			m_networkInterface->Connect(
				parameters[ System::Parameters::Network::HostAddress ].As< std::string >( ).c_str( ),
				parameters[ System::Parameters::Network::Port ].As< int >( ),
				0, 0 );
		}
	}

	unsigned char NetworkSystem::GetPacketIdentifier( Packet *packet )
	{
		if ( packet == 0 )
		{
			return 255;
		}

		if ( ( unsigned char ) packet->data[ 0 ] == ID_TIMESTAMP )
		{
			assert( packet->length > sizeof( unsigned char ) + sizeof( unsigned long ) );
			return ( unsigned char ) packet->data[ sizeof( unsigned char ) + sizeof( unsigned long ) ];
		}

		return ( unsigned char ) packet->data[ 0 ];
	}
}