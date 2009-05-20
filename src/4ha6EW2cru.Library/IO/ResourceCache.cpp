#include "ResourceCache.h"

#include "../Management/Management.h"
using namespace IO;

#include "Resource.h"

#include "../Logging/Logger.h"
using namespace Logging;

namespace Resources
{
	static const int MAX_RESOURCE_CACHE_SIZE = 50;

	ResourceCache::~ResourceCache()
	{
		for ( IResource::ResourceMap::iterator i = m_resourceCache.begin( ); i != m_resourceCache.end( ); ++i )
		{
			delete ( *i ).second;
		}
	}

	IResource* ResourceCache::GetResource( const std::string& filePath )
	{
		IResource::ResourceMap::iterator result = m_resourceCache.find( filePath );

		if ( result != m_resourceCache.end( ) )
		{
			std::stringstream cacheHitMessage;
			cacheHitMessage << "ResourceCache::GetResource: Cache Hit for " << filePath;
			Logger::Debug( cacheHitMessage.str( ) );

			return ( *result ).second;
		}

		FileBuffer* fileBuffer = Management::GetInstance( )->GetFileManager( )->GetFile( filePath, true );
		IResource* resource = new Resource( fileBuffer );

		m_resourceCache.insert( std::make_pair( filePath, resource ) );

		std::stringstream cacheMissMessage;
		cacheMissMessage << "ResourceCache::GetResource: Cache Miss for " << filePath;
		Logger::Debug( cacheMissMessage.str( ) );

		return resource;
	}
};