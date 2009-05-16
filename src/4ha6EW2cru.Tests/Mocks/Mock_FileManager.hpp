#ifndef MOCK_FILEMANAGER_H
#define MOCK_FILEMANAGER_H

#include "mockpp/mockpp.h"
#include "mockpp/MockObject.h"
#include "mockpp/ExpectationCounter.h"

USING_NAMESPACE_MOCKPP

#include "IO/IFileSystem.hpp"
using namespace IO;

class Mock_FileManager : public IFileSystem, public MockObject
{

public:

	Mock_FileManager( )
		: MockObject( "Mock_FileManager", 0 )
		, getFile_count( "Mock_FileManager/GetFile", this )
	{ };

	virtual ~Mock_FileManager( ) { };

	bool Mount( const std::string filePath, const std::string mountPoint )
	{
		return false;
	}

	ExpectationCounter getFile_count;

	FileBuffer* GetFile( const std::string filePath ) const
	{
		Mock_FileManager* fileManager = const_cast< Mock_FileManager* >( this );
		fileManager->getFile_count.inc( );
		int length = 5;
		char* fileBytes = new char[ length ];
		return new FileBuffer( fileBytes, length, "" );
	}

	void SaveFile( const FileBuffer& fileBuffer ) const { };

	bool FileExists( const std::string filePath, bool throwOnFail = false ) const
	{
		return false;
	}

	FileSearchResult::FileSearchResultList* FileSearch( const std::string path, const std::string searchPattern, const bool recursive ) const
	{
		return new FileSearchResult::FileSearchResultList( );
	}

private:

	Mock_FileManager & operator = ( const Mock_FileManager & copy ) { return *this; };
};

#endif