#include "ServerNetworkProvider.h"

#include "NetworkUtils.h"

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
				Debug( logMessage.str( ) );
			}

			m_networkInterface->DeallocatePacket( packet );
		}
	}

	void ServerNetworkProvider::Message( const std::string& message, AnyType::AnyTypeMap parameters )
	{

	}

	void ServerNetworkProvider::OnClientConnected( Packet* packet )
	{
		Debug( packet->systemAddress.ToString( ), "connected" );

		m_clients.push_back( packet->systemAddress );

		AnyType::AnyTypeMap parameters;
		parameters[ System::Parameters::Game::LevelName ] = Management::Get( )->GetInstrumentation( )->GetLevelName( );

		NetworkMessage messageToSend;
		messageToSend.message = ID_USER_PACKET_ENUM;
		messageToSend.messageId = System::Messages::Game::ChangeLevel.c_str( );
		messageToSend.parameters = parameters;

		NetworkUtils::SendNetworkMessage( messageToSend, packet->systemAddress, m_networkInterface );
	}

	void ServerNetworkProvider::OnUserPacketReceived( Packet* packet )
	{
		BitStream bitStream( packet->data, packet->length, false );
		NetworkMessage* message = NetworkUtils::DeSerialize( &bitStream );

		Debug( message->messageId.C_String( ), "from", packet->systemAddress.ToString( false ) );

		if ( message->messageId == System::Messages::Network::ComponentUpdate.c_str( ) )
		{
			m_networkSystem->MessageComponent( 
				message->parameters[ System::Parameters::Network::ComponentId ].As< std::string >( ), 
				message->parameters[ System::Parameters::Network::ComponentMessage ].As< System::Message >( ),
				message->parameters
				);

			std::string messageText = message->parameters[ System::Parameters::Network::ComponentMessage ].As< System::Message >( );
			messageText = ( messageText.empty( ) ) ? message->messageId.C_String( ) : messageText;
			Debug( messageText, "from", packet->systemAddress.ToString( false ) );
		}

		if ( message->messageId == System::Messages::Network::Client::CharacterSelected.c_str( ) )
		{
			//IEvent* event = new ScriptEvent( "CHARACTER_SELECTED", message->parameters[ System::Parameters::Network::Client::CharacterName ].As< System::Message >( ) );
			//Management::Get( )->GetEventManager( )->QueueEvent( event );
		}
	}


	void ServerNetworkProvider::PushMessage( const std::string& componentId, const std::string& message, AnyType::AnyTypeMap parameters )
	{
		if ( 
			message == System::Messages::SetPosition ||
			message == System::Messages::SetOrientation
			)
		{
			parameters[ System::Parameters::Network::ComponentMessage ] = message;
			parameters[ System::Parameters::Network::ComponentId ] = componentId;

			NetworkMessage messageToSend;
			messageToSend.message = ID_USER_PACKET_ENUM;
			messageToSend.messageId = System::Messages::Network::ComponentUpdate.c_str( );
			messageToSend.parameters = parameters;

			for ( SystemAddressList::iterator i = m_clients.begin( ); i != m_clients.end( ); ++i )
			{
				NetworkUtils::SendNetworkMessage( messageToSend, ( *i ), m_networkInterface );
			}
		}
	}
}