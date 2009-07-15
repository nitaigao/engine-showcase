/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   ServerNetworkProvider.h
*  @date   2009/07/15
*/
#pragma once
#ifndef SERVERNETWORKPROVIDER_H
#define SERVERNETWORKPROVIDER_H

#include "INetworkProvider.hpp"
#include "INetworkSystem.hpp"

#include <RakPeerInterface.h>

#include <deque>

namespace Network
{
	/*! 
	 *  A Network Provider that servers to clients
	 */
	class ServerNetworkProvider : public INetworkProvider
	{

		typedef std::deque< SystemAddress > SystemAddressList;

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~ServerNetworkProvider( );


		/*! Default Constructor
		*
		* @return (  )
		*/
		ServerNetworkProvider( INetworkSystem* networkSystem )
			: m_configuration( 0 )
			, m_networkSystem( networkSystem )
		{

		}

		void Initialize( Configuration::IConfiguration* configuration );

		void Update( const float& deltaMilliseconds );

		void Message( const std::string& message, AnyType::AnyTypeMap parameters );

		void Release( );

		void PushMessage( const std::string& componentId, const std::string& message, AnyType::AnyTypeMap parameters );

	private:

		ServerNetworkProvider( const ServerNetworkProvider & copy ) { };
		ServerNetworkProvider & operator = ( const ServerNetworkProvider & copy ) { return *this; };

		void OnUserPacketReceived( Packet* packet );
		void OnClientConnected( Packet* packet );

		void SendMessageTo( const SystemAddress& networkAddress, const std::string& message, AnyType::AnyTypeMap parameters );

		Configuration::IConfiguration* m_configuration;

		INetworkSystem* m_networkSystem;
		RakPeerInterface* m_networkInterface;
		SystemAddressList m_clients;
		
	};
};

#endif