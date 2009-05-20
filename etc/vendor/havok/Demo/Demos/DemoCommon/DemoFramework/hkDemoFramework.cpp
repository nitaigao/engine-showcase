/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Demos/DemoCommon/DemoFramework/hkDemo.h>

#include <Demos/DemoCommon/DemoFramework/hkDemo.h>
#include <Graphics/Common/hkGraphics.h>

#include <Demos/DemoCommon/DemoFramework/hkDemoFramework.h>

#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Demos/DemoCommon/DemoFramework/hkPerformanceCounterUtility.h>
#include <Demos/DemoCommon/DemoFramework/hkDemoDatabase.h>
#include <Demos/DemoCommon/DemoFramework/hkDemoConsole.h>
#include <Demos/DemoCommon/DemoFramework/hkDefaultDemo.h>

#include <Graphics/Common/hkGraphics.h>
#include <Graphics/Common/Camera/hkgCamera.h>
#include <Graphics/Common/Texture/SkyBox/hkgSkyBox.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/DisplayWorld/hkgDisplayWorld.h>
#include <Graphics/Common/Light/hkgLightManager.h>

#include <Graphics/Bridge/System/hkgSystem.h>
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>

#include <Common/Visualize/hkVisualDebugger.h>
 
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/Memory/Memory/Debug/hkDebugMemory.h>
#include <Common/Base/Memory/hkDebugMemorySnapshot.h>
#include <Common/Base/Memory/Memory/Pool/hkPoolMemory.h>
#include <Common/Base/Memory/MemoryClasses/hkMemoryClassDefinitions.h>

#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Base/System/Io/Reader/hkStreamReader.h>
#include <Common/Base/System/Io/Writer/hkStreamWriter.h>
#include <Common/Base/System/Io/Reader/Buffered/hkBufferedStreamReader.h>
#include <Common/Base/System/Stopwatch/hkStopwatch.h>

#include <Common/Base/Memory/Memory/FreeList/hkFreeListMemory.h>
#include <Common/Base/Memory/Memory/FreeList/SystemMemoryBlockServer/hkSystemMemoryBlockServer.h>

#include <Demos/DemoCommon/DemoFramework/hkPseudoPad.h>
#include <Demos/DemoCommon/DemoFramework/hkPackedFileSystemSource.inl>
#include <Demos/DemoCommon/DemoFramework/hkDemoFilesystem.h>

#include <Common/Serialize/Version/hkVersionPatchManager.h>

#if defined (HK_PLATFORM_PS3_PPU)
//#define ENABLE_SN_TUNER
//#ifdef ENABLE_SN_TUNER
//#include <hkdemoframework/libsntuner.h>
//#endif
#endif



// Enable FORCE_STATS_GENERATION to run the stats bootstrapper with the renderer switched off.
// This works for all platforms but is for Wii mostly, as the renderer can't be disabled via 
// command args until file IO is supported.
// #define FORCE_STATS_GENERATION

// Utility class for persistent text display. Substitutes "demo" for "game" in the framework menus
// in the virtual function outputText.
class hkIndustrialTextDisplay : public hkTextDisplay
{
public:

	// Create a text display attached to window 'window'
	hkIndustrialTextDisplay( hkgWindow* window );

	// Request 'str' to be rendered at absolute screen position x,y.
	// Substitutes "demo" for "game" in str.
	virtual void outputText(const hkString& str, float x, float y, hkUint32 color = 0xffffffffU, int frames = 1 );
};

hkIndustrialTextDisplay::hkIndustrialTextDisplay(hkgWindow* window)
:	hkTextDisplay(window)
{
}

void hkIndustrialTextDisplay::outputText(const hkString& str, float x, float y, hkUint32 color, int frames )
{
	hkTextDisplay::outputText( str.replace("game", "demo"), x, y, color, frames );
}






class hkOverrideFileSystem : public hkFileSystem
{
public:

	hkOverrideFileSystem( hkFileSystem* factory, const char* override )
		: m_parent(factory), m_override(override)
	{
		m_parent->addReference();
	}
	~hkOverrideFileSystem()
	{
		m_parent->removeReference();
	}

	hkStreamReader* openReader(const char* name)
	{
		hkString override = m_override;
		const char* basename = hkString::strRchr(name, '/');
		basename = basename ? basename+1 : name;
		override += basename;
		hkStreamReader* reader = m_parent->openReader(override.cString());
		if( reader->isOk() == false )
		{
			delete reader;
			reader = m_parent->openReader(name);
		}
		else
		{
			printf("override: using %s instead of %s\n", override.cString(), name);
		}
		return reader;
	}

	hkStreamWriter* openWriter(const char* name)
	{
		return m_parent->openWriter(name);
	}

	hkFileSystem* m_parent;
	const char* m_override;
};

hkDemoFrameworkOptions::hkDemoFrameworkOptions()
:	m_windowed(true),
	m_enableShadows(false), // disable by default (even on PC as some lower Nvidia cards claim to suppot fully but don't quite)
	m_forceNoShadows(false), // allow demos to turn them on if they like, even if m_enableShadows if off generally.
	m_shadowMapRes(0), // 0 == use default platform size
	m_enableFsaa(true),
	m_forceKeyboardGamepad(false), // if true, on pc, even if gamepad found, use keyboard to fake one.
	m_width(640),
	m_height(480),
	m_xPos(20),
	m_yPos(20),
	m_repositionConsole(false),
	m_invertFlyMode(false),
	m_trackballMode(0),
	m_debugger(false),
	m_industrial(false),
	#if HK_PLATFORM_IS_CONSOLE
	m_lockFps(0), // consoles don't need framelock.
	#else
	m_lockFps(60),
	#endif
	m_maxIterations(-1),
	m_defaultDemo(HK_NULL),
	m_windowTitle("Havok"),
	m_masterFile(HK_NULL),
	m_renderer("d3d9s"), // DirectX9S now the default, and will fall back to fixed function d3d9 if no shader support.
	m_multiGpu(false),
	m_forceCpu(false),
	m_numSaveFrames(0),
	m_saveFrames(false),
	m_statsDir(HK_NULL),
	m_overrideFileDir(HK_NULL),
	m_enableWiiFilewriter(false),
	m_statBarX(0),
	m_statBarY(1),
	m_statBarScreenFraction(5),
	m_statBarStartTime(17),
	m_enablePerformanceCounter( false ),
	m_perfomanceCounterOption(hkPerformanceCounterUtility::IDCACHE_MISSES_PENALTY),
	m_graphicsDevice( unsigned(-1) ),
	m_argc(0),
	m_argv(HK_NULL),
	m_rebootIOP(true),
	m_forceCompiledShaders(false),
	m_windowHint(0),
	m_runLastDemo(false),
	m_runNextDemo(false),
	m_anisotropicFiltering(false),
	m_replayType(REPLAY_NONE),
	m_inputFilename(HK_NULL)
{
	hkHardwareInfo info;
	hkGetHardwareInfo(info);
	m_numThreads = info.m_numThreads;
	m_forceMT = m_numThreads > 1;

#if defined(HK_PLATFORM_PS3_PPU)
	// Only use one PPU thread on the PLAYSTATION(R)3
	m_numThreads = 1;
#endif
	m_numSpus = 5;
	m_useSpuThreads = false;
	m_attributes[0] = 0;
#ifdef HK_PS2
	m_rebootIOP	= true;
#endif
	m_renderParallelWithSimulation = false;
}

