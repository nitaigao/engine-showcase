#include "NetworkUtils.h"

#include "Logging/Logger.h"
using namespace Logging;

#include "Maths/MathVector3.hpp"
#include "Maths/MathQuaternion.hpp"
using namespace Maths;

#include <RakString.h>
#include <BitStream.h>
#include <MessageIdentifiers.h>
using namespace RakNet;

#include "System/SystemType.hpp"

namespace Network
{

	BitStream* NetworkUtils::Serialize( const NetworkMessage& message )
	{
		BitStream* bitStream = new BitStream( );

		bitStream->Write( ( MessageID ) ID_USER_PACKET_ENUM );
		bitStream->Write( message.messageId );

		int parameterCount = 0;

		BitStream* parameterBitStream = new BitStream( );

		for( AnyType::AnyTypeMap::const_iterator i = message.parameters.begin( ); i != message.parameters.end( ); ++i )
		{
			if ( 
				( *i ).first == System::Attributes::Position || 
				( *i ).first == System::Attributes::Orientation || 
				( *i ).first == System::Parameters::Network::ComponentId ||
				( *i ).first == System::Parameters::Network::ComponentMessage ||
				( *i ).first == System::Parameters::Game::LevelName
				)
			{
				parameterCount++;

				RakString parameterId( ( *i ).first.c_str( ) );
				parameterBitStream->Write( parameterId );

				if ( ( *i ).second.GetType( ) == typeid( std::string ) )
				{
					RakString value( ( *i ).second.As< std::string >( ).c_str( ) );
					parameterBitStream->Write( static_cast< int >( NETWORKTYPE_STRING ) );
					parameterBitStream->Write( value );
				}
				else if ( ( *i ).second.GetType( ) == typeid( MathVector3 ) )
				{
					parameterBitStream->Write( static_cast< int > ( NETWORKTYPE_VECTOR ) );
					parameterBitStream->Write( ( *i ).second.As< MathVector3 >( ) );
				}
				else if ( ( *i ).second.GetType( ) == typeid( MathQuaternion ) )
				{
					parameterBitStream->Write( static_cast< int > ( NETWORKTYPE_QUATERNION ) );
					parameterBitStream->Write( ( *i ).second.As< MathQuaternion >( ) );
				}
			}
		}

		bitStream->Write( parameterCount );
		bitStream->Write( parameterBitStream );

		delete parameterBitStream;

		return bitStream;
	}

	NetworkMessage* NetworkUtils::DeSerialize( BitStream* bitStream )
	{
		NetworkMessage* message = new NetworkMessage( );

		bitStream->Read( message->message );
		bitStream->Read( message->messageId );

		int parameterCount = 0;
		bitStream->Read( parameterCount );

		for( int i = 0; i < parameterCount; i++ )
		{
			RakString parameterId;
			bitStream->Read( parameterId );

			NetworkTypes networkType;
			bitStream->Read( networkType );

			switch ( networkType )
			{

			case NETWORKTYPE_STRING:
				{
					RakString parameter;
					bitStream->Read( parameter );
					message->parameters[ parameterId.C_String( ) ] = std::string( parameter.C_String( ) );

					break;
				}

			case NETWORKTYPE_VECTOR:
				{
					MathVector3 parameter;
					bitStream->Read( parameter );
					message->parameters[ parameterId.C_String( ) ] = parameter;

					break;
				}

			case NETWORKTYPE_QUATERNION:
				{
					MathQuaternion parameter;
					bitStream->Read( parameter );
					message->parameters[ parameterId.C_String( ) ] = parameter;

					break;
				}
			}

		}

		return message;
	}

	unsigned char NetworkUtils::GetPacketIdentifier( Packet *packet )
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

	void NetworkUtils::SendNetworkMessage( NetworkMessage& message, const SystemAddress& destination, RakPeerInterface* networkInterface )
	{
		BitStream* bitStream = NetworkUtils::Serialize( message );

		networkInterface->Send( bitStream, MEDIUM_PRIORITY, RELIABLE, 0, destination, false );

		delete bitStream;

		if ( message.messageId == System::Messages::Network::ComponentUpdate.c_str( ) )
		{
			std::string messageText = message.parameters[ System::Parameters::Network::ComponentMessage ].As< System::Message >( );
			messageText = ( messageText.empty( ) ) ? message.messageId.C_String( ) : messageText;
			Debug( messageText, "to", destination.ToString( false ) );
		}
		else
		{
			Debug( message.messageId, "to", destination.ToString( false ) );
		}
	}
}