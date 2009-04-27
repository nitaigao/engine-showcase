#include "ResourceCache.h"

#include "../Management/Management.h"
using namespace IO;

#include "Resource.h"

#include "../Logging/Logger.h"
using namespace Logging;

namespace Resources
{
	static const int MAX_RESOURCE_CACHE_SIZE = 50;

	ResourceCache::ResourceCache()
	{

	}

	ResourceCache::~ResourceCache()
	{
		for ( ResourceMap::iterator i = _resourceCache.begin( ); i != _resourceCache.end( ); ++i )
		{
			delete ( *i ).second;
		}
	}

	IResource* ResourceCache::GetResource( const std::string& filePath )
	{
		ResourceMap::iterator result = _resourceCache.find( filePath );

		if ( result != _resourceCache.end( ) )
		{
			std::stringstream cacheHitMessage;
			cacheHitMessage << "ResourceCache::GetResource: Cache Hit for " << filePath;
			Logger::Debug( cacheHitMessage.str( ) );

			return ( *result ).second;
		}

		FileBuffer* fileBuffer = Management::GetInstance( )->GetFileManager( )->GetFile( filePath, true );
		IResource* resource = new Resource( fileBuffer );

		_resourceCache.insert( std::make_pair( filePath, resource ) );

		std::stringstream cacheMissMessage;
		cacheMissMessage << "ResourceCache::GetResource: Cache Miss for " << filePath;
		Logger::Debug( cacheMissMessage.str( ) );

		return resource;
	}
};