hkDemoFrameworkOptions::~hkDemoFrameworkOptions()
{
}

void tokenizeArgs(const char* src, int &argc, char** &argv);
void tokenizeArgs(const char* src, int &argc, char** &argv)
{
	char *bp = const_cast<char*>(src);
	while (*bp) {
		char *start = HK_NULL;

		// pass up leading spaces
		while (*bp == ' ')
			bp++;

		// if we get to the end of the string after reading spaces then quit
		if ( *bp == '\0' )
		{
			break;
		}

		// handle a quoted string
		if ( *bp == '\"' )
		{
			bp++;
			start = bp;

			while (*bp && *bp != '\"')
				bp++;

		}
		else
		{
			start = bp;

			while (*bp && *bp != ' ')
				bp++;
		}

		argv[argc++] = start;

		// only increment the pointer if we are not at the end of the string
		if (*bp)
			*bp++ = '\0';
	}
}

void hkDemoFrameworkOptions::parse(int argc, const char** argv, int firstArg)
{
	m_argc = argc;
	m_argv = argv;

	int i = 0; // 0 so that we get the first arg in hkdemo.cfg
	while( i < argc )
	{
		if(argv[i][0] == '-')
		{
			switch( hkString::toLower(argv[i][1]))
			{
			case '?': //print out the options available
				{
					printf("\nHavok demo framework command line options:\n");
					printf("\t\t -a            Assign a user attributes string to the demo\n");
					printf("\t\t -norepos      Disable reposition of console window on startup [PC, default]\n");
					printf("\t\t -repos        Reposition of console window on startup [PC]\n");
					printf("\t\t -d            Run the VDB server\n");
					printf("\t\t -g DemoName   Run the given demo only\n");
					printf("\t\t -lastdemo     When running bootstrap demos (e.g. -g bootstrapall), start from the last demo run by the bootstrap\n");
					printf("\t\t -nextdemo     When running bootstrap demos (e.g. -g bootstrapall), start from the demo after the last demo run by the bootstrap\n");
					printf("\t\t -r {ogl,d3d9,d3d9s,d3d8,null}  Attempt to use the given renderer. ");
					printf("\t\t               Must be last option if used. [mainly PC, null allowed on all]\n");
					printf("\t\t -trackball    Use trackball style navigation (default navigation is fly mode):\n");
					printf("\t\t               can be like Max (-trackballMax), Maya (-trackballMaya) or normal (-trackball).\n");
					printf("\t\t -invertfly    Use inverted fly mode (flight sim style, forward/up is pitch down).\n");
					printf("\t\t -f            Force fullscreen mode [PC]\n");
					printf("\t\t -w            Force windowed mode [PC]\n");
					printf("\t\t -shadows      Force shadows on (using shadow maps) in all applicable demos on \n");
					printf("\t\t -smapXXX      Set shadow map resolution to XXX \n");
					printf("\t\t -noshadows    Force shadows off\n");
					printf("\t\t -fsaa		   Force antialiasing on \n");
					printf("\t\t -nofsaa       Force antialiasing off \n");
					printf("\t\t -aniso        Turn anisotropic texture filtering on\n");
					printf("\t\t -sli          Use multiple GPUs if available\n");
					printf("\t\t -fakegamepad  Force keyboard input to override gamepad (fake gamepad)[PC]\n");
					printf("\t\t -record FILE   Record all input to FILE.\n");
					printf("\t\t -playback FILE Playback input from FILE\n");
					printf("\t\t -lDDD         Force demo not to exceed DDD Hz (lock framerate) using a software wait[PC]\n");
					printf("\t\t -x DDD         Set the window width(DDD pixels) [PC]\n");
					printf("\t\t -y DDD         Set the window height(DDD pixels) [PC]\n");
					printf("\t\t -xp DDD        Set the window x position (DDD pixels of the virtual screen) [PC]\n");
					printf("\t\t -yp DDD        Set the window y position (DDD pixels of the virtual screen) [PC]\n");
					printf("\t\t -i (num)      Run the demo for (num) iterations.\n");
					printf("\t\t -b            Don't reboot the IOP [PS2]\n");
					printf("\t\t -m Filename   Set the master resource file for demos [PS2]\n");
					printf("\t\t -mt           force the demo(s) to run in multi-threaded simulation mode\n");
					printf("\t\t -mtr          force the demo(s) to run in multi-threaded simulation mode, in parallel with rendering\n");
					printf("\t\t -spu(num)     Set the number of SPUs used to (num). -mt must be set for SPUs to be used. \n");
					printf("\t\t -sputhread	   Use spuThreads instead of SPURS. \n");
					printf("\t\t -c	           Run using the debug mem manager [Debug only, not in GCC or GameCube builds, not in hkdemo.cfg]\n");
					printf("\t\t -p{i,d,l,f}   Enable performance counter and set it to count instruction cache(i), d cache(d), load-hit-store(l), and instr flushed (f), or combo of them.\n");
					printf("\t\t               or dcache (pd) misses [PS2]\n");
					printf("\t\t -h{VGA,480p,480i,576p,576i,720p,720i,1080p,1080i}  Set the display hint \n");
					printf("\t\t -device{0,1,2,3} Set the display device (adapter) to use on multi monitor PCs \n");
					printf("\t\t -rl(0,1,2,3,4,5) Set the reporting level 0 = NONE, 5 = ALL");
					printf("\t\t -video        Dump all displayed frames to disk\n");
					printf("\t\t -wiifw        Enable filewriter (cannot use VDB if enabled) [Wii]\n");
					break;
				}

			case 'a':
				{
					if (hkString::strCasecmp(argv[i], "-aniso") == 0)
					{
						m_anisotropicFiltering = true;
					}
					else
					{
						hkString::snprintf(m_attributes, 50, &argv[i][2]);
					}
					break;
				}

			case 'b': // dont re-boot iop
				{
					// Note: the behavior demos use "-b" as a prefix to all commandline options (see hkDefaultBehaviorDemo),
					// so we carefully parse all options with a "-b" prefix.
					if ( argv[i][2] == '\0' )
					{
						m_rebootIOP = false;
					}
					// note that the following expression will not read off the end of the string
					// (although it looks suspicious) due to short-circuit expression evaluation
					else if (	(argv[i][2] == 'i') &&
						(argv[i][3] == 'n') &&
						(argv[i][4] == '\0' ) ) // -bin
					{
						m_forceCompiledShaders = true;
					}
					break;
				}
				// case 'c': check memory - handled before hkBaseSystem::init.
			case 'd': // debugger
				{

					// -device{0,1,choose} Set the display device (adapter) to use on mutlimon PCS \n");
					if ( argv[i][2] && argv[i][3] && (argv[i][2] == 'e') && (argv[i][3] == 'v') )
					{
						if (argv[i][5] && argv[i][6] && argv[i][7])
						{
							if (argv[i][7] == '0')
								m_graphicsDevice = 0;
							else if (argv[i][7] == '1')
								m_graphicsDevice = 1;
							else if (argv[i][7] == '2')
								m_graphicsDevice = 2;
							else if (argv[i][7] == '3')
								m_graphicsDevice = 3;
						}
					}
					// -d(ebugger)
					else
					{
						m_debugger = true;
					}
					break;
				}
			case 'f': // fullscreen
				{
					// -fakegamepad
					if ( argv[i][2] && (argv[i][2] == 'a') )
					{
						m_forceKeyboardGamepad = true;
					}
					// -forcecpu
					else if ( argv[i][2] && (argv[i][2] == 'o') )
					{
						m_forceCpu = true;
					}
					else if ( argv[i][2] && (argv[i][2] == 's') ) // fsaa
					{
						m_enableFsaa = true;
					}
					else // assume -f or -fullscreen etc
					{
						m_windowed = false;
					}
					break;
				}
			case 'g':
				{
					if(++i < argc)
					{
						m_defaultDemo = argv[i];
					}
					break;
				}
			case 'i': // Iterations
				{
					if (argv[i][2] && (hkString::toLower(argv[i][2]) == 'n')) // -invert
					{
						m_invertFlyMode = true;
					}
					else if(++i < argc)
					{
						m_maxIterations = hkString::atoi(argv[i]);
					}
					break;
				}
			case 'l':
				{
					// -lastdemo
					if ( argv[i][2] && (argv[i][2] == 'a') )
					{
						m_runLastDemo = true;
					}
					else
					{
						m_lockFps = -1;
						if( argv[i][2] != HK_NULL) // '-l60'
						{
							m_lockFps = hkString::atoi(argv[i]+2);
						}
						else if( i+1 < argc && argv[i+1][0] != '-') // '-l' '60'
						{
							++i;
							m_lockFps = hkString::atoi(argv[i]);
						}
						if( m_lockFps == -1) // '-l' or parse error
						{
							m_lockFps = 60;
						}
					}
					break;
				}
			case 'm':
				{
					// -mt (force multithreaded)
					if ( argv[i][2] == 't' )
					{
						m_forceMT = true;
						if ( argv[i][3] >= '1' && argv[i][3] <= '9')
						{
							m_numThreads = argv[i][3] - '0';
						}
						// -mtr (run rendering in parallel with simulation)
						if ( argv[i][3] == 'r' )
						{
							m_renderParallelWithSimulation = true;
						}
					}
					else // assume -m
					{
						if(++i < argc)
						{
							m_masterFile = argv[i];
						}
					}
					break;
				}
			case 'n':
				{
					if ( argv[i][2] && (argv[i][2] == 'e') ) // -nextdemo
					{
						m_runNextDemo = true;
					}
					else if ( argv[i][2] && (argv[i][2] == 'o') && argv[i][3] && (argv[i][3] == 'r')) // -norepos
					{
						m_repositionConsole = false;
					}
					else if ( argv[i][2] && argv[i][3] && (argv[i][3] == 'f') ) // -nofsaa
					{
						m_enableFsaa = false;
					}
					else if ( argv[i][2] && argv[i][3] && (argv[i][3] == 's') )// -noshadows
					{
						m_enableShadows = false;
						m_forceNoShadows = true;
					}
					break;
				}
			case 'o':
				{
					if(++i < argc)
					{
						m_overrideFileDir = argv[i];
					}
					break;
				}


			case 'p': // p{i|d|l|lp|f}
				{
					if (hkString::strCasecmp(argv[i], "-playback") == 0)
					{
						if (++i < argc)
						{
							m_replayType = REPLAY_PLAYBACK;
							m_inputFilename = argv[i];
						}
					}
					else
					{
						m_enablePerformanceCounter = true;
						if (argv[i][2] && (argv[i][2] != ' ') )
						{
							int idx = 1;
							m_perfomanceCounterOption = hkPerformanceCounterUtility::DISABLE; //0
							while (++idx >= 2)
							{
								switch (argv[i][idx])
								{
								case 'i' :
									m_perfomanceCounterOption |= hkPerformanceCounterUtility::ICACHE_MISSES_PENALTY;
									break;
								case 'd' :
									m_perfomanceCounterOption |= hkPerformanceCounterUtility::DCACHE_MISSES_PENALTY;
									break;
								case 'l' :
									if (argv[i][idx+1] =='p')
									{
										m_perfomanceCounterOption |= hkPerformanceCounterUtility::LOAD_HIT_STORE_PENALTY;
										++idx;
									}
									else
										m_perfomanceCounterOption |= hkPerformanceCounterUtility::LOAD_HIT_STORE;
									break;
								case 'f' :
									m_perfomanceCounterOption |= hkPerformanceCounterUtility::INSTRUCTIONS_FLUSHED;
									break;
								default:
									idx = 0; break;
								}
							}
						}
					}
					break;
				}
			case 'r':
				{
					if (hkString::strCasecmp(argv[i], "-repos") == 0) //-repos windows
					{
						m_repositionConsole = true;
					}
					else if (hkString::strNcasecmp(argv[i], "-rl", 3) == 0 && argv[i][3] ) //-rl# reporting level
					{
						m_reportingLevel = argv[i][3] - '0';
						m_reportingLevel = hkMath::clamp( (int)m_reportingLevel, (int)hkDemoEnvironment::REPORT_OFF, (int)hkDemoEnvironment::REPORT_ALL );
					}
					else if (hkString::strCasecmp(argv[i], "-record") == 0)
					{
						if (++i < argc)
						{
							m_replayType = REPLAY_RECORD;
							m_inputFilename = argv[i];
						}
					}
					else if(++i < argc) //// renderer
					{
						m_renderer = argv[i];
						if( argv[i][0] == 'n' )
						{
							m_lockFps = 0; // turn off frame lock for null renderer
						}
					}					
					break;
				}
			case 's': // set the stats output directory or shadows
				{
					// -st singlethreaded
					if ( argv[i][2] == 't' )
					{
						if (argv[i][3] == 'a') // -statbars[tart], -statbarx, -statbary, -statbarf[raction] all followed by a space then the int
						{
							switch (argv[i][8])
							{
							case 's':
								if(++i < argc)
								{
									m_statBarStartTime =  hkString::atoi(argv[i]);
									if (m_statBarStartTime < 1 || m_statBarStartTime > 255)
									{
										m_statBarStartTime = 17;
									}
								}
								break;

							case'x':
								if(++i < argc)
								{
									m_statBarX =  hkString::atoi(argv[i]);
									if (m_statBarX < 0 || m_statBarStartTime > 10000)
									{
										m_statBarX = 0;
									}
								}
								break;

							case 'y':
								if(++i < argc)
								{
									m_statBarY =  hkString::atoi(argv[i]);
									if (m_statBarY < 0 || m_statBarY > 10000)
									{
										m_statBarY = 0;
									}
								}
								break;

							case 'f':
								if(++i < argc)
								{
									m_statBarScreenFraction =  hkString::atoi(argv[i]);
									if (m_statBarScreenFraction < 1 || m_statBarScreenFraction > 100)
									{
										m_statBarScreenFraction = 5;
									}
								}
								break;
							}
						}
						else
						{
							// -st
							m_forceMT = false;
							m_numThreads = 1;
						}
						break;
					}

					// -spu (set num spus, spuThread)
					if ( ( argv[i][2] == 'p' ) && ( argv[i][3] == 'u' ) )
					{
						if ( argv[i][4] == 't' )
						{
							m_useSpuThreads = true;
						}
						else
						{
							if ( argv[i][4] >= '1' && argv[i][4] <=	 '6')
							{
								m_numSpus = argv[i][4] - '0';
							}
						}
						break;
					}
					// shadows
					if (argv[i][2] && (hkString::toLower(argv[i][2]) == 'h'))
					{
						m_enableShadows = true;
						m_forceNoShadows = false;
					}
					// -smapRES
					else if ( argv[i][2] && (hkString::toLower(argv[i][2]) == 'm') && argv[i][3] && argv[i][4] && argv[i][5] )
					{
						m_shadowMapRes =  hkString::atoi(argv[i]+5);
					}
					// sli
					else if (argv[i][2] && (hkString::toLower(argv[i][2]) == 'l'))
					{
						m_multiGpu = true;
					}
					// else stats
					else if(++i < argc)
					{
						m_statsDir = argv[i];
					}
					break;
				}
			case 't':
				{
					if ( argv[i][2] != HK_NULL) // '-trackball'
					{
						const char* name = &(argv[i][1]);
						if ( hkString::strNcasecmp(name, "trackball", 9) == 0)
						{
							m_trackballMode = 1;
							const char* style = &(argv[i][10]);
							if ( style[0] )
							{
								if (hkString::strNcasecmp(style, "Maya", 4) == 0)
									m_trackballMode = 3;
								else if (hkString::strNcasecmp(style, "Max", 3) == 0)
									m_trackballMode = 2;
							}
						}
					}
					else if(++i < argc) // -t windowname
					{
						m_windowTitle = argv[i];
					}
					break;
				}
			case 'v':  // -video
				{
					m_saveFrames = true;
					break;
				}
			case 'w': 
				{
					if ( argv[i][2] == 'i' ) // '-wiifw'
					{
						m_enableWiiFilewriter = true;
					}
					else
					{
						m_windowed = true; // // windowed
					}	
					break;
				}
			case 'x': // x res (width)
				{
					if (argv[i][2] == 'p') // '-xp640'
					{
						m_xPos =  hkString::atoi(argv[i]+3);
					}
					else if( argv[i][2] != HK_NULL) // '-x640'
					{
						m_width = hkString::atoi(argv[i]+2);
					}
					break;
				}
			case 'y': // y res (height)
				{
					if (argv[i][2] == 'p') // '-xp640'
					{
						m_yPos =  hkString::atoi(argv[i]+3);
					}
					else if( argv[i][2] != HK_NULL) // '-y480'
					{
						m_height = hkString::atoi(argv[i]+2);
					}
					break;
				}
			case 'h':
				{
					// -h{VGA,480p,480i,576p,576i,720p,720i,1080p,1080i}
					if( argv[i][2] != HK_NULL)
					{
						m_windowHint |= (hkString::strCasecmp(&argv[i][2], "VGA")==0)   ? HKG_WINDOW_HINT_VGA : 0;
						m_windowHint |= (hkString::strCasecmp(&argv[i][2], "480p")==0)  ? HKG_WINDOW_HINT_480p : 0;
						m_windowHint |= (hkString::strCasecmp(&argv[i][2], "480i")==0)  ? HKG_WINDOW_HINT_480i : 0;
						m_windowHint |= (hkString::strCasecmp(&argv[i][2], "576p")==0)  ? HKG_WINDOW_HINT_576p : 0;
						m_windowHint |= (hkString::strCasecmp(&argv[i][2], "576i")==0)  ? HKG_WINDOW_HINT_576i : 0;
						m_windowHint |= (hkString::strCasecmp(&argv[i][2], "720p")==0)  ? HKG_WINDOW_HINT_720p : 0;
						m_windowHint |= (hkString::strCasecmp(&argv[i][2], "720i")==0)  ? HKG_WINDOW_HINT_720i : 0;
						m_windowHint |= (hkString::strCasecmp(&argv[i][2], "1080p")==0) ? HKG_WINDOW_HINT_1080p : 0;
						m_windowHint |= (hkString::strCasecmp(&argv[i][2], "1080i")==0) ? HKG_WINDOW_HINT_1080i : 0;
					}
					break;
				}

			default:
				{
					//hkprintf("option `%s' was not understood", argv[i]);
				}
			}
		}
		else
		{
			//hkprintf("option `%s' was not understood", argv[i]);
		}

		++i;
	}
#if defined FORCE_STATS_GENERATION
	{
		m_renderer = "n";
		m_lockFps = 0;
		m_defaultDemo = "DemoCommon/Utilities/Bootstrap/BootstrapStats";
	}
#endif
}

