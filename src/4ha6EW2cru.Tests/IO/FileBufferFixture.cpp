#include "FileBufferFixture.h"

#include "IO/FileBuffer.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION( FileBufferFixture );

void FileBufferFixture::Should_Contain_Given_Values( )
{
	FileBuffer* fileBuffer = new FileBuffer( );

	char* fileBytes = new char[ 1 ];
	std::string filePath = "path";
	int fileSize = 99;

	fileBuffer->fileBytes = fileBytes;
	fileBuffer->filePath = filePath;
	fileBuffer->fileSize = fileSize;

	CPPUNIT_ASSERT( fileBytes == fileBuffer->fileBytes );
	CPPUNIT_ASSERT( filePath == fileBuffer->filePath );
	CPPUNIT_ASSERT( fileSize == fileBuffer->fileSize );

	delete fileBuffer;
}
