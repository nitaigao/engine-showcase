/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   NetworkUtils.hpp
*  @date   2009/07/14
*/
#pragma once
#ifndef NETWORKUTILS_HPP
#define NETWORKUTILS_HPP

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
	struct NetworkMessage
	{
		unsigned char message;
		RakString messageId;
		AnyType::AnyTypeMap parameters;
	};

	enum NetworkTypes
	{
		NETWORKTYPE_EMPTY = 0,
		NETWORKTYPE_STRING = 1,
		NETWORKTYPE_BOOL = 2,
		NETWORKTYPE_VECTOR = 3,
		NETWORKTYPE_QUATERNION = 4
	};

	/*! 
	 *  A Collection of Utilities for manipulating Network Data
	 */
	class NetworkUtils
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~NetworkUtils( ) { };


		/*! Default Constructor
		*
		* @return (  )
		*/
		NetworkUtils( ) { };

		static inline BitStream* Serialize( const NetworkMessage& message )
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

		static inline NetworkMessage* DeSerialize( BitStream* bitStream )
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

		static unsigned char GetPacketIdentifier( Packet *packet )
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

	private:

		NetworkUtils( const NetworkUtils & copy ) { };
		NetworkUtils & operator = ( const NetworkUtils & copy ) { return *this; };
		
	};
};

#endif