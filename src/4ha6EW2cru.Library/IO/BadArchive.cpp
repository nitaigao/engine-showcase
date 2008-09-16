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
		Logger::GetInstance( )->Fatal( "BadArchive, There was an error adding a BAD file location to the FileManager" );
		throw ArchiveNotFoundException( );
	}
}

Ogre::DataStreamPtr BadArchive::open( const Ogre::String& filename ) const
{
	DataStreamPtr stream;

	FileBuffer* fileBuffer = FileManager::GetInstance( )->GetFile( filename );

	std::stringstream logMessage;
	logMessage << "BadArchive: Opening File" << filename;
	Logger::GetInstance( )->Debug( logMessage.str( ) );

	if ( fileBuffer != 0 )
	{
		std::ifstream* fileStream = new std::ifstream( fileBuffer->fileBytes );
		Ogre::FileStreamDataStream* dataStream = new FileStreamDataStream( filename, fileStream, fileBuffer->fileSize, true );
		stream = DataStreamPtr( dataStream );
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
	return FileInfoListPtr( new FileInfoList( ) );
}