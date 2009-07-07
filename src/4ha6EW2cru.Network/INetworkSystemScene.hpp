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
		
	};
};

#endif