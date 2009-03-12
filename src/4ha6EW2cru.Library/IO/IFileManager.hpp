#ifndef __IFILEMANAGER_H
#define __IFILEMANAGER_H

#include "FileBuffer.hpp"
#include "FileSearchResult.hpp"

class IFileManager
{

public:

	virtual ~IFileManager( ) { };

	virtual void Release( ) { };

	virtual bool MountFileStore( const std::string filePath, const std::string mountPoint ) = 0;

	virtual FileBuffer* GetFile( const std::string filePath, bool binary ) const = 0;

	virtual void SaveFile( const FileBuffer& fileBuffer ) const = 0;

	virtual bool FileExists( const std::string filePath, bool throwOnFail = false ) const = 0;

	virtual FileSearchResultList* FileSearch( const std::string path, const std::string searchPattern, const bool recursive ) const = 0;

};

#endif