/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   NetworkSystemScene.h
*  @date   2009/07/06
*/
#pragma once
#ifndef NETWORKSYSTEMSCENE_H
#define NETWORKSYSTEMSCENE_H

#include "INetworkSystemScene.hpp"

namespace Network
{
	/*! 
	 *  A Network System Scene
	 */
	class NetworkSystemScene : public INetworkSystemScene
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~NetworkSystemScene( ) { };


		/*! Default Constructor
		*
		* @return (  )
		*/
		NetworkSystemScene( INetworkSystem* system )
			: m_system( system )
		{

		}


		/*! Initializes the System Scene
		*
		*  @return (void)
		*/
		void Initialize( ) { };


		/*! Steps internal data of the SystemScene
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds ) { };


		/*! Destroys the System Scene
		*
		*  @return (void)
		*/
		inline void Destroy( ) { };


		/*! Gets the System::Types::Type of the SystemScene
		*
		*  @return (System::Types::Type)
		*/
		inline System::Types::Type GetType( ) const { return System::Types::NETWORK; };


		/*! Creates a SystemComponent specific to the SystemScene
		*
		*  @param[in] const std::string & name
		*  @param[in] const std::string & type
		*  @return (ISystemComponent*)
		*/
		ISystemComponent* CreateComponent( const std::string& name, const std::string& type );


		/*! Destroys a SystemComponent created by the SystemScene
		*
		*  @param[in] ISystemComponent * component
		*  @return (void)
		*/
		void DestroyComponent( ISystemComponent* component );


		/*! Returns the Network System
		*
		* @return ( INetworkSystem* )
		*/
		INetworkSystem* GetSystem( ) const { return m_system; };


		/*! Messages a component within the Scene
		*
		* @param[in] const std::string componentId
		* @param[in] const System::Message & message
		* @param[in] AnyType::AnyTypeMap parameters
		* @return ( void )
		*/
		void MessageComponent( const std::string componentId, const System::Message& message, AnyType::AnyTypeMap parameters );

	private:

		NetworkSystemScene( const NetworkSystemScene & copy ) { };
		NetworkSystemScene & operator = ( const NetworkSystemScene & copy ) { return *this; };

		ISystemComponent::SystemComponentMap m_components;

		INetworkSystem* m_system;
		
	};
};

#endif