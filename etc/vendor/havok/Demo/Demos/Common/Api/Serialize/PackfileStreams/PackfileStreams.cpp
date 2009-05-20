/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Base/System/Io/OStream/hkOStream.h>
#include <Common/Base/System/Io/Reader/hkStreamReader.h>
#include <Common/Base/System/Io/Reader/Buffered/hkBufferedStreamReader.h>
#include <Common/Base/System/Io/Writer/hkStreamWriter.h>
#include <Common/Serialize/hkSerialize.h>
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileReader.h>
#include <Common/Serialize/Packfile/hkPackfileWriter.h>

#define USE_VARIABLE(A) A=A

void readFromFileName(hkPackfileReader* reader)
{
	// stream is opened by hkFileSystem
	hkIstream is("Resources/Common/Api/Serialize/SimpleLoad/simple.xml");

	reader->loadEntireFile(is.getStreamReader());
}

void readFromMemory(hkPackfileReader* reader)
{
	char buf[100];
	// fill buffer using external read
	hkIstream is( buf, sizeof(buf) );

	reader->loadEntireFile(is.getStreamReader());
	// reader now has own copy, we can destroy "buf"
}

void readFromPersistentMemoryInplace(hkBinaryPackfileReader* binaryReader)
{
	void* externalMemory = 0;
	int externalMemorySize = 0;

	// special method only for binary files - no allocations, no streams.
	binaryReader->loadEntireFileInplace(externalMemory, externalMemorySize);

	// externalMemory must remain valid for as long as the reader
	// contents are accessed.
}

namespace
{
	class CustomStreamReader_NoSetMark_NoSeek : public hkStreamReader
	{
	};
	class CustomHeader
	{
		int data[10];
	};
}
void readFromUserStream(hkPackfileReader* reader)
{
	// seek is never required for loadEntireFile
	CustomStreamReader_NoSetMark_NoSeek* stream = HK_NULL;

	// streams need not be positioned as offset zero
	CustomHeader customHeader;
	stream->read( &customHeader, sizeof(CustomHeader));

	// Binary can load entire file without mark/seek
	{
		hkBinaryPackfileReader* bin = static_cast<hkBinaryPackfileReader*>(reader);
		bin->loadEntireFile( stream );
	}

	// Xml requires mark for parsing
	// hkBufferedStreamReader does the hard work for you
	{
		hkBinaryPackfileReader* bin = static_cast<hkBinaryPackfileReader*>(reader);
		hkBufferedStreamReader buffered(stream);
		bin->loadEntireFile( &buffered );
	}
}





void writeToFileName(hkPackfileWriter* writer, const hkPackfileWriter::Options& options)
{
	// stream is opened by hkFileSystem
	hkOstream os("Resources/Common/Api/Serialize/SimpleLoad/simple.xml");

	writer->save(os.getStreamWriter(), options);
}

void writeToFixedSizeMemory(hkPackfileWriter* writer, const hkPackfileWriter::Options& options)
{
	char buf[100];
	// If buf is too small, the file will be truncated.
	hkOstream os( buf, sizeof(buf) );

	writer->save(os.getStreamWriter(), options);
	int bufferUsed = os.getStreamWriter()->tell();
	USE_VARIABLE(bufferUsed);
}

void writeToExpandingMemory(hkPackfileWriter* writer, const hkPackfileWriter::Options& options)
{
	hkArray<char> buf;
	hkOstream os( buf ); // buf expanded as needed

	writer->save(os.getStreamWriter(), options);
	int bufferUsed = buf.getSize();
	USE_VARIABLE(bufferUsed);
}





class PackfileStreamsDemo : public hkDefaultDemo
{
	public:

		PackfileStreamsDemo( hkDemoEnvironment* env)
			: hkDefaultDemo(env), m_done(false)
		{
		}

		Result stepDemo()
		{
			if( 0 ) // we don't actually call these functions as they use dummy data
			{
				// reading
				readFromFileName(HK_NULL);
				readFromMemory(HK_NULL);
				readFromPersistentMemoryInplace(HK_NULL);
				readFromUserStream(HK_NULL);

				// writing
				hkPackfileWriter::Options options;
				writeToFileName(HK_NULL, options);
				writeToFixedSizeMemory(HK_NULL, options);
				writeToExpandingMemory(HK_NULL, options);
			}

			m_env->m_textDisplay->outputText("This is a source code demo", 20, 150, 0xffffffff);
			return DEMO_OK;
		}

		hkBool m_done;
};


static const char helpText[] = \
"This demo does not produce output. It is a source code example which shows\n" \
"how the packfile readers and writers interface with the stream classes.\n" \
"See also the plain streams demo.";


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO(PackfileStreamsDemo, HK_DEMO_TYPE_PRIME, "Packfile Stream examples", helpText);

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20090216)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2009
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
