/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/FileBrowser/FileBrowser.h>

#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileReader.h>
#include <Common/Serialize/Packfile/Xml/hkXmlPackfileReader.h>

#include <Common/Base/System/Io/FileSystem/hkFileSystem.h>
#include <Graphics/Common/Font/hkgFont.h>
#include <Common/Serialize/Packfile/Binary/hkPackfileHeader.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileReader.h>
#include <Common/Serialize/Packfile/Xml/hkXmlPackfileReader.h>
#include <Common/Serialize/Util/hkStructureLayout.h>

hkBool HK_CALL hkFileFilter_isValidPackfile( const char* p )
{
	hkString path(p);

	if( path.endsWith(".xml") || path.endsWith(".hkx") )
	{
		hkIstream is( path.cString() );
		if( is.isOk() )
		{
			hkPackfileReader::FormatType format = hkPackfileReader::detectFormat(is.getStreamReader());
			if( format == hkPackfileReader::FORMAT_XML )
			{
				return true;
			}
			else if( format == hkPackfileReader::FORMAT_BINARY )
			{
				hkPackfileHeader pack;
				if( is.read(&pack, sizeof(pack)) == hkSizeOf(pack) )
				{
					return hkString::memCmp( pack.m_layoutRules, &hkStructureLayout::HostLayoutRules, sizeof(hkStructureLayout::HostLayoutRules)) == 0;
				}
			}
		}
	}

	return false;
}

hkBool HK_CALL hkDirectoryFilter_notCvs( const char* name )
{
	return hkString::strCmp(name, "CVS") != 0;
}

FileBrowser::FileBrowser(FileBrowser::FILTER fileFilter, FileBrowser::FILTER directoryFilter)
:	m_curDir("")
,	m_curIndex(0)
,   m_directoryFilter(directoryFilter)
,	m_fileFilter(fileFilter)
,	m_prevDir(" ") //anything which does not equal m_curDir
{
}


hkString FileBrowser::getFullPath( ) const
{
	if (m_curIndex < m_listing.getEntries().getSize())
	{
		hkString fullPath = m_curDir + "/" + m_listing.getEntries()[m_curIndex].name;
		return fullPath;
	}

	return "ERROR";
}

hkBool FileBrowser::parseAndDisplayDirectoryAndFiles(const hkDemoEnvironment* m_env, int startH, int startV, bool recurse)
{
	//m_env->m_textDisplay->outputText("Browse for file :", startH, startV, 0xffffffff);
	int textHeight = (5*(int)m_env->m_textDisplay->getFont()->getCharHeight()) / 4;
	int textWidth = (int)m_env->m_textDisplay->getFont()->getCharWidth();

	// get filtered listing
	if( m_prevDir != m_curDir )
	{
		m_listing.clear();
		m_toScan.clear();
		m_toScan.expandOne() = ""; // Force a rescan below
		hkFileSystem::DirectoryListing unfiltered;
		hkFileSystem::getInstance().listDirectory( m_curDir.cString(), unfiltered );
		const hkArray<hkFileSystem::Entry>& ents = unfiltered.getEntries();
		for( int i = 0; i < ents.getSize(); ++i )
		{
			if( ents[i].isDir() && m_directoryFilter(ents[i].name) )
			{
				m_listing.addEntry( ents[i] );
			}
		}
		m_prevDir = m_curDir;
	}

	if( m_listing.getEntries().getSize() > 50 )
	{
		m_toScan.clear();
	}

	static int stepTick;
	const int SLOWDOWN = 1;

	if( m_toScan.getSize() && (++stepTick % SLOWDOWN) == 0 )
	{
		hkFileSystem::DirectoryListing listing;
		hkString cur = m_curDir + "/" + m_toScan[0];
		hkFileSystem::getInstance().listDirectory( cur.cString(), listing );
		const hkArray<hkFileSystem::Entry>& ents = listing.getEntries();

		for( int i = 0; i < ents.getSize(); ++i )
		{
			hkString path = cur + ents[i].name;
			if( ents[i].isFile() && m_fileFilter( path.cString() ) )
			{
				m_listing.addFile( (m_toScan[0] + ents[i].name).cString() );
			}
			else if( recurse && ents[i].isDir() && m_directoryFilter( ents[i].name ) )
			{
				m_toScan.expandOne() = m_toScan[0] + ents[i].name + "/";
			}
		}
		m_toScan.removeAtAndCopy(0);
	}

	// current entries
	const hkArray<hkFileSystem::Entry>& entries = m_listing.getEntries();
	
	// show
	{
		hkObjectArray<hkString> curDir;
		m_curDir.split('/', curDir);
		for( int i = 0; i < curDir.getSize(); ++i )
		{
			m_env->m_textDisplay->outputText( (curDir[i]+"/"), startH, startV, 0xdddddddd );
			startH += 2*textWidth;
			startV += textHeight;
		}

		for( int i = 0; i < entries.getSize(); ++i )
		{
			const hkFileSystem::Entry& e = entries[i];
			hkString s; s.printf( e.isDir() ? "[ %s ]": "  %s", e.name);
			m_env->m_textDisplay->outputText( s, startH-textWidth, startV,  0xffffffff, 1, i!=m_curIndex ? -1 : 0 );
			startV += textHeight;
		}
		startH += textWidth;

		if( m_toScan.getSize() )
		{
			startV += textHeight;
			static int spinner; static const char spins[] = "/-\\|";
			hkString s; s.printf("%c %s", spins[spinner/SLOWDOWN], m_toScan[0].cString() );
			m_env->m_textDisplay->outputText( s, startH-textWidth, startV,  0xffffffff );
			spinner = (spinner+1) % (4*SLOWDOWN);
			startV += textHeight;
		}
	}

	// check pad status
	const hkgPad& pad = *m_env->m_gamePad;
	const hkgKeyboard& key = m_env->m_window->getKeyboard();
	
	if( pad.wasButtonPressed( HKG_PAD_DPAD_LEFT )  || key.wasKeyPressed(HKG_VKEY_BACK)) // go one level up
	{
		int lastSlash = m_curDir.lastIndexOf('/');
		if( lastSlash != -1 )
		{
			m_curDir.setAsSubstr( 0, lastSlash );
			m_curIndex = 0;
		}
		else
		{
			m_curDir = "";
		}
	}
	else if( entries.getSize() )
	{
		if( pad.wasButtonPressed( HKG_PAD_DPAD_DOWN ) ) // scroll down
		{
			m_curIndex = (m_curIndex+1) % entries.getSize();
		}
		else if( pad.wasButtonPressed( HKG_PAD_DPAD_UP ) ) // scroll up
		{
			m_curIndex = (m_curIndex-1+entries.getSize()) % entries.getSize();
		}
		else if( pad.wasButtonPressed( HKG_PAD_DPAD_RIGHT ) || pad.wasButtonPressed( HKG_PAD_BUTTON_0 ) )
		{
			if( entries[m_curIndex].isDir() ) // expand current directory
			{
				if( m_curDir.getLength() ) m_curDir += "/";
				m_curDir += entries[m_curIndex].name;
				m_curIndex = 0;
			}
			else if( entries[m_curIndex].isFile() && 	pad.wasButtonPressed( HKG_PAD_BUTTON_0 ) ) // open file
			{
				
				return true;
			}
		}
	}
	return false;
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
