#include "ClientNetworkProvider.h"

#include "NetworkUtils.h"

#include "Configuration/Configuration.h"
#include "Configuration/ConfigurationTypes.hpp"
using namespace Configuration;

#include <RakNetworkFactory.h>
#include <RakSleep.h>
#include <BitStream.h>
#include <RakPeer.h>
#include <MessageIdentifiers.h>
using namespace RakNet;

#include "Logging/Logger.h"
using namespace Logging;

#include "Events/Event.h"
#include "Events/EventData.hpp"
using namespace Events;

#include "Management/Management.h"

namespace Network
{
	ClientNetworkProvider::~ClientNetworkProvider()
	{
		if ( m_networkInterface != 0 )
		{
			delete m_networkInterface;
		}
	}

	void ClientNetworkProvider::Release()
	{
		m_networkInterface->Shutdown( m_configuration->Find( ConfigSections::Network, ConfigItems::Network::MaxClientReleaseTime ).As< int >( ) );
	}

	void ClientNetworkProvider::Initialize( Configuration::IConfiguration* configuration )
	{
		m_configuration = configuration;

		m_configuration->SetDefault( ConfigSections::Network, ConfigItems::Network::MaxClientConnections, 1 );
		m_configuration->SetDefault( ConfigSections::Network, ConfigItems::Network::ClientSleepTime, 0 );
		m_configuration->SetDefault( ConfigSections::Network, ConfigItems::Network::MaxClientReleaseTime, 10 );

		m_networkInterface = new RakPeer( );

		m_networkInterface->Startup( 
			m_configuration->Find( ConfigSections::Network, ConfigItems::Network::MaxClientConnections ).As< int >( ), 
			m_configuration->Find( ConfigSections::Network, ConfigItems::Network::ClientSleepTime ).As< int >( ), 
			&SocketDescriptor( ), 1 
			);
	}

	void ClientNetworkProvider::Update( const float& deltaMilliseconds )
	{
		RakSleep( m_configuration->Find( ConfigSections::Network, ConfigItems::Network::ClientSleepTime ).As< int >( ) );

		Packet* packet = m_networkInterface->Receive( );

		std::stringstream logMessage;

		if ( packet )
		{
			unsigned char packetId = NetworkUtils::GetPacketIdentifier( packet );
			DefaultMessageIDTypes messageType = static_cast< DefaultMessageIDTypes >( packetId );

			switch( packetId )
			{

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

				this->OnUserPacketReceived( packet );

				break;
			}

			if( logMessage.str( ).length( ) > 0 )
			{
				Debug( logMessage.str( ) );
			}

			m_networkInterface->DeallocatePacket( packet );
		}
	}

	void ClientNetworkProvider::Message( const std::string& message, AnyType::AnyTypeMap parameters )
	{
		if ( message == System::Messages::Network::Connect )
		{
			m_serverAddress = SystemAddress(
				parameters[ System::Parameters::Network::HostAddress ].As< std::string >( ).c_str( ),
				parameters[ System::Parameters::Network::Port ].As< int >( )
				);

			m_networkInterface->Connect( 
				m_serverAddress.ToString( false ),
				m_serverAddress.port,
				0, 0
			);
		}

		if( message == System::Messages::Network::Client::CharacterSelected )
		{
			this->PushMessage( System::Messages::Network::Client::CharacterSelected, parameters );
		}
	}

	void ClientNetworkProvider::OnUserPacketReceived( Packet* packet )
	{
		BitStream bitStream( packet->data, packet->length, false );
		NetworkMessage* message = NetworkUtils::DeSerialize( &bitStream );

		if ( message->messageId == System::Messages::Game::ChangeLevel.c_str( ) )
		{
			std::string levelName = message->parameters[ System::Parameters::Game::LevelName ].As< std::string >( );
			IEventData* eventData = new LevelChangedEventData( levelName );
			IEvent* event = new Event( GAME_LEVEL_CHANGED, eventData );
			Management::Get( )->GetEventManager( )->QueueEvent( event );
		}

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

		if( message->messageId == System::Messages::Network::Client::LevelLoaded.c_str( ) )
		{

		}

		delete message;
	}

	void ClientNetworkProvider::PushMessage( const std::string& message, AnyType::AnyTypeMap parameters )
	{
		NetworkMessage messageToSend;
		messageToSend.message = ID_USER_PACKET_ENUM;
		messageToSend.messageId = message.c_str( );
		messageToSend.parameters = parameters;

		this->SendNetworkMessage( messageToSend, m_serverAddress );

	}

	void ClientNetworkProvider::PushMessage( const std::string& componentId, const std::string& message, AnyType::AnyTypeMap parameters )
	{
		if ( 
			message == System::Messages::Move_Forward_Pressed ||
			message == System::Messages::Move_Backward_Pressed ||
			message == System::Messages::Move_Forward_Released ||
			message == System::Messages::Move_Backward_Released ||
			message == System::Messages::Strafe_Right_Pressed ||
			message == System::Messages::Strafe_Left_Pressed ||
			message == System::Messages::Strafe_Right_Released ||
			message == System::Messages::Strafe_Left_Released ||
			message == System::Messages::Jump ||
			message == System::Messages::Mouse_Moved
			)
		{
			parameters[ System::Parameters::Network::ComponentMessage ] = message;
			parameters[ System::Parameters::Network::ComponentId ] = componentId;

			NetworkMessage messageToSend;
			messageToSend.message = ID_USER_PACKET_ENUM;
			messageToSend.messageId = System::Messages::Network::ComponentUpdate.c_str( );
			messageToSend.parameters = parameters;

			this->SendNetworkMessage( messageToSend, m_serverAddress );
		}
	}

	void ClientNetworkProvider::SendNetworkMessage( const NetworkMessage& message, const SystemAddress& destination )
	{
		BitStream* bitStream = NetworkUtils::Serialize( message );
		m_networkInterface->Send( bitStream, MEDIUM_PRIORITY, RELIABLE, 0, m_serverAddress, false );
		Debug( message.messageId, "to",m_serverAddress.ToString( false ) );
		delete bitStream;
	}
}