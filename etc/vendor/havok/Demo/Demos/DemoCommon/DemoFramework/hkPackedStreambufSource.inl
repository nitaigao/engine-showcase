/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// The hkPackedStreambufFactory will work with any underlying steambuf

#include <Common/Base/System/Io/Reader/Memory/hkMemoryStreamReader.h>
#include <Common/Base/System/Io/Writer/Array/hkArrayStreamWriter.h>

class hkPackedStreambufSource : public hkStreambufFactory
{
	public:

		hkPackedStreambufSource(hkStreamReader* reader, hkBool seekBug=false);

		~hkPackedStreambufSource();

		virtual hkStreamWriter* openConsole(hkStreambufFactory::StdStream s)
		{
			HK_ASSERT2( 0xbc360d8, 0, "Console should have been opened before now");
			return HK_NULL;
		}

		virtual hkStreamWriter* openWriter(const char* name);

		virtual hkStreamReader* openReader(const char* name);

	protected:


		struct TocEntry
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, TocEntry);
			hkString m_filename;
			int m_length;
			int m_offset;
		};

		struct MemFile
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, MemFile);
			hkString m_filename;
			hkArray<char> m_data;
		};

	protected:
			
			// The reader for the packed file
		hkStreamReader* m_reader;

			// Entries for files in the packed file
		hkArray<TocEntry*> m_toc;

			// Entries for files written at runtime
		hkArray<MemFile*> m_memFiles;

	protected:

			// Does a fuzzy match on the filename to try and find an appropriate
			// entry. This method tries to be as forgiving as possible
			// it ignores case and will match substrings automagically
		TocEntry* findTocEntry(const hkString& filename);

			// Look for a file written at runtime
		MemFile* findMemFile(const hkString& filename);
};


template <typename T>
static void HK_CALL readRaw(hkStreamReader* sr, T& t)
{
	sr->read( reinterpret_cast<char*>(&t), sizeof(T));
}

hkPackedStreambufSource::hkPackedStreambufSource(hkStreamReader* reader, hkBool seekBug)
{
	// WARNING - all this assumes the same endian-ness as the packing tool
	// Switch this to use an hkIArchive if you need it for gamecube

	m_reader = reader;
	m_reader->addReference();
	HK_ASSERT(0x3b1f82fc, m_reader->seekTellSupported());

	if (m_reader->isOk())
	{
		// Read in the Toc
		// As with Zip files the Toc is stored at the end of the file
		hkUint32 tocStart;

		// Need to switch sign depending on whether or not we're reading from
		// PS2 cdrom
		if (seekBug)
		{
			m_reader->seek( sizeof(hkUint32), hkStreamReader::STREAM_END);
		}
		else
		{
			m_reader->seek( -hkSizeOf(hkUint32), hkStreamReader::STREAM_END);
		}
		readRaw(m_reader, tocStart);
		m_reader->seek(tocStart , hkStreamReader::STREAM_SET);

		hkUint32 numEntries;
		readRaw(m_reader, numEntries);

		for( unsigned i = 0; i < numEntries; ++i)
		{
			int filenameLength;
			readRaw(m_reader, filenameLength);

			hkArray<char> buf;
			buf.setSize(filenameLength+1);
			m_reader->read( buf.begin(), filenameLength);
			buf[filenameLength] = '\0';

			int fileOffset;
			int fileLength;

			// read filesize and offset
			readRaw(m_reader, fileLength);
			readRaw(m_reader, fileOffset);

			//hkprintf("File(%06d) Offset:%09d %s\n", fileLength, fileOffset, &buf[0]);

			// Store in Toc
			TocEntry* entry = new TocEntry;
			entry->m_filename = buf.begin();
			entry->m_length = fileLength;
			entry->m_offset = fileOffset;
			m_toc.pushBack(entry);
		}
	}
}

hkPackedStreambufSource::~hkPackedStreambufSource()
{
	for(int i = 0; i < m_toc.getSize(); ++i)
	{
		delete m_toc[i];
	}
	for(int j = 0; j < m_memFiles.getSize(); ++j)
	{
		delete m_memFiles[j];
	}
	m_reader->removeReference();
}

hkStreamWriter* hkPackedStreambufSource::openWriter(const char* name)
{
	hkArray<char>* array;

	if( MemFile* memFile = findMemFile(name) ) // overwrite?
	{
		memFile->m_data.clear();
		array = &memFile->m_data;
	}
	else // new file
	{
		memFile = new MemFile();
		memFile->m_filename = name;
		m_memFiles.pushBack( memFile );
		array = &memFile->m_data;
	}
	return new hkArrayStreamWriter( array, hkArrayStreamWriter::ARRAY_BORROW );
}

hkStreamReader* hkPackedStreambufSource::openReader(const char* name)
{
	if( MemFile* memFile = findMemFile(name) )
	{
		// Check memory files first as they are most up to date
		return new hkMemoryStreamReader( memFile->m_data.begin(), memFile->m_data.getSize(), hkMemoryStreamReader::MEMORY_INPLACE);
	}
	else if( TocEntry* entry = findTocEntry(name) )
	{
		m_reader->seek( entry->m_offset, hkStreamReader::STREAM_SET);

		char* buf = hkAllocate<char>( entry->m_length, HK_MEMORY_CLASS_STREAM );
		int nread = m_reader->read( buf, entry->m_length);

		if(nread == entry->m_length)
		{
			return new hkMemoryStreamReader(buf, entry->m_length, hkMemoryStreamReader::MEMORY_TAKE);
		}
		else
		{
			hkDeallocate<char>(buf);
		}
	}
	return HK_NULL;
}

// This does a pretty fuzzy match to try and find a file.
hkPackedStreambufSource::TocEntry* hkPackedStreambufSource::findTocEntry( const hkString& filename)
{
	// for PS2 we automatically strip host0: and cdrom0:
	hkString fname = filename;

	const hkString hostPrefix = filename.substr(0,6);
	if (hostPrefix.compareToIgnoreCase("host0:")==0)
	{
		fname = filename.substr(6, filename.getLength()-6);
	}

	const hkString cdromPrefix = filename.substr(0,7);
	if (cdromPrefix.compareToIgnoreCase("cdrom0:")==0)
	{
		fname = filename.substr(7, filename.getLength()-7);
	}

	for (int i=0; i< m_toc.getSize(); i++)
	{
		const TocEntry& current = *(m_toc[i]);

		if (fname.compareToIgnoreCase(current.m_filename) == 0)
		{
			return m_toc[i];
		}

		const int fLen = fname.getLength();
		const int eLen = m_toc[i]->m_filename.getLength();

		// If filname is shorter then compare it against the end of the entry filename
		if ((fLen < eLen) && (fname.compareToIgnoreCase(current.m_filename.substr(eLen - fLen, fLen))==0))
		{
			return m_toc[i];
		}

		// If filename is longer then compare end of it against the the entry filename
		if ((fLen > eLen) && (current.m_filename.compareToIgnoreCase(fname.substr(fLen - eLen, eLen))==0))
		{
			return m_toc[i];
		}
	}
	return HK_NULL;
}

hkPackedStreambufSource::MemFile* hkPackedStreambufSource::findMemFile(const hkString& filename)
{
	for (int i = 0; i < m_memFiles.getSize(); ++i)
	{
		if( m_memFiles[i]->m_filename == filename )
		{
			return m_memFiles[i];
		}
	}
	return HK_NULL;
}


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
