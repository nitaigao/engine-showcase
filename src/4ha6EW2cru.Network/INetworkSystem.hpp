/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   INetworkSystem.hpp
*  @date   2009/04/27
*/
#ifndef INETWORKSYSTEM_HPP
#define INETWORKSYSTEM_HPP

#include <System/ISystem.hpp>

namespace Network
{
	/*!
	*  The Sound System 
	*/
	class INetworkSystem : public ISystem
	{

	public:

		/*! Default Destructor
		*
		*  @return ()
		*/
		virtual ~INetworkSystem( ) { };

	};
};

#endif