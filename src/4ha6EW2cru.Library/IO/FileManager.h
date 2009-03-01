#ifndef __FILEMANAGER_H
#define __FILEMANAGER_H

#include "IFileManager.hpp"

#include "FileBuffer.hpp"
#include "FileSearchResult.hpp"

/*!
	Manages the retrieval of files from the local File System
*/
class FileManager : public IFileManager
{

public:

	FileManager( );
	~FileManager( ) { };

	/*! Retrieves an instance of the FileManager Singleton */
	static  FileManager* GetInstance( );

	/*! Releases all resources and the FileManager Singleton */
	void Release( );

	/*! Initlaized the Logging mechanism */
	static const bool Initialize( );

	/*! Adds a BAD File Store to the File System search path */
	bool MountFileStore( const std::string filePath, const std::string mountPoint );

	/*! Gets a FileBuffer object from the File System */
	FileBuffer* GetFile( const std::string filePath ) const;

	/*! Saves a FileBuffer to the File System */
	void SaveFile( const FileBuffer& fileBuffer ) const;

	/*! Checks to see if a File exists */
	bool FileExists( const std::string filePath, bool throwOnFail = false ) const;

	/*! Finds all files matching a specific file pattern */
	FileSearchResultList* FileSearch( const std::string path, const std::string searchPattern, const bool recursive ) const;

private:

	FileSearchResultList* _FileSearch( const std::string path, const std::string pattern, const bool recursive, FileSearchResultList* results ) const;

	FileManager( const FileManager & copy ) { };
	FileManager & operator = ( const FileManager & copy ) { return *this; };

};

#endif

