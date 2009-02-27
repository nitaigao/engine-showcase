#ifndef __MOCK_FILEMANAGER_H
#define __MOCK_FILEMANAGER_H

#include "mockpp/mockpp.h"
#include "mockpp/MockObject.h"
#include "mockpp/ExpectationCounter.h"

USING_NAMESPACE_MOCKPP

#include "IO/IFileManager.hpp"

class Mock_FileManager : public IFileManager, public MockObject
{

public:

	Mock_FileManager( )
		: MockObject( "Mock_FileManager", 0 )
	{ };

	virtual ~Mock_FileManager( ) { };

	bool MountFileStore( const std::string filePath, const std::string mountPoint )
	{
		return false;
	}

	FileBuffer* GetFile( const std::string filePath ) const = 0;

	void SaveFile( const FileBuffer& fileBuffer ) const { };

	bool FileExists( const std::string filePath, bool throwOnFail = false ) const
	{
		return false;
	}

	FileSearchResultList* FileSearch( const std::string path, const std::string searchPattern, const bool recursive ) const
	{
		return new FileSearchResultList( );
	}

private:

	Mock_FileManager & operator = ( const Mock_FileManager & copy ) { return *this; };
};

#endif

#endif