static void HK_CALL _demoResizeNotify(hkgWindow* w,unsigned int width, unsigned int height, void* userContext )
{
	if (userContext)
	{
		((hkDemo*)userContext)->windowResize(width, height);
	}
}

static void render(hkgWindow* window, const hkDemoEnvironment& env, hkDemo* demo )
{
	HKG_TIMER_BEGIN_LIST("_Render", "render");

	if (!window || (window->getWidth() == 0) || (window->getHeight() == 0) )
	{
		HKG_TIMER_BEGIN("SwapBuffers", HK_NULL);
		if (window)
		{
			window->getContext()->lock();
				window->swapBuffers();
			window->getContext()->unlock();
		}
		HKG_TIMER_END();
		HKG_TIMER_END_LIST(); // render
		return; // nothing to render too..
	}

	hkgDisplayWorld* dw = env.m_displayWorld;
	hkgDisplayContext* ctx = window->getContext();

	ctx->lock();

	// only reason there won't be a displayWorld is if the demo is doing the rendering itself.
	{
		for( int viewport = 0; viewport < window->getNumViewports(); ++viewport )
		{
			HKG_TIMER_SPLIT_LIST("SetViewport");

			hkgViewport* v = window->getViewport(viewport);

			// send the user camera
			{
				hkgCamera* c = v->getCamera();

				float floatFrom[3]; c->getFrom(floatFrom);
				float floatTo[3]; c->getTo(floatTo);
				float floatUp[3]; c->getUp(floatUp);

				hkVector4 from; from.set( floatFrom[0], floatFrom[1], floatFrom[2] );
				hkVector4 to;   to.set( floatTo[0], floatTo[1], floatTo[2] );
				hkVector4 up;   up.set( floatUp[0], floatUp[1], floatUp[2] );

				HK_UPDATE_CAMERA(from, to, up, c->getNear(), c->getFar(), c->getFOV(), "Demo Framework");
			}

			v->setAsCurrent( ctx );

			if (v->getSkyBox())
			{
				HKG_TIMER_SPLIT_LIST("SkyBox");
				v->getSkyBox()->render( ctx, v->getCamera() );
			}


			HKG_TIMER_SPLIT_LIST("DemoPreRenderWorld");
			if (demo)
			{
				demo->preRenderDisplayWorld( v );
			}

			HKG_TIMER_SPLIT_LIST("DisplayWorld");
			if (dw)
			{
				// can't alter the world in the middle of a render pass, so it will lock itself
				dw->render( window->getContext(), true, env.m_options->m_enableShadows ); // culled with shadows (if any setup)
			}

			HKG_TIMER_SPLIT_LIST("DemoPostRenderWorld");
			if (demo)
			{
				demo->postRenderDisplayWorld( v );
			}


			HKG_TIMER_SPLIT_LIST("DrawImmediate");
			env.m_displayHandler->drawImmediate();
		}
	}

	HKG_TIMER_SPLIT_LIST("PostEffects");
	window->applyPostEffects();
	HKG_TIMER_SPLIT_LIST("DisplayText");
	env.m_textDisplay->displayText(window);
	HKG_TIMER_SPLIT_LIST("SwapBuffers");
	window->swapBuffers();

	if (env.m_options->m_saveFrames)
	{
		char filename[128];
		hkString::sprintf(filename,"frame%05i.bmp",env.m_options->m_numSaveFrames);
		window->saveFrontFrameBufferToBMP(filename);
		env.m_options->m_numSaveFrames++;
	}

	ctx->unlock();

	HKG_TIMER_END_LIST(); // render
}

