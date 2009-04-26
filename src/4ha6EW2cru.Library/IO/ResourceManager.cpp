#include "ResourceManager.h"

#include "FileManager.h"
#include "Resource.h"

#include "../Logging/Logger.h"

namespace Resources
{
	static const int MAX_RESOURCE_CACHE_SIZE = 50;

	ResourceManager::ResourceManager()
	{
		_fileManager = new FileManager( );
	}

	ResourceManager::~ResourceManager()
	{
		for ( ResourceMap::iterator i = _resourceCache.begin( ); i != _resourceCache.end( ); ++i )
		{
			delete ( *i ).second;
		}

		delete _fileManager;
	}

	IResource* ResourceManager::GetResource( const std::string& filePath )
	{
		ResourceMap::iterator result = _resourceCache.find( filePath );

		if ( result != _resourceCache.end( ) )
		{
			std::stringstream cacheHitMessage;
			cacheHitMessage << "ResourceManager::GetResource: Cache Hit for " << filePath;
			Logger::GetInstance( )->Debug( cacheHitMessage.str( ) );

			return ( *result ).second;
		}

		FileBuffer* fileBuffer = _fileManager->GetFile( filePath, true );
		IResource* resource = new Resource( fileBuffer );

		_resourceCache.insert( std::make_pair( filePath, resource ) );

		std::stringstream cacheMissMessage;
		cacheMissMessage << "ResourceManager::GetResource: Cache Miss for " << filePath;
		Logger::GetInstance( )->Debug( cacheMissMessage.str( ) );

		return resource;
	}
};