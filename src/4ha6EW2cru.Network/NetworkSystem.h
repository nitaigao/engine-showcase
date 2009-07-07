/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   NetworkSystem.h
*  @date   2009/07/06
*/
#pragma once
#ifndef NETWORKSYSTEM_H
#define NETWORKSYSTEM_H

#include "INetworkSystem.hpp"

#include "Configuration/IConfiguration.hpp"

#include <RakPeerInterface.h>

namespace Network
{
	/*! 
	 *  The Network System
	 */
	class NetworkSystem : public INetworkSystem
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~NetworkSystem( );


		/*! Default Constructor
		*
		* @return (  )
		*/
		NetworkSystem( )
			: m_networkInterface( 0 )
			, m_configuration( 0 )
		{

		}


		/*! Initializes the System
		*
		*  @return (void)
		*/
		virtual void Initialize( Configuration::IConfiguration* configuration );


		/*! Steps the System's internal data
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds );


		/*! Releases internal data of the System
		*
		*  @return (System::Types::Type)
		*/
		void Release( );


		/*! Messages the system with a command
		*
		* @param[in] const std::string & message
		* @param[in] AnyType::AnyTypeMap parameters
		* @return ( void )
		*/
		inline void Message( const std::string& message, AnyType::AnyTypeMap parameters );


		/*! Returns the type of the System
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::NETWORK; };


		/*! Creates a System Scene
		*
		*  @return (ISystemScene*)
		*/
		ISystemScene* CreateScene( );


		/*! Gets the System's Properties
		*
		*  @return (AnyTypeMap)
		*/
		inline AnyType::AnyTypeMap GetAttributes( ) const { return m_attributes; };


		/*! Sets a System Property
		*
		*  @param[in] const std::string & name
		*  @param[in] AnyType value
		*  @return (void)
		*/
		inline void SetAttribute( const std::string& name, AnyType value ) { m_attributes[ name ] = value; };


		/*! Sound Service Interface
		*
		*  @param[in] const std::string & actionName
		*  @param[in] AnyType::AnyTypeMap & parameters
		*  @return (AnyType::AnyTypeMap)
		*/
		AnyType::AnyTypeMap Execute( const std::string& actionName, AnyType::AnyTypeMap& parameters ) { };


	private:

		NetworkSystem( const NetworkSystem & copy ) { };
		NetworkSystem & operator = ( const NetworkSystem & copy ) { return *this; };

		unsigned char GetPacketIdentifier( Packet *p );

		RakPeerInterface* m_networkInterface;
		Configuration::IConfiguration* m_configuration;

		AnyType::AnyTypeMap m_attributes;
		
	};
};

#endif