static void HK_CALL errorReportFunction(const char* str, void*)
{
	printf("%s", str);
}

static hkMemory* newMemoryManager(int argc, const char** argv, hkMemoryBlockServer*& server)
{
	server = HK_NULL;

	// Debug memory option instead if you want:
	for( int i = 0; i < argc; ++i )
	{
		if( argv[i] && argv[i][0] == '-' && argv[i][1] == 'c')
		{
			hkDebugMemory* debugMem = hkDebugMemory::create();
			if ( debugMem )
			{
				return debugMem;
			}
		}
	}


	// Minimum block allocation size is 1Mb
	server = new hkSystemMemoryBlockServer(1*1024*1024);
	return new hkFreeListMemory(server);
	//return new hkPoolMemory();
}

//void disableSecondaryDisplays();
//void reEnableSecondaryDisplays();

HWND GetConsoleHwnd(void)
{
#define MY_BUFSIZE 1024 // Buffer size for console window titles.
	HWND hwndFound;         // This is what is returned to the caller.
	char pszNewWindowTitle[MY_BUFSIZE]; // Contains fabricated
	// WindowTitle.
	char pszOldWindowTitle[MY_BUFSIZE]; // Contains original
	// WindowTitle.
	// Fetch current window title.
	GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

	// Format a "unique" NewWindowTitle.
	wsprintf(pszNewWindowTitle,"%d/%d",
		GetTickCount(),
		GetCurrentProcessId());

	// Change current window title.
	SetConsoleTitle(pszNewWindowTitle);

	// Ensure window title has been updated.
	Sleep(40);

	// Look for NewWindowTitle.
	hwndFound=FindWindow(NULL, pszNewWindowTitle);

	// Restore original window title.
	SetConsoleTitle(pszOldWindowTitle);

	return(hwndFound);
}


