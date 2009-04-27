#include "FileBufferFixture.h"

#include "IO/FileBuffer.hpp"
using namespace IO;

#include "../Suites.h"
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( FileBufferFixture, Suites::IOSuite( ) );

void FileBufferFixture::Should_Contain_Given_Values( )
{
	char* fileBytes = new char[ 1 ];
	std::string filePath = "path";
	int fileLength = 99;

	FileBuffer* fileBuffer = new FileBuffer( fileBytes, fileLength, filePath );

	CPPUNIT_ASSERT( fileBytes == fileBuffer->fileBytes );
	CPPUNIT_ASSERT( filePath == fileBuffer->filePath );
	CPPUNIT_ASSERT( fileLength == fileBuffer->fileLength );

	delete fileBuffer;
}
