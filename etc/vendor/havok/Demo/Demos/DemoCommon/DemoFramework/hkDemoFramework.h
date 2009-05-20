/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEMOFRAMEWORK_DEMOFRAMEWORK_H
#define HK_DEMOFRAMEWORK_DEMOFRAMEWORK_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultDemo.h>
#include <Demos/DemoCommon/DemoFramework/hkDemoDatabase.h>
#include <Demos/DemoCommon/DemoFramework/hkDemoConsole.h>

struct hkDemoFrameworkOptions
{
	hkDemoFrameworkOptions();
	~hkDemoFrameworkOptions();
	void parse(int argc, const char** argv, int firstArg=1);

	hkBool m_windowed;
	hkBool m_enableShadows;
	hkBool m_forceNoShadows; // will prevent demos from turning them on 
	int m_shadowMapRes;
	hkBool m_enableFsaa;
	hkBool m_forceKeyboardGamepad;
	hkInt32 m_width;
	hkInt32 m_height;
	hkInt32 m_xPos;
	hkInt32 m_yPos;

	hkBool m_repositionConsole;
	char m_attributes[50];

	hkBool m_invertFlyMode;
	int m_trackballMode; // 0== no trackball, 1== normal trackball, 2== max, 3 == maya

	hkBool m_debugger;

	hkBool m_industrial;

	int m_lockFps;

	int m_maxIterations;

	const char* m_defaultDemo;
	const char* m_windowTitle;

	const char* m_masterFile;
	const char* m_renderer;

    hkBool m_multiGpu;
    hkBool m_forceCpu;

	hkUint32 m_numSaveFrames;
	hkBool m_saveFrames;

	hkBool m_forceMT;
	int m_numThreads;	// the number of threads of this computer.  set by the -mt# option, default is 2
	int m_numSpus;
	hkBool m_useSpuThreads;
	hkBool m_renderParallelWithSimulation;

	const char* m_statsDir;
	const char* m_overrideFileDir;
	hkBool m_enableWiiFilewriter;

	hkInt32 m_statBarX;
	hkInt32 m_statBarY;
	hkUint32 m_statBarScreenFraction; // 5 == 5th etc, 1 == all of the screen
	hkUint32 m_statBarStartTime; // 17ms by default

	hkBool m_enablePerformanceCounter;
	hkUint32 m_perfomanceCounterOption;

	hkUint32 m_graphicsDevice; // '-1' == auto (default), '0' == primary, '1' == secondary, etc

	int m_argc;
	const char** m_argv;
	char m_argvBuffer[1024];

	hkBool m_rebootIOP;
	hkBool m_forceCompiledShaders;
	hkUint32 m_windowHint;

	hkBool m_runLastDemo;
	hkBool m_runNextDemo;

	hkBool m_anisotropicFiltering;

	hkUint32 m_reportingLevel;

	enum
	{
		REPLAY_NONE = 0,
		REPLAY_RECORD,
		REPLAY_PLAYBACK
	};

	int			m_replayType;
	const char*	m_inputFilename;
};

#endif //HK_DEMOFRAMEWORK_DEMOFRAMEWORK_H

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
