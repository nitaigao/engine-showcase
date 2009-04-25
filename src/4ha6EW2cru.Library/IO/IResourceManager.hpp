#ifndef __IRESOURCEMANAGER_H
#define __IRESOURCEMANAGER_H

#include "IResource.hpp"

namespace Resource
{
	class IResourceManager
	{

	public:

		virtual ~IResourceManager( ) { };

		virtual Resource::IResource* GetResource( const std::string& filePath ) = 0;
		virtual void Update( float deltaMilliseconds ) = 0;

	};
};

#endif