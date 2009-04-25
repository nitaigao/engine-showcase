#ifndef __RESOURCEMANAGER_H
#define __RESOURCEMANAGER_H

#include <map>

#include "IResourceManager.hpp"
#include "IFileManager.hpp"

namespace Resource
{
	class ResourceManager : public IResourceManager
	{

		typedef std::map< std::string, IResource* > ResourceMap;

	public:

		virtual ~ResourceManager( );

		ResourceManager( );

		IResource* GetResource( const std::string& filePath );
		void Update( float deltaMilliseconds ) { };

	private:

		IFileManager* _fileManager;
		ResourceMap _resourceCache;

	};
};

#endif