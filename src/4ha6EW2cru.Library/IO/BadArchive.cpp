#include "BadArchive.h"

#include <fstream>

#include "FileManager.h"

#include "../Logging/Logger.h"
#include "../Exceptions/ArchiveNotFoundException.hpp"

BadArchive::BadArchive( const std::string name, const std::string type ) : Archive( name, type )
{
	bool result = FileManager::GetInstance( )->AddFileStore( name );

	if ( !result )
	{
		ArchiveNotFoundException e( "BadArchive, There was an error adding a BAD file location to the FileManager" );
		Logger::GetInstance( )->Fatal( e.what( ) );
		throw e;
	}
}

Ogre::DataStreamPtr BadArchive::open( const Ogre::String& filename ) const
{
	DataStreamPtr stream;

	FileBuffer* fileBuffer = FileManager::GetInstance( )->GetFile( filename );

	std::stringstream logMessage;
	logMessage << "BadArchive: Opening File " << filename;
	Logger::GetInstance( )->Debug( logMessage.str( ) );

	if ( fileBuffer != 0 )
	{
		MemoryDataStream memoryStream( fileBuffer->fileBytes, fileBuffer->fileSize, false );
		stream = DataStreamPtr( new MemoryDataStream( memoryStream, true ) );
		delete fileBuffer;
	}

	return stream;
}

bool BadArchive::exists( const Ogre::String& filename )
{
	return FileManager::GetInstance( )->FileExists( filename );
}

StringVectorPtr BadArchive::find( const String& pattern, bool recursive /* = true */, bool dirs /* = false */ )
{
	return StringVectorPtr( new StringVector( ) );
}

FileInfoListPtr BadArchive::findFileInfo( const String& pattern, bool recursive /* = true */, bool dirs /* = false */ )
{
	FileInfoListPtr fileList = FileInfoListPtr( new FileInfoList( ) );

	if ( FileManager::GetInstance( )->FileExists( pattern ) )
	{
		FileInfo fileInfo;
		fileInfo.archive = this;
		fileList->push_back( fileInfo );
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