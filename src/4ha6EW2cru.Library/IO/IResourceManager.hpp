#ifndef __IRESOURCEMANAGER_H
#define __IRESOURCEMANAGER_H

#include "IResource.hpp"

namespace Resources
{
	class IResourceManager
	{

	public:

		virtual ~IResourceManager( ) { };

		virtual Resources::IResource* GetResource( const std::string& filePath ) = 0;
		virtual void Update( float deltaMilliseconds ) = 0;

	};
};

#endif