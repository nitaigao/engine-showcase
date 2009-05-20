/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef DEMOS_UTILITIES_MENUDEMO_H
#define DEMOS_UTILITIES_MENUDEMO_H

#include <Demos/demos.h>
#include <Demos/DemoCommon/DemoFramework/hkDemoDatabase.h>

#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Common/Base/Monitor/MonitorStreamAnalyzer/hkMonitorStreamAnalyzer.h>
#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Demos/DemoCommon/Utilities/Replay/DemoReplay.h>

#include <Common/Base/Memory/Memory/Debug/hkDebugMemory.h>
#include <Common/Base/Memory/hkDebugMemorySnapshot.h>

class Tweaker;

// Demo framework menu
class MenuDemo : public hkDefaultDemo
{
	public:

		MenuDemo(hkDemoEnvironment* env);
		~MenuDemo();

		virtual Result stepDemo();

		void preRenderDisplayWorld(class hkgViewport* v);
		void postRenderDisplayWorld(class hkgViewport* v);

		virtual Result stepVisualDebugger();

		virtual void windowResize(int w, int h);

		struct Empty
		{
			HK_DECLARE_REFLECTION();
		};

		// whether or not help is being displayed
		bool isHelpOn();

		// stop displaying the help text
		void turnOffHelp();

		// whether the monitors are turned on
		bool areMonitorsOn();

		// get a menu demo (kinda like a singleton)
		static MenuDemo* getMenuDemo();

	private:

		struct Entry
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, Entry);
			hkString	m_name;
			hkString	m_fullPath;
			const char*	m_help;
			int			m_index;

			Entry(const hkString& name, const hkString fullPath, const char* help, int index)
			: m_name(name), m_fullPath(fullPath), m_help(help), m_index(index) {}
			hkBool isDir() { return m_help == HK_NULL; }
		};

		Result stepMenuDemo();
		Result stepCurrentDemo();

		void stopCurrentDemo();
		void startCurrentDemo();

		void writeSimpleMemoryStatistics();

		void addStatus(hkString s);
		hkString getStatus();

			// show and handle the paused menu
		Result showPausedMenu( bool handleKeysOnly );

		Result showTweakMenu();

		void showMonitorAnalysis(int xOffset, hkArray<hkTimerData>& threadStreamInfos, hkArray<hkTimerData>& spuStreamInfos );
		void showMonitorGraph( hkArray<hkTimerData>& threadStreamInfos, hkArray<hkTimerData>& spuStreamInfos );

	private:

		const char* m_defaultDemo;	// set if there is a demo name we should launch by default
									// else we will show the menu demo

		hkPerformanceCounterUtility* m_performanceCounterUtility;

		hkString m_currentPath;		// full path of current demo
		hkDemo* m_currentDemo;		// current demo if it exists

		hkReal m_helpTimeMax;		// how long do we want help for (in seconds)
		hkReal m_helpTimeLeft;		// number of seconds left to show help. zero means help is off.
		hkString m_statusLine;		// status text
		hkString m_searchString;	// quicksearch

		hkgTexture* m_menuImage;
		float		m_menuImageBackground[3];
		float		m_menuImageBackgroundDirection[3];

		// switches
		hkBool m_paused;
		hkBool m_wantMonitors;
		hkBool m_wantTweak;
		hkBool m_childDemoJustCreated;
		hkBool m_wantCameraData;

		hkDemoReplayUtility m_replay;

		// current node for monitors
		hkMonitorStreamAnalyzer::Node* m_activeNode;
		hkMonitorStreamAnalyzer::Node* m_statsTree;

		// These are added by the test suite
		hkArray<hkDemoEntryRegister*> m_extraDemos;
		hkArray<hkString*> m_extraNames;

		// which button are we tracking for picking  (-1 for none)
		int m_mousePadTrackedButton;
		bool m_mouseInMenuOverlay;
		int m_mouseStatZoomOriginX;
		int m_mouseStatZoomOriginY;
		bool m_viewportMouseEventBackup;

		// Current index of search.
		int m_searchIndex;

		// in tweak mode which entry are we modifiying?
		Tweaker* m_tweaker;

        hkDebugMemorySnapshot m_memorySnapshot;

		hkPadSpu<char*> m_savedStreamBegin;
		hkPadSpu<char*> m_savedStreamCurrent;
		hkPadSpu<char*> m_savedStreamEnd;
		hkPadSpu<char*> m_simTimersEnd;
		hkPadSpu<char*> m_pausedSimTimers;

		bool m_newTimersGathered;


		// a handy graphical display of the stats:
		class hkgStatGraph* m_statGraph;

		// an instance of the menu demo if there is one
		static MenuDemo* m_menuDemo;
};

#endif // DEMOS_UTILITIES_MENUDEMO_H

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
