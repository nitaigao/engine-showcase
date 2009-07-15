#include "ServerNetworkProvider.h"

#include "NetworkUtils.hpp"

#include "Configuration/Configuration.h"
#include "Configuration/ConfigurationTypes.hpp"
using namespace Configuration;

#include <RakNetworkFactory.h>
#include <RakSleep.h>
#include <BitStream.h>
#include <MessageIdentifiers.h>

using namespace RakNet;

#include <sstream>

#include "Logging/Logger.h"
using namespace Logging;

#include "Management/Management.h"

namespace Network
{
	ServerNetworkProvider::~ServerNetworkProvider()
	{
		if ( m_networkInterface != 0 )
		{
			delete m_networkInterface;
		}
	}

	void ServerNetworkProvider::Release()
	{
		m_networkInterface->Shutdown( m_configuration->Find( ConfigSections::Network, ConfigItems::Network::MaxServerReleaseTime ).As< int >( ) );
	}

	void ServerNetworkProvider::Initialize( Configuration::IConfiguration* configuration )
	{
		m_configuration = configuration;

		configuration->SetDefault( ConfigSections::Network, ConfigItems::Network::ServerPort, 8989 );
		configuration->SetDefault( ConfigSections::Network, ConfigItems::Network::MaxServerConnections, 10 );
		configuration->SetDefault( ConfigSections::Network, ConfigItems::Network::ServerSleepTime, 30 );
		m_configuration->SetDefault( ConfigSections::Network, ConfigItems::Network::MaxServerReleaseTime, 10 );

		m_networkInterface = RakNetworkFactory::GetRakPeerInterface( );

		SocketDescriptor socketDescriptor( configuration->Find( ConfigSections::Network, ConfigItems::Network::ServerPort ).As< int >( ), 0 );

		m_networkInterface->SetMaximumIncomingConnections( 
			configuration->Find( ConfigSections::Network, ConfigItems::Network::MaxServerConnections ).As< int >( ) 
			);

		m_networkInterface->Startup( 
			configuration->Find( ConfigSections::Network, ConfigItems::Network::MaxServerConnections ).As< int >( ), 
			configuration->Find( ConfigSections::Network, ConfigItems::Network::ServerSleepTime ).As< int >( ), 
			&socketDescriptor, 1 );
	}

	void ServerNetworkProvider::Update( const float& deltaMilliseconds )
	{
		RakSleep( m_configuration->Find( ConfigSections::Network, ConfigItems::Network::ServerSleepTime ).As< int >( ) );

		Packet* packet = m_networkInterface->Receive( );

		std::stringstream logMessage;

		if ( packet )
		{
			unsigned char packetId = NetworkUtils::GetPacketIdentifier( packet );
			DefaultMessageIDTypes messageType = static_cast< DefaultMessageIDTypes >( packetId );

			switch( packetId )
			{

			case ID_NEW_INCOMING_CONNECTION:

				this->OnClientConnected( packet );

				break;

			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			case ID_DISCONNECTION_NOTIFICATION:

				logMessage << packet->systemAddress.ToString( false ) << " has disconnected";

				break;

			case ID_REMOTE_CONNECTION_LOST:

				logMessage << packet->systemAddress.ToString( false ) << " has lost connection";

				break;

			case ID_USER_PACKET_ENUM:

				this->OnUserPacketReceived( packet );
			}

			if( logMessage.str( ).length( ) > 0 )
			{
				Logger::Get( )->Debug( logMessage.str( ) );
			}

			m_networkInterface->DeallocatePacket( packet );
		}
	}

	void ServerNetworkProvider::Message( const std::string& message, AnyType::AnyTypeMap parameters )
	{

	}

	void ServerNetworkProvider::OnClientConnected( Packet* packet )
	{
		std::stringstream logMessage;
		logMessage << "Server - " << packet->systemAddress.ToString( false ) << " connected";
		Logger::Get( )->Debug( logMessage.str( ) );

		m_clients.push_back( packet->systemAddress );

		AnyType::AnyTypeMap parameters;
		parameters[ System::Parameters::Game::LevelName ] = Management::Get( )->GetInstrumentation( )->GetLevelName( );

		this->SendMessageTo( packet->systemAddress, System::Messages::Game::ChangeLevel, parameters );
	}

	void ServerNetworkProvider::OnUserPacketReceived( Packet* packet )
	{
		BitStream bitStream( packet->data, packet->length, false );
		NetworkMessage* message = NetworkUtils::DeSerialize( &bitStream );

		std::stringstream logMessage;
		std::string messageText = message->parameters[ System::Parameters::Network::ComponentMessage ].As< System::Message >( );
		messageText = ( messageText.empty( ) ) ? message->messageId.C_String( ) : messageText;
		logMessage << "ServerNetworkProvider::OnUserPacketReceived - " << messageText << " from " << packet->systemAddress.ToString( false );
		Logger::Get( )->Debug( logMessage.str( ) );

		if ( message->messageId == System::Messages::Network::ComponentUpdate.c_str( ) )
		{
			m_networkSystem->MessageComponent( 
				message->parameters[ System::Parameters::Network::ComponentId ].As< std::string >( ), 
				message->parameters[ System::Parameters::Network::ComponentMessage ].As< System::Message >( ),
				message->parameters
				);
		}
	}

	void ServerNetworkProvider::SendMessageTo( const SystemAddress& networkAddress, const std::string& message, AnyType::AnyTypeMap parameters )
	{
		NetworkMessage messageToSend;
		messageToSend.message = ID_USER_PACKET_ENUM;
		messageToSend.messageId = message.c_str( );
		messageToSend.parameters = parameters;

		messageToSend.parameters[ System::Parameters::Network::ComponentMessage ] = std::string( "" );

		BitStream* bitStream = NetworkUtils::Serialize( messageToSend );

		m_networkInterface->Send( bitStream, MEDIUM_PRIORITY, RELIABLE, 0, networkAddress, false );

		delete bitStream;

		std::stringstream logMessage;
		std::string messageText = messageToSend.parameters[ System::Parameters::Network::ComponentMessage ].As< System::Message >( );
		messageText = ( messageText.empty( ) ) ? messageToSend.messageId.C_String( ) : messageText;
		logMessage << "ServerNetworkProvider::SendMessageTo - " << messageText << " to " << networkAddress.ToString( false );
		Logger::Get( )->Debug( logMessage.str( ) );
	}

	void ServerNetworkProvider::PushMessage( const std::string& componentId, const std::string& message, AnyType::AnyTypeMap parameters )
	{
		if ( message == System::Messages::SetPosition )
		{
			parameters[ System::Parameters::Network::ComponentMessage ] = message;
			parameters[ System::Parameters::Network::ComponentId ] = componentId;

			NetworkMessage messageToSend;
			messageToSend.message = ID_USER_PACKET_ENUM;
			messageToSend.messageId = System::Messages::Network::ComponentUpdate.c_str( );
			messageToSend.parameters = parameters;

			BitStream* bitStream = NetworkUtils::Serialize( messageToSend );

			for ( SystemAddressList::iterator i = m_clients.begin( ); i != m_clients.end( ); ++i )
			{
				m_networkInterface->Send( bitStream, MEDIUM_PRIORITY, RELIABLE, 0, ( *i ), false );

				std::stringstream logMessage;
				logMessage << "ServerNetworkProvider::PushMessage - " << message << " to " << ( *i ).ToString( false );
				Logger::Get( )->Debug( logMessage.str( ) );

				delete bitStream;
			}
		}
	}
}