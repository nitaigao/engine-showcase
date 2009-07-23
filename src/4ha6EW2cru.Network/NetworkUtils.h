/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   NetworkUtils.hpp
*  @date   2009/07/14
*/
#pragma once
#ifndef NETWORKUTILS_HPP
#define NETWORKUTILS_HPP

#include "NetworkMessage.hpp"

#include "RakPeerInterface.h"
#include <BitStream.h>

namespace Network
{
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

		static RakNet::BitStream* Serialize( const NetworkMessage& message );

		static NetworkMessage* DeSerialize( RakNet::BitStream* bitStream );

		static unsigned char GetPacketIdentifier( Packet *packet );

		static void SendNetworkMessage( NetworkMessage& message, const SystemAddress& destination, RakPeerInterface* networkInterface );
		

	private:

		NetworkUtils( const NetworkUtils & copy ) { };
		NetworkUtils & operator = ( const NetworkUtils & copy ) { return *this; };
		
	};
};

#endif