static hkgWindow* s_debugWindowHandle = HK_NULL;
static hkDemoEnvironment* s_debugEnvironmentHandle = HK_NULL;

// A utility function useful for debugging inner loops
int HK_CALL debugRenderNow()
{
	int returnCode=0;

	HK_ASSERT(0, s_debugWindowHandle != HK_NULL);
	HK_ASSERT(0, s_debugEnvironmentHandle != HK_NULL);

	s_debugEnvironmentHandle->m_textDisplay->m_holdTextForDebug = true;
	s_debugEnvironmentHandle->m_textDisplay->outputText("Debug display", 20, 0);
	while ( s_debugWindowHandle->peekMessages() == HKG_WINDOW_MSG_CONTINUE )
	{
		render(s_debugWindowHandle, *s_debugEnvironmentHandle, HK_NULL );
		s_debugWindowHandle->clearBuffers();
		const hkgKeyboard& key = s_debugEnvironmentHandle->m_window->getKeyboard();
		if( key.wasKeyPressed(HKG_VKEY_SPACE) || key.getKeyState(HKG_VKEY_SHIFT) )
		{
			returnCode = 1;
			break;
		}
	}

	s_debugEnvironmentHandle->m_textDisplay->m_holdTextForDebug = false;
	render(s_debugWindowHandle, *s_debugEnvironmentHandle, HK_NULL );

	s_debugWindowHandle->clearBuffers();
	s_debugEnvironmentHandle->m_displayHandler->clear(); // clear debug display before a potential world step

	return returnCode;
}

