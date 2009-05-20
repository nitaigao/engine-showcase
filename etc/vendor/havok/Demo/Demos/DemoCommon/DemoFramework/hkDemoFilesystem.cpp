/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Demos/DemoCommon/DemoFramework/hkDemoFilesystem.h>
#include <Common/Base/System/Io/Reader/hkStreamReader.h>
#include <Common/Base/System/Io/Writer/hkStreamWriter.h>

namespace
{
	class NullStreamReader : public hkStreamReader
	{
		public:
			NullStreamReader() { }
			hkBool isOk() const { return false; }
			virtual int read(void* buf, int nbytes) { return 0; }
	};

	class NullStreamWriter : public hkStreamWriter
	{
		public:
			NullStreamWriter() { }
			hkBool isOk() const { return false; }
			virtual int write(const void* buf, int nbytes) { return 0;}
	};
}

hkDemoFileSystem::hkDemoFileSystem()
{
	hkFileSystem* parent = &hkFileSystem::getInstance();
	parent->addReference();
	m_parent = parent;
	addSearchPath("./");
	addSearchPath(hkDemoDatabase::getInstance().getPrefix().cString());
	m_locations.pushBack(hkString::strDup(""));
}

void hkDemoFileSystem::addSearchPath(const char* p, hkBool32 atFront)
{
	int pos = atFront ? 0 : m_locations.getSize();
	m_locations.insertAt(pos, hkString::strDup(p));
}

hkDemoFileSystem::~hkDemoFileSystem()
{
	m_parent->removeReference();
	for( int i = 0; i < m_locations.getSize(); ++i )
	{
		hkDeallocate( m_locations[i] );
	}
}

const char* HK_CALL hkDemoFileSystem::removeDemoBasePath( const char* pathIn, hkArray<char>& buffer )
{
	hkString path( pathIn );
	path.makeLowerCase();
	// strip everything up to the demo root folder
	hkString root("/demos/");
	root.makeLowerCase();

	const char* str = path.cString();
	const char* start = hkString::strStr(str, root.cString());
	if ( start != HK_NULL )
	{
		int pos = static_cast<int>(start - str) + root.getLength();
		buffer.setSize( path.getLength() - pos + 1 );
		// copy the part of the string that follows the demo prefix
		hkString::memCpy( buffer.begin(), pathIn+pos, buffer.getSize()-1 );
		buffer[ buffer.getSize()-1 ] = 0; // null terminate
		return buffer.begin();
	}
	else 
	{
		// nothing to do
		return pathIn;
	}
}

hkStreamWriter* hkDemoFileSystem::openWriter(const char* name)
{
	for(int i = 0; i < m_locations.getSize(); ++i)
	{
		hkString p = m_locations[i];
		p += name;
		hkStreamWriter* sb = m_parent->openWriter(p.cString());
		if(sb->isOk() || i == (m_locations.getSize()-1))
		{
			return sb;
		}
		else
		{
			sb->removeReference();
		}
	}
	return new NullStreamWriter();
}

hkStreamReader* hkDemoFileSystem::openReader(const char* name)
{
	for(int i = 0; i < m_locations.getSize(); ++i)
	{
		hkString p = m_locations[i];
		p += name;

		hkStreamReader* sb = m_parent->openReader(p.cString());
		if(sb->isOk() || i == (m_locations.getSize()-1))
		{
			return sb;
		}
		else
		{
			sb->removeReference();
		}
	}
	return new NullStreamReader();
}

hkResult hkDemoFileSystem::listDirectory(const char* basePath, DirectoryListing& listingOut)
{
	for(int i = 0; i < m_locations.getSize(); ++i)
	{
		hkString p = m_locations[i];
		p += basePath;
		if( m_parent->listDirectory(p.cString(), listingOut ) == HK_SUCCESS )
		{
			return HK_SUCCESS;
		}
	}
	return HK_FAILURE;
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
