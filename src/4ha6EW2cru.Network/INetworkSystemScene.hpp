/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   INetworkSystemScene.hpp
*  @date   2009/07/06
*/
#pragma once
#ifndef INETWORKSYSTEMSCENE_HPP
#define INETWORKSYSTEMSCENE_HPP

#include "System/ISystemScene.hpp"
#include "INetworkSystem.hpp"

namespace Network
{
	/*! 
	 *  A Network System Scene
	 */
	class INetworkSystemScene : public ISystemScene
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~INetworkSystemScene( ) { };


		/*! Returns the Network System
		*
		* @return ( INetworkSystem* )
		*/
		virtual INetworkSystem* GetSystem( ) const = 0;


		/*! Messages a component within the Scene
		*
		* @param[in] const std::string componentId
		* @param[in] const System::Message & message
		* @param[in] AnyType::AnyTypeMap parameters
		* @return ( void )
		*/
		virtual void MessageComponent( const std::string componentId, const System::Message& message, AnyType::AnyTypeMap parameters ) = 0;
		
	};
};

#endif