// A utility function useful for debugging inner loops
void HK_CALL debugRenderNowNoWait()
{
	HK_ASSERT(0, s_debugWindowHandle != HK_NULL);
	HK_ASSERT(0, s_debugEnvironmentHandle != HK_NULL);

	s_debugEnvironmentHandle->m_textDisplay->m_holdTextForDebug = true;
	s_debugEnvironmentHandle->m_textDisplay->outputText("Debug display", 20, 0);

	render(s_debugWindowHandle, *s_debugEnvironmentHandle, HK_NULL );
	s_debugWindowHandle->clearBuffers();

	s_debugEnvironmentHandle->m_textDisplay->m_holdTextForDebug = false;
	render(s_debugWindowHandle, *s_debugEnvironmentHandle, HK_NULL );

	s_debugWindowHandle->clearBuffers();
	s_debugEnvironmentHandle->m_displayHandler->clear(); // clear debug display before a potential world step
}

static int HK_CALL getWindowDeviceFlag( int d )
{
	switch (d)
	{
	case -1:
		return 0;
	case 0:
		return HKG_WINDOW_PRIMARY_DEVICE;
	case 1:
		return HKG_WINDOW_SECONDARY_DEVICE;
	case 2:
		return HKG_WINDOW_TERTIARY_DEVICE;
	case 3:
		return HKG_WINDOW_QUATERNARY_DEVICE;
	default:
		break;
	}
	return 0;
}

static const char* new_argv[64];


extern void HK_CALL registerPatches(hkVersionPatchManager&);

