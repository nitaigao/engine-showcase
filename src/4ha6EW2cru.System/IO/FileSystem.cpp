#include "FileSystem.h"

#include "physfs.h"

#include "../Logging/Logger.h"
using namespace Logging;

#include "../Utility/SearchUtility.hpp"

#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/AlreadyInitializedException.hpp"
#include "../Exceptions/FileNotFoundException.hpp"
#include "../Exceptions/FileWriteException.hpp"

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

namespace IO
{
	FileSystem::FileSystem( )
	{
		int result = PHYSFS_init( 0 );

		if( !result )
		{
			throw AlreadyInitializedException( "FileSystem::FileSystem - Attempted to initialized the FileSystem twice" );
		}
	}

	FileSystem::~FileSystem()
	{
		PHYSFS_deinit( );
	}

	void FileSystem::Initialize()
	{
		PHYSFS_setWriteDir( "../game" );
		this->Mount( "../game", "/" );

		std::string developmentPath = "../../../etc/data";

		if ( exists( developmentPath ) )
		{
			directory_iterator end;
			for ( directory_iterator i( developmentPath ); i != end; ++i )
			{
				if ( is_directory( i->status( ) ) )
				{
					std::stringstream path;
					path << i->path( );

					this->Mount( path.str( ), "/data" );
				}
			}
		}
	}

	bool FileSystem::FileExists( const std::string& filePath, const bool& throwOnFail ) const
	{
		int exists = PHYSFS_exists( filePath.c_str( ) );

		if ( !exists && throwOnFail )
		{
			throw FileNotFoundException( "FileSystem::GetFile - Could not find file within the Search Path" );
		}

		return ( exists > 0 );
	}

	bool FileSystem::Mount( const std::string& filePath, const std::string& mountPoint )
	{
		Info( "Mounting", filePath, "to", mountPoint );

		int result = ( PHYSFS_mount( filePath.c_str( ), mountPoint.c_str( ), 1 ) > 0 );

		if ( !result )
		{
			throw FileNotFoundException( "FileSystem::AddFileStore - Could not find File Store" );
		}

		return ( result > 0 );
	}

	FileBuffer* FileSystem::GetFile( const std::string& filePath, const bool& binary ) const
	{
		this->FileExists( filePath, true );

		PHYSFS_file *pFile = PHYSFS_openRead( filePath.c_str( ) );
		
		unsigned int fileLength = static_cast< unsigned int >( PHYSFS_fileLength( pFile ) );

		char* fileBytes = 0;
		
		if( binary )
		{
			fileBytes = new char[ fileLength ];
			PHYSFS_read( pFile, fileBytes, 1, fileLength );
		}
		else
		{
			fileBytes = new char[ fileLength + 1 ];
			PHYSFS_read( pFile, fileBytes, 1, fileLength );
			fileBytes[ fileLength ] = '\0';
		}

		FileBuffer* fileBuffer = new FileBuffer( fileBytes, fileLength, filePath );

		PHYSFS_close( pFile );

		return fileBuffer;
	}

	void FileSystem::SaveFile( const FileBuffer& fileBuffer ) const
	{
		PHYSFS_file* file = PHYSFS_openWrite( fileBuffer.filePath.c_str( ) );	

		if ( !file )
		{
			throw FileWriteException( "FileSystem::SaveFile - Unable to open the destination file for writing" );
		}

		PHYSFS_write( file, fileBuffer.fileBytes, 1, fileBuffer.fileLength );
		PHYSFS_close( file );
	}

	FileSearchResult::FileSearchResultList* FileSystem::FileSearch( const std::string& path, const std::string& searchPattern, const bool& recursive ) const
	{
		FileSearchResult::FileSearchResultList* results = new FileSearchResult::FileSearchResultList( );
		
		this->_FileSearch( path, searchPattern, recursive, results );
		
		return results;
	}

	FileSearchResult::FileSearchResultList* FileSystem::_FileSearch( const std::string& path, const std::string& pattern, const bool& recursive, FileSearchResult::FileSearchResultList* results ) const
	{
		char **rc = PHYSFS_enumerateFiles( path.c_str( ) );

		for ( char **i = rc; *i != NULL; i++ )
		{
			std::string fileName = *i;

			std::stringstream fullPath;
			fullPath << path << *i;

			if( PHYSFS_isDirectory( fullPath.str( ).c_str( ) ) && recursive )
			{
				std::stringstream newPath;
				newPath << path << *i << "/";

				this->_FileSearch( newPath.str( ), pattern, recursive, results );
			}
			else
			{
				SearchUtility searchUtility;
				searchUtility.SetSearchPattern( pattern );
				searchUtility.SetWildCard( "*" );

				std::string searchTerm = ( searchUtility.IsExactSearch( ) ) ? fullPath.str( ) : fileName;

				if( searchUtility.FindMatch( searchTerm ) )
				{
					FileSearchResult result;

					result.FileName = fileName;
					result.FilePath = fullPath.str( );

					results->push_back( result );
				}
			}
		}

		PHYSFS_freeList( rc );

		return results;
	}
}