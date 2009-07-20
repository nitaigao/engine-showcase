#include "BadArchive.h"
using namespace Ogre;

#include "IO/IResource.hpp"
using namespace Resources;
using namespace IO;

#include "Management/Management.h"

namespace Renderer
{
	Ogre::DataStreamPtr BadArchive::open( const Ogre::String& filename ) const
	{
		DataStreamPtr stream;

		IResource* resource = Management::Get( )->GetResourceManager( )->GetResource( filename );

		MemoryDataStream memoryStream( resource->GetFileBuffer( )->fileBytes, resource->GetFileBuffer( )->fileLength, false );
		stream = DataStreamPtr( new MemoryDataStream( memoryStream, true ) );

		return stream;
	}

	bool BadArchive::exists( const Ogre::String& filename )
	{
		return Management::Get( )->GetFileManager( )->FileExists( filename );
	}

	StringVectorPtr BadArchive::find( const String& pattern, bool recursive /* = true */, bool dirs /* = false */ )
	{
		StringVector* resultsVector = new StringVector( );

		FileSearchResult::FileSearchResultList* results = Management::Get( )->GetFileManager( )->FileSearch( "/data/", pattern, true );

		for( FileSearchResult::FileSearchResultList::iterator i = results->begin( ); i != results->end( ); ++i )
		{
			FileSearchResult result = ( *i );

			m_tableOfContents.insert( std::make_pair( result.FilePath, result ) );
			resultsVector->push_back( result.FilePath );
		}

		delete results;
		
		return StringVectorPtr( resultsVector );
	}

	FileInfoListPtr BadArchive::findFileInfo( const String& pattern, bool recursive /* = true */, bool dirs /* = false */ )
	{
		FileInfoListPtr fileList = FileInfoListPtr( new FileInfoList( ) );

		int wildCardIndex = pattern.find( '*' );

		if ( wildCardIndex != pattern.npos )
		{
			std::string searchTerm = pattern.substr( wildCardIndex + 1, pattern.length( ) - wildCardIndex );

			for( TableOfContents::iterator i = m_tableOfContents.begin( ); i != m_tableOfContents.end( ); ++i )
			{
				if ( ( *i ).first.find( searchTerm ) != ( *i ).first.npos )
				{
					FileInfo fileInfo;

					fileInfo.archive = this;
					fileInfo.filename = ( *i ).second.FilePath;
					fileInfo.basename = ( *i ).second.FileName;

					fileList->push_back( fileInfo );
				}
			}
		}
		else
		{
			TableOfContents::iterator i = m_tableOfContents.find( pattern );

			if ( i != m_tableOfContents.end( ) )
			{
				FileInfo fileInfo;

				fileInfo.archive = this;
				fileInfo.filename = ( *i ).second.FilePath;
				fileInfo.basename = ( *i ).second.FileName;

				fileList->push_back( fileInfo );
			}
		}

		return fileList;
	}

	StringVectorPtr BadArchive::list( bool recursive /* = true */, bool dirs /* = false */ )
	{
		return StringVectorPtr( new StringVector( ) );
	}

	FileInfoListPtr BadArchive::listFileInfo( bool recursive /* = true */, bool dirs /* = false */ )
	{
		return FileInfoListPtr( new FileInfoList( ) );
	}
}