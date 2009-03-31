#include "BadArchive.h"

#include <fstream>

#include "FileManager.h"
#include "../System/Management.h"

#include "../Logging/Logger.h"
#include "../Exceptions/ArchiveNotFoundException.hpp"

Ogre::DataStreamPtr BadArchive::open( const Ogre::String& filename ) const
{
	DataStreamPtr stream;

	FileBuffer* fileBuffer = Management::GetInstance( )->GetFileManager( )->GetFile( filename, false );

	std::stringstream logMessage;
	logMessage << "BadArchive: Opening File " << filename;
	Logger::GetInstance( )->Debug( logMessage.str( ) );

	if ( fileBuffer != 0 )
	{
		MemoryDataStream memoryStream( fileBuffer->fileBytes, fileBuffer->fileLength, false );
		stream = DataStreamPtr( new MemoryDataStream( memoryStream, true ) );
		delete fileBuffer;
	}

	return stream;
}

bool BadArchive::exists( const Ogre::String& filename )
{
	return Management::GetInstance( )->GetFileManager( )->FileExists( filename );
}

StringVectorPtr BadArchive::find( const String& pattern, bool recursive /* = true */, bool dirs /* = false */ )
{
	return StringVectorPtr( new StringVector( ) );
}

FileInfoListPtr BadArchive::findFileInfo( const String& pattern, bool recursive /* = true */, bool dirs /* = false */ )
{
	FileInfoListPtr fileList = FileInfoListPtr( new FileInfoList( ) );

	/*	Ogre's resource manager seems to assume that all zip archives are flat in folder structure, so we will 
		be performing a recursive search here throughout the BAD folder structure for whatever Ogre requires */

	FileSearchResultList* results = Management::GetInstance( )->GetFileManager( )->FileSearch( "/", pattern, true );

	for( FileSearchResultList::iterator i = results->begin( ); i != results->end( ); ++i )
	{
		FileInfo fileInfo;

		fileInfo.archive = this;
		fileInfo.filename = ( *i ).FilePath;
		fileInfo.basename = ( *i ).FileName;
		//fileInfo.path = ( *i ).FilePath;

		fileList->push_back( fileInfo );
	}

	delete results;

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