int HK_CALL hkFrameworkMain (hkDemoFrameworkOptions& options, char* startUpDemo);
int HK_CALL hkFrameworkMain (hkDemoFrameworkOptions& options, char* startUpDemo)
{
	// We start initializing hkBaseSystem
	hkMemoryBlockServer* server = HK_NULL;
	hkMemory* memoryManager = newMemoryManager(options.m_argc, options.m_argv, server);

	hkThreadMemory* threadMemory = new hkThreadMemory(memoryManager);

	hkBaseSystem::init( memoryManager, threadMemory, errorReportFunction );
	memoryManager->removeReference();

	// We now initialize the stack area to 2 mega bytes (fast temporary memory to be used by the engine).
	char* stackBuffer;
	{
		int stackSize = 2*1024*1024; // 2MB stack


		stackBuffer = hkAllocate<char>( stackSize, HK_MEMORY_CLASS_BASE);


		threadMemory->setStackArea( stackBuffer, stackSize);
	}

	registerPatches(hkVersionPatchManager::getInstance());
 
	// Create a 2 Megabyte buffer for collecting statistics

	{
		if(options.m_masterFile != HK_NULL)
		{
			hkStreamReader * masterFile = hkFileSystem::getInstance().openReader(options.m_masterFile);
			if (masterFile->isOk())
			{
				// For PlayStation(R)2 cdrom
				hkBool seekBug = (hkString::strNcasecmp("cdrom" , options.m_masterFile, 5) == 0);
				// Replace with the masterfile
				hkPackedFileSystem* msb = new hkPackedFileSystem(masterFile, seekBug);
				masterFile->removeReference();
				hkFileSystem::replaceInstance(msb);
			}
			else
			{
				HK_WARN( 0x2c23ee65, "Could not open master file " << options.m_masterFile );
				options.m_masterFile = HK_NULL;
			}
		}
		if( options.m_masterFile == HK_NULL ) // master not specified or not found
		{
			hkDemoFileSystem* dsb = new hkDemoFileSystem();
			hkFileSystem::replaceInstance(dsb);
			if( options.m_overrideFileDir != HK_NULL )
			{
				hkFileSystem* osb = new hkOverrideFileSystem(dsb, options.m_overrideFileDir);
				hkFileSystem::replaceInstance(osb);
			}
		}

		// open the cmdline settings file
		hkIstream stream("hkdemo.cfg");

		// If we have a command line file
		if (stream.isOk())
		{
			// copy argv so we can expand it
			for( int i = 0; i < options.m_argc; i++ )
			{
				new_argv[i] = options.m_argv[i];
			}
			options.m_argv = new_argv;
			int oldArgc = options.m_argc;

			// read the settings file
			// comment lines start with ;
			// can have arguments spread over multiple lines
			// or all arguments can be on same line

			int curLen = 0;
			while( stream.isOk() )
			{
				HK_ASSERT2(0x1d4eaa89, curLen < 1024, "hkdemo.cfg command line arguments has exceeded maximum number of allowed chars (1024)");

				// curGlobalLine holds a pointer to the next free position in the m_argvBuffer
				char* curGlobalLine =  options.m_argvBuffer + curLen;

				stream.getline( curGlobalLine, 255 );

				// comment lines are ignored
				if( curGlobalLine[0] == ';' )
				{
					continue;
				}

				hkString curLine( curGlobalLine );

				if (curLine.getLength() > 0)
				{
					// trim whitespace at end
					char trim = curLine[curLine.getLength() - 1];
					while( (curLine.getLength() > 0) && ((' ' == trim) || ('\r' == trim) || ('\n' == trim)) )
					{
						curLine = curLine.substr( 0, curLine.getLength() - 1 );
						trim = curLine[curLine.getLength() - 1];
					}
				}

				int cr = curLine.getLength();

				// ignore empty lines
				if( 0 == cr )
				{
					continue;
				}

				// hold onto the lines with cmd line args
				curGlobalLine[cr] = 0;

				// tokenize the line using ' ', increment argc and add each token to argv
				tokenizeArgs(curGlobalLine, options.m_argc, (char**&)options.m_argv);

				curLen += cr + 1;
			}

			// set the last argument to null
			options.m_argv[options.m_argc] = HK_NULL;

			// parse new options (but not the ones that were already parsed in main())
			options.parse(options.m_argc, options.m_argv, oldArgc);
		}
	}

	//   if (options.m_multiGpu)
	//   {
	//       disableSecondaryDisplays();
	//   }

	int exitCode = 0;

	// Now initialize the graphics, using hkgSystem
	hkgSystem::init(options.m_renderer);
	if (hkgSystem::g_RendererType != hkgSystem::HKG_RENDERER_CONSOLE)
	{
#ifdef HK_DEBUG
			printf("HKG: Renderer using %s.\n", hkgSystem::getRendererString() );							
#endif
	}

	{
		RECT visible;
		::SystemParametersInfo(SPI_GETWORKAREA, 0, &visible, 0);
		if (options.m_windowed && options.m_repositionConsole)
		{
			int consoleTop = visible.top + options.m_yPos + options.m_height + 10;
			SetWindowPos( GetConsoleHwnd(), NULL,
				visible.left + options.m_xPos, consoleTop,
				options.m_width, visible.bottom - consoleTop, 0 );
		}
		else if (!options.m_windowed)
		{
			ShowWindow(GetConsoleHwnd(), SW_HIDE);
		}

		hkgWindow* window = hkgWindow::create();
		s_debugWindowHandle = window;
		window->setShadowMapSize( options.m_shadowMapRes );
		window->initialize( 
			getWindowDeviceFlag( options.m_graphicsDevice ) | 
			(options.m_enableFsaa ? HKG_WINDOW_FSAA : 0) | 
			(options.m_windowed ? HKG_WINDOW_WINDOWED : HKG_WINDOW_FULLSCREEN) | 
			options.m_windowHint,
			HKG_WINDOW_BUF_COLOR | HKG_WINDOW_BUF_DEPTH32, options.m_width, options.m_height,
			options.m_windowTitle);

		// check that render is actually ok (shader drivern and platform has shaders for instance)
		if ((hkgSystem::g_RendererType == hkgSystem::HKG_RENDERER_DX9S) || (hkgSystem::g_RendererType == hkgSystem::HKG_RENDERER_OGLS))
		{
			if (!window->shaderSupportGreaterOrEqualTo(2))
			{
				printf("HKG: Changing to fixed function renderer as current hardware does not support shader model 2 or higher.\n");
				
				window->release();
			
				window = hkgWindow::create();
				s_debugWindowHandle = window;
				window->setShadowMapSize( options.m_shadowMapRes );
				window->initialize( 
					getWindowDeviceFlag( options.m_graphicsDevice ) | 
					(options.m_enableFsaa ? HKG_WINDOW_FSAA : 0) | 
					(options.m_windowed ? HKG_WINDOW_WINDOWED : HKG_WINDOW_FULLSCREEN) | 
					options.m_windowHint,
					HKG_WINDOW_BUF_COLOR | HKG_WINDOW_BUF_DEPTH32, options.m_width, options.m_height,
					options.m_windowTitle);
			}
			else if (window->shaderSupportGreaterOrEqualTo(3) && (hkgSystem::g_RendererType == hkgSystem::HKG_RENDERER_DX9S) )
			{
				// then dealling with a good graphics card, so we can assume shadows etc unlesss told otherwise
				if (!options.m_forceNoShadows)
				{
					options.m_enableShadows = true;
				}
			}
		}

		window->getContext()->lock();

		if (options.m_windowed && options.m_repositionConsole && (options.m_xPos != -1) && (options.m_yPos != -1))
		{
			SetWindowPos( (HWND)(window->getPlatformHandle()), NULL,
				visible.left + options.m_xPos, visible.top + options.m_yPos,
				options.m_width, options.m_height, 0);
		}
		//SendMessage( (HWND)(window->getPlatformHandle()), 0x0112/*WM_SYSCOMMAND*/, 0xF030/*SC_MAXIMIZE*/, 0);

		// don't allow viewport resizing
		window->setWantViewportBorders(false);
		window->setWantViewportResizeByMouse(false);


		// setup navigation style
		if (options.m_invertFlyMode)
			window->getViewport(0)->setInvertLook(true);

		if (options.m_trackballMode > 0)
		{
			window->getViewport(0)->setNavigationMode(HKG_CAMERA_NAV_TRACKBALL);
			if (options.m_trackballMode == 2) // Max
				window->getViewport(0)->setMouseConvention(HKG_MC_3DSMAX);
			else if (options.m_trackballMode == 3) // Maya
				window->getViewport(0)->setMouseConvention(HKG_MC_MAYA);
		}

		for(int i = 0; i < 2; ++i)
		{
			window->clearBuffers();
			window->swapBuffers();
		}

		// Demos that use two pads will attempt to use the CTRL key for switching between pads. 
		// Here we attempt to "free" the CTRL key for other uses if not needed for pads.
		hkPseudoPad* pad0 = new hkPseudoPad ();
		hkPseudoPad* pad1 = new hkPseudoPad ();

		if (window->hasGamePads())
		{
			printf("HKG: Found a gamepad, so unless you have specified otherwise (-fakegamepad), only that input will be used.\n");
		}


		hkDemoEnvironment environment;
		s_debugEnvironmentHandle = &environment;
		environment.m_window = window;
		environment.m_displayWorld = hkgDisplayWorld::create();
		environment.m_sceneConverter = new hkgSceneDataConverter( environment.m_displayWorld, window->getContext() );
		environment.m_reportingLevel = (hkDemoEnvironment::ReportingLevel) options.m_reportingLevel;

		if (options.m_industrial)
		{
			// Use Industrial logo (or none at all for now).
			environment.m_window->setWantDrawHavokLogo(false);
			// Substitute "demo" for "game" in the framework menus.
			environment.m_textDisplay = new hkIndustrialTextDisplay(window);
		}
		else
		{
			environment.m_textDisplay = new hkTextDisplay(window);
		}
		environment.m_gamePad = pad0;
		environment.m_gamePadB = pad1;
		environment.m_displayHandler = new hkgDisplayHandler(environment.m_displayWorld, environment.m_window->getContext(), environment.m_window);
		environment.m_options = &options;

		hkDefaultDemo::setupLights(&environment);

		hkDemoDatabase& database = hkDemoDatabase::getInstance();

		hkDemo* demo = database.createDemo(startUpDemo, &environment);

		window->setWindowResizeFunction( _demoResizeNotify, demo );

		const int MAX_FRAMES = 10;
		int frameTimes[MAX_FRAMES] = {0};
		int curFrame = 0;
		int stepCount = 0;
		hkStopwatch ticksSoFar;
		hkStopwatch totalTicks;
		ticksSoFar.start();
		totalTicks.start();
		int previousFpsLimit = options.m_lockFps;
		hkReal minSecondsFrame = options.m_lockFps ? (1.0f / options.m_lockFps) : 0;
		hkUint64 minTicksPerFrame = options.m_lockFps ? (hkStopwatch::getTicksPerSecond() / options.m_lockFps) : 0;


#ifdef ENABLE_SN_TUNER
		snTunerInit();
#endif

		window->getContext()->unlock();

		//
		// main loop
		//
		while ( window->peekMessages() == HKG_WINDOW_MSG_CONTINUE )
		{
#ifdef HK_PS2
			// On PlayStation(R)2 we must restart the performance counters every 7 seconds
			// otherwise we will get an exception which cannot be caught by the user!
			// Please don't move it out, unless you are certain everything works ok on PlayStation(R)2
			//ticksSoFar.stop();
			scePcStart( SCE_PC_CTE | SCE_PC_U0 | SCE_PC_U1 | SCE_PC0_ICACHE_MISS | SCE_PC1_CPU_CYCLE, 0 ,0 );
			//ticksSoFar.start();
#endif

			HKG_TIMER_BEGIN_LIST("_Render", "UpdatePads");

			// Normally, pad1 is emulated by holding CTRL (pad0 is emulated when CTRL is not being held).
			// When only one pad is present, we want pad1 to be emulated with no CTRL so we flip the flag.
			// COM-305: If the user wants to use CTRL, we bypass this.
			hkBool pad0DesiredCtrlState;
			{
				if (environment.m_controlKeyReserved)
				{
					pad0DesiredCtrlState = window->getKeyboard().getKeyState(HKG_VKEY_CONTROL);
				}
				else
				{
					pad0DesiredCtrlState = environment.getDesiredKeyboardCtrlStateForGamePad();			
				}
			}

			pad0->updatePad(0, window, 0.01f, environment.m_options->m_forceKeyboardGamepad, pad0DesiredCtrlState);
			pad1->updatePad(1, window, 0.01f, environment.m_options->m_forceKeyboardGamepad, !pad0DesiredCtrlState);

			HKG_TIMER_SPLIT_LIST("ClearBuffers");
			window->getContext()->lock();
				bool graphicsWindowOK = window->clearBuffers();
			window->getContext()->unlock();
			
			HKG_TIMER_END_LIST();

			hkDemo::Result step = demo->stepDemo();

			// Automatically stop the demo if the user limit for iterations is reached.
			if (options.m_maxIterations > 0)
			{
				stepCount++;
				//hkprintf("Iteration %d of %d\n", stepCount, options.m_maxIterations);
				if (stepCount == options.m_maxIterations)
				{
					step = hkDemo::DEMO_STOP;
					printf("Elapsed Time: %0.3fs\n", totalTicks.getElapsedSeconds());
					printf("Average FPS: %0.3f\n", options.m_maxIterations / totalTicks.getElapsedSeconds());
				}
			}

			if ( step != hkDemo::DEMO_STOP && step != hkDemo::DEMO_RESTART )
			{
				if(options.m_lockFps)
				{
					if (options.m_lockFps != previousFpsLimit)
					{
						minSecondsFrame = options.m_lockFps ? (1.0f / options.m_lockFps) : 0;
						minTicksPerFrame = options.m_lockFps ? (hkStopwatch::getTicksPerSecond() / options.m_lockFps) : 0;
						previousFpsLimit = options.m_lockFps;
					}


// 					HKG_TIMER_BEGIN_LIST("_Render", "SoftwareFrameLock");

					hkUint64 ticks = ticksSoFar.getElapsedTicks();

					while ( minSecondsFrame - ticksSoFar.getElapsedSeconds() > 0 )
					{
					}

					frameTimes[curFrame++] = static_cast<int>(ticks);
					curFrame = curFrame % MAX_FRAMES;

					ticksSoFar.reset();
					ticksSoFar.start();

					int minTime = frameTimes[0];
					for(int i = 1; i < MAX_FRAMES; ++i)
					{
						if(frameTimes[i] < minTime)
						{
							minTime = frameTimes[i];
						}
					}

					// Comment this in to display the frame rate in the top left corner

					//					int fps = 1 + int(hkStopwatch::getTicksPerSecond()/(minTime+1));
					//					hkString str; str.printf("%i", fps);
					//					environment.m_textDisplay->outputText(str, 0,0);

// 					HKG_TIMER_END_LIST();

				}

				// Render the views
				if (graphicsWindowOK) 
				{
					render(window, environment, demo );
				}
			}
			else if ( step == hkDemo::DEMO_STOP )
			{
				break;
			}
			else if ( step == hkDemo::DEMO_RESTART )
			{
				delete demo;
				demo = database.createDemo(options.m_defaultDemo, &environment);
			}


			// Handy reboot to launcher on some platforms@
			HKG_PAD_BUTTON buttons = HKG_PAD_BUTTON_L1 | HKG_PAD_BUTTON_L2 | HKG_PAD_BUTTON_R1 | HKG_PAD_BUTTON_R2;
			if ( (pad0->getButtonState() & buttons) == buttons )
			{
				break;
			}
		}

		//      reEnableSecondaryDisplays();

		delete pad0;
		delete pad1;
		delete demo;
		window->getContext()->lock();
		delete environment.m_textDisplay;
		delete environment.m_displayHandler;
		environment.m_sceneConverter->release();
		environment.m_displayWorld->release();
		window->getContext()->unlock();
		exitCode = environment.m_exitCode;

		window->release();

	}

	threadMemory->setStackArea(0, 0);
	hkDeallocate(stackBuffer);

	threadMemory->removeReference();

	hkgSystem::quit();
	hkResult res = hkBaseSystem::quit();
		
	printf("Base system shut down.\n");

	/// If we have a server we can delete it
	delete server;


	return exitCode | res;
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
