#ifndef __FILEMANAGER_H
#define __FILEMANAGER_H

#include "FileBuffer.hpp"

/*!
	Manages the retrieval of files from the local File System
*/
class FileManager
{

public:

	FileManager( );

	/*! Retrieves an instance of the FileManager Singleton */
	static  FileManager* GetInstance( );

	/*! Releases all resources and the FileManager Singleton */
	void Release( );

	/*! Initlaized the Logging mechanism */
	static const bool Initialize( );

	/*! Adds a BAD File Store to the File System search path */
	bool AddFileStore( const std::string filePath );

	/*! Gets a FileBuffer object from the File System */
	FileBuffer* GetFile( const std::string filePath ) const;

	/*! Checks to see if a File exists */
	bool FileExists( const std::string filePath ) const;

private:

	~FileManager( ) { };
	FileManager( const FileManager & copy ) { };
	FileManager & operator = ( const FileManager & copy ) { return *this; };

};

#endif