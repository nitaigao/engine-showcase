/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// precompiled header file
#include <Demos/DemoCommon/DemoFramework/hkDemo.h>
#include <Demos/demos.h>
#include <Demos/DemoCommon/DemoFramework/hkDemo.h>
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Demos/DemoCommon/DemoFramework/hkDemoFramework.h>
#include <Common/Base/System/Io/FileSystem/hkFileSystem.h>
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>


#include <Common/Base/Spu/Util/hkSpuUtil.h>

hkDemoEnvironment::hkDemoEnvironment()	:
	m_window(0), 
	m_displayHandler(0),	
	m_sceneConverter(0),
	m_displayWorld(0),  
	m_textDisplay(0),
	m_gamePad(0),
	m_gamePadB(0),
	m_controlKeyReserved(false),
	m_mousePickingEnabled(true),
	m_options(0),
	m_cpuMhz(
#if HK_PLATFORM_IS_CONSOLE == 0
	2000
#elif defined(HK_PLATFORM_PSP)
	300
#elif defined(HK_PLATFORM_PS2)
	450
#elif defined(HK_PLATFORM_GC)
	450
#elif defined(HK_PLATFORM_XBOX)
	450
#elif defined(HK_PLATFORM_PS3_PPU)
	1600
#elif defined(HK_PLATFORM_XBOX360)
	1600
#else
#	error please add a mhz rating for this platform
#endif
	),
	m_reportingLevel(REPORT_ALL),
	m_exitCode(0)
{
}

// COM-305 : You can use reserve/release in your demos if you want to use the CTRL key (PC only) 
//	         for something different than switching game pads
void hkDemoEnvironment::reserveControlKey()
{
	HK_WARN_IF (m_controlKeyReserved, 0x0, "Reserving CTRL key twice?");
	m_controlKeyReserved = true;
}
void hkDemoEnvironment::releaseControlKey()
{
	HK_WARN_IF (!m_controlKeyReserved, 0x0, "Releasing CTRL key twice?");
	m_controlKeyReserved = false;
}

void hkDemo::getNumTimerStreams( int& numThreadStreams, int& numSpuStreams, int maxThreads ) const
{
	numThreadStreams = 1;
	numSpuStreams = 0;
}

void hkDemo::getTimerStreamInfo( hkArray<hkTimerData>& timerStreams, hkArray<hkTimerData>& spuStreams, int maxThreads  )
{
	if (maxThreads > 0)
	{
		hkTimerData info;
		info.m_streamBegin = hkMonitorStream::getInstance().getStart();
		info.m_streamEnd = hkMonitorStream::getInstance().getEnd();
		timerStreams.pushBack(info);
	}
}

void hkDemo::resetTimerStreams()
{
	hkMonitorStream::getInstance().reset();
}

hkBool hkDemoEnvironment::getDesiredKeyboardCtrlStateForGamePad( int padIndex ) const
{
	// Normally, pad1 is emulated by holding CTRL (pad0 is emulated when CTRL is not being held).
	// When only one pad is present, we want pad1 to be emulated with no CTRL so we flip the flag.

	bool ctrlState = (	!m_options->m_forceKeyboardGamepad && 
						m_window->hasGamePads() && 
						m_window->getGamePad(0).isValid() && 
						!m_window->getGamePad(1).isValid() );

	return ( padIndex ? !ctrlState : ctrlState );
}

hkBool hkDemoEnvironment::wasKeyPressed( int padIndex, HKG_KEYBOARD_VKEY key ) const
{
	const hkgKeyboard& keyboard = m_window->getKeyboard();

	hkBool desiredCtrl = getDesiredKeyboardCtrlStateForGamePad( padIndex );
	hkBool ctrlState = keyboard.getKeyState(HKG_VKEY_CONTROL);

	if ( desiredCtrl == ctrlState )
	{
		return keyboard.wasKeyPressed( key );
	}

	return false;
}

hkBool hkDemoEnvironment::wasKeyReleased( int padIndex, HKG_KEYBOARD_VKEY key ) const
{
	const hkgKeyboard& keyboard = m_window->getKeyboard();

	hkBool desiredCtrl = getDesiredKeyboardCtrlStateForGamePad( padIndex );
	hkBool ctrlState = keyboard.getKeyState(HKG_VKEY_CONTROL);

	if ( desiredCtrl == ctrlState )
	{
		return keyboard.wasKeyReleased( key );
	}

	return false;
}

hkBool hkDemoEnvironment::getKeyState( int padIndex, HKG_KEYBOARD_VKEY key ) const
{
	const hkgKeyboard& keyboard = m_window->getKeyboard();

	hkBool desiredCtrl = getDesiredKeyboardCtrlStateForGamePad( padIndex );
	hkBool ctrlState = keyboard.getKeyState(HKG_VKEY_CONTROL);

	if ( desiredCtrl == ctrlState )
	{
		return keyboard.getKeyState( key );
	}

	return false;
}

void hkDemo::setDemoName(hkString& name )
{
	m_name = name;
}

void HK_CALL hkDemo::scanDirectoryAndCreateDemosForHkxFiles(hkString path, hkDemoEntry& thisEntry, hkArray<hkDemoEntry*>& entriesOut )
{
	hkFileSystem& fs =  hkFileSystem::getInstance();

	hkFileSystem::DirectoryListing listing;
	fs.listDirectory( path.cString(), listing );

	const hkArray<hkFileSystem::Entry>& entries = listing.getEntries();

	hkString ending;

	// enable this if you want to filter for Havok specific layout based filename rules
	//hkAssetManagementUtil::getFileEnding( ending, hkStructureLayout::HostLayoutRules );
	ending += ".hkx";

	for (int i =0; i < entries.getSize(); i++ )
	{
		const hkFileSystem::Entry& entry = entries[i];
		if ( entry.isDir() )
		{
			hkDemoEntry a;
			a = thisEntry;
			a.m_menuPath += "/";
			a.m_menuPath += entry.name;
			hkString childPath = path;
			childPath += entry.name;
			childPath += "/";
			scanDirectoryAndCreateDemosForHkxFiles( childPath, a, entriesOut );
			continue;
		}

		hkString fileName( entry.name );
		if ( !fileName.endsWith(ending))
		{
			continue;
		}
		hkDemoEntry* a = new hkDemoEntry;
		*a = thisEntry;
		a->m_menuPath += "/";
		a->m_menuPath += fileName;

		entriesOut.pushBack(a);
	}
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
