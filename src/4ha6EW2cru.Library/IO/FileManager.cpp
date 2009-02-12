#include "FileManager.h"

#include <windows.h>
#include <sstream>
#include "physfs.h"

#include "../Logging/Logger.h"
#include "../Common/Paths.hpp"
#include "../Utility/SearchUtility.hpp"

#include "../Exceptions/UnInitializedException.hpp"
#include "../Exceptions/AlreadyInitializedException.hpp"
#include "../Exceptions/FileNotFoundException.hpp"
#include "../Exceptions/FileWriteException.hpp"

static FileManager* g_FileManagerInstance = 0;

FileManager* FileManager::GetInstance( )
{
	if ( g_FileManagerInstance == 0 )
	{
		UnInitializedException e( "FileManager::GetInstance - FileManager has not been Intialized" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	return g_FileManagerInstance;
}

void FileManager::Release( )
{
	Logger::GetInstance( )->Info( "Releasing the File Manager" );

	PHYSFS_deinit( );

	delete g_FileManagerInstance;
	g_FileManagerInstance = 0;
}

const bool FileManager::Initialize( )
{
	if ( g_FileManagerInstance != 0 )
	{
		std::string errorMessage = "FileMananger has already been Intialized";
		Logger::GetInstance( )->Fatal( errorMessage );
		throw AlreadyInitializedException( errorMessage );
	}

	Logger::GetInstance( )->Info( "Initializing the File Manager" );

	g_FileManagerInstance = new FileManager( );

	return true;	
}

FileManager::FileManager( )
{
	PHYSFS_init( 0 );
	PHYSFS_addToSearchPath( Paths::GetGameFolderPath( ).c_str( ), 1 );
	PHYSFS_setWriteDir( Paths::GetGameFolderPath( ).c_str( ) );
}

bool FileManager::FileExists( const std::string filePath, bool throwOnFail ) const
{
	int exists = PHYSFS_exists( filePath.c_str( ) );

	if ( !exists && throwOnFail )
	{
		FileNotFoundException e( "FileManager::GetFile - Could not find file within the Search Path" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	return exists;
}

bool FileManager::MountFileStore( const std::string filePath, const std::string mountPoint )
{
	int result = ( PHYSFS_mount( filePath.c_str( ), mountPoint.c_str( ), 1 ) > 0 );

	if ( !result )
	{
		FileNotFoundException e( "FileManager::AddFileStore - Could not find File Store" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	return result;
}

FileBuffer* FileManager::GetFile( const std::string filePath ) const
{
	std::stringstream logMessage;
	logMessage << "Loading File: " << filePath;
	Logger::GetInstance( )->Debug( logMessage.str( ) );

	this->FileExists( filePath, true );

	PHYSFS_file *pFile = PHYSFS_openRead( filePath.c_str( ) );
	
	int fileLength = PHYSFS_fileLength( pFile );

	char* fileBytes = new char[ fileLength + 1 ];
	PHYSFS_read( pFile, fileBytes, 1, fileLength );
	fileBytes[ fileLength ] = '\0';

	FileBuffer* fileBuffer = new FileBuffer( fileBytes, fileLength, filePath );

	PHYSFS_close( pFile );

	return fileBuffer;
}

void FileManager::SaveFile( const FileBuffer& fileBuffer ) const
{
	PHYSFS_file* file = PHYSFS_openWrite( fileBuffer.filePath.c_str( ) );	

	if ( !file )
	{
		FileWriteException e( "FileManager::SaveFile - Unable to open the destination file for writing" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}

	PHYSFS_write( file, fileBuffer.fileBytes, 1, fileBuffer.fileLength );
	PHYSFS_close( file );
}

FileSearchResultList* FileManager::FileSearch( const std::string path, const std::string searchPattern, const bool recursive ) const
{
	FileSearchResultList* results = new FileSearchResultList( );
	
	_FileSearch( path, searchPattern, recursive, results );
	
	return results;
}

FileSearchResultList* FileManager::_FileSearch( const std::string path, const std::string pattern, const bool recursive, FileSearchResultList* results ) const
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

			_FileSearch( newPath.str( ), pattern, recursive, results );
		}
		else
		{
			SearchUtility searchUtility;
			searchUtility.SetSearchPattern( pattern );
			searchUtility.SetWildCard( "*" );

			std::string searchTerm = ( searchUtility.IsExactSearch( ) ) ? fullPath.str( ) : fileName;

			if ( fileName.find( "introscreen.layout" ) != std::string::npos )
			{
				int a = 1;
			}

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