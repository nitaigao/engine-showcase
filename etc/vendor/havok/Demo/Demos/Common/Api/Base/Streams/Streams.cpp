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
#include <Common/Base/System/Io/Writer/hkStreamWriter.h>
#include <Common/Base/System/Io/Reader/Buffered/hkBufferedStreamReader.h>

//
//
// Output streams
//
//

// Ostreams are used for formatted text output.
static void functionTakingOstream(hkOstream& os)
{
}

// StreamWriters used for low level byte output.
static void functionTakingStreamWriter( hkStreamWriter* writer )
{
}

static void streamToFixedSizeMemory()
{
	char buf[100];
	// Create a stream from an in memory buffer.
	// Overrun bytes are simply discarded.
	// Passing true keeps the buffer null terminated.
	hkOstream os( buf, sizeof(buf), true );
	
	// Write to the stream.
	functionTakingOstream(os);

	// hkOstream is a convenient way of creating hkStreamWriter instances.
	// It creates and manages the reference to its hkStreamWriter.
	functionTakingStreamWriter( os.getStreamWriter() );

	// examine the buffer
	//printf("%s", buf);
}

static void streamToExpandingMemory()
{
	hkArray<char> buf;

	// If we know the buffer is likely to be small, 
	// we could avoid a heap allocation.
	// hkInplaceArray<char,128> buf;

	// buf will automatically expand as needed.
	hkOstream os( buf );

	// as before
	functionTakingOstream(os);
	functionTakingStreamWriter(os.getStreamWriter());

	// We now have buf.getSize() bytes of data from buf.begin()
}

static void streamToFile()
{
	// Opening with a name forwards to hkFileSystem
	// to do the platform specific calls and filename munging
	// (such as prefixing host0:, replacing backslashs, etc)
	hkOstream os("filename.txt");

	functionTakingOstream(os);

	hkStreamWriter* writer = os.getStreamWriter();
	// The underlying hkStreamWriters are reference counted
	// so we can let it be destroyed with the ostream (default)
	// or keep it alive by adding a reference.
	// In that case we also have responsibility to removeReference.
	writer->addReference();
	functionTakingStreamWriter(writer);
	writer->removeReference(); // need not be in this block
}

//
//
// Input streams
//
//

// Istreams are used for formatted text input.
static void functionTakingIstream( hkIstream& is )
{
}

// StreamReaders used for low level byte output.
static void functionTakingStreamReader( hkStreamReader* reader)
{
}

static void streamFromFixedSizeMemory()
{
	// fill these buffers externally.
	char buf[100];
	hkArray<char> array;

	// Create a stream from an in memory buffer.
	hkIstream is( buf, sizeof(buf) );
	hkIstream is2( array.begin(), array.getSize() );

	functionTakingIstream( is );

	// hkIstream is a convenient way of creating hkStreamReader instances.
	// It creates and manages the reference to its hkStreamReader.
	functionTakingStreamReader( is.getStreamReader() );
}

static void streamFromFile()
{
	// Opening with a name forwards to hkFileSystem
	// to do the platform specific calls and filename munging
	// (such as prefixing host0:, replacing backslashs, etc)
	hkIstream is("filename.txt");

	functionTakingIstream( is );
	functionTakingStreamReader( is.getStreamReader() );
}

// Several parts of the sdk (most notably the xml parser)
// require the stream reader to be able to rewind.
// The easiest way to have your custom stream do this
// is to implement only the basic
// operations and let a proxy class do the hard work.
struct MyStreamReader : public hkStreamReader
{
	MyStreamReader() { }
	virtual hkBool isOk() const { return true; }
	virtual int read(void* buf, int nbytes) { return 0; } // dummy implementaion
};

static void bufferingCustomInput()
{
	MyStreamReader mystream;
	// Real code would probably new/addReference/removeReference.
	hkBufferedStreamReader buffered( &mystream );

	// Stream now behaves as if it supported rewinding.
	functionTakingStreamReader( &buffered );
}

class StreamsDemo : public hkDefaultDemo
{
	public:

		StreamsDemo( hkDemoEnvironment* env)
			: hkDefaultDemo(env), m_done(false)
		{
		}

		Result stepDemo()
		{
			if( m_done == false )
			{
				m_done = true;
				hkReferencedObject::lockAll();

				// writing
				streamToFixedSizeMemory();
				streamToExpandingMemory();
				streamToFile();

				// reading
				streamFromFixedSizeMemory();
				streamFromFile();

				// custom input
				bufferingCustomInput();
				hkReferencedObject::unlockAll();
			}

			m_env->m_textDisplay->outputText("This is a source code demo", 20, 150, 0xffffffff);
			return DEMO_OK;
		}

		hkBool m_done;
};


static const char helpText[] = \
"This demo does not produce output. It is a source code example which shows:\n" \
" * loading and saving from memory buffers\n" \
" * hooking havok streams into your streams";

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO(StreamsDemo, HK_DEMO_TYPE_PRIME, "Stream examples", helpText);

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
