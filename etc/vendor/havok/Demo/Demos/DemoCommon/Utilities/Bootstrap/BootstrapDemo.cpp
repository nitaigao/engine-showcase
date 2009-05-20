/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// Havok Bootstrapper

//
// Includes
//
#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/Bootstrap/BootstrapDemo.h>

#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Demos/DemoCommon/DemoFramework/hkDoubleConvCheck.h>
#include <Demos/DemoCommon/DemoFramework/hkFloatingPointExceptionCheck.h>

#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Graphics/Common/Font/hkgFont.h>

#if defined(USING_HAVOK_PHYSICS)
#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Utilities/Serialize/hkpHavokSnapshot.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>
#endif

#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>
#include <Common/Base/Reflection/Registry/hkVtableClassRegistry.h>

#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Base/System/Io/Reader/Memory/hkMemoryStreamReader.h>
#include <Common/Base/System/Io/Writer/Array/hkArrayStreamWriter.h>
#include <Common/Base/DebugUtil/DeterminismUtil/hkCheckDeterminismUtil.h>

#include <Common/Base/Algorithm/Sort/hkSort.h>

#include <Demos/DemoCommon/DemoFramework/hkPerformanceCounterUtility.h>
#include <Common/Base/Types/Color/hkColor.h>

#include <Common/Internal/Misc/hkSystemDate.h>

// For processor info
#include <Common/Base/Fwd/hkwindows.h>

// Demos for stats bootstrap
struct StatsBootstrapDemoInstance
{
	char* m_name;
};

// Support for the last successful bootstrap demo
extern const char* LASTDEMO_FILENAME_BOOTSTRAP;


	// set the next define to force the bootstrap to execute a single demo only
//#define SPECIAL_DEMO "PhysicsApi/Dynamics/Constraints/BallAndSocketRope/Length 200"
//#define SPECIAL_DEMO "Examples/Physics/Continuous/BrickWall/8x8x3 Continuous"
void hkSetLastDemoBootstrap(const char* namein)
{
	hkOfstream out(LASTDEMO_FILENAME_BOOTSTRAP);
	if(out.isOk())
	{
		out << namein;
	}
}

extern const char* hkSetLastDemo(const char* namein);

// Demo type masks
static const int DEMO_MASK_ALL = HK_DEMO_TYPE_COMPLETE |
								 HK_DEMO_TYPE_ANIMATION |
								 HK_DEMO_TYPE_PHYSICS |
								 HK_DEMO_TYPE_CLOTH |
								 HK_DEMO_TYPE_PRIME |
								 HK_DEMO_TYPE_BEHAVIOR;

static const int DEMO_MASK_PHYSICS = HK_DEMO_TYPE_PHYSICS |
									 HK_DEMO_TYPE_PRIME;

static const int DEMO_MASK_ANIMATION = HK_DEMO_TYPE_ANIMATION;
static const int DEMO_MASK_SERIALIZE = HK_DEMO_TYPE_SERIALIZE;

static const int DEMO_MASK_BEHAVIOR = HK_DEMO_TYPE_BEHAVIOR;

static const int DEMO_MASK_DESTRUCTION = HK_DEMO_TYPE_DESTRUCTION |
										 HK_DEMO_TYPE_DESTRUCTION_CRITICAL |
										 HK_DEMO_TYPE_DESTRUCTION_CRITICAL_FILEBASED;

static const int DEMO_MASK_CLOTH = HK_DEMO_TYPE_CLOTH;

static const int DEMO_MASK_ALL_CRITICAL = HK_DEMO_TYPE_CRITICAL;

static const int DEMO_MASK_AI = HK_DEMO_TYPE_AI;

static const int DEMO_MASK_ALL_STATS = HK_DEMO_TYPE_STATS;

static const int DEMO_MASK_STATS_CRITICAL = HK_DEMO_TYPE_STATS |
											HK_DEMO_TYPE_CRITICAL;



// Definitions of variants
struct BootstrapVariant
{
	const char*	m_name;
	BootstrapDemo::TestType m_testType;
	int m_demoTypeMask;
	const char* m_details;
};


static const BootstrapVariant g_variants[] =
{
{ "BootstrapAll",			BootstrapDemo::TEST_NORMAL, DEMO_MASK_ALL,			"Running all demos", },
{ "BootstrapPhysics",		BootstrapDemo::TEST_NORMAL, DEMO_MASK_PHYSICS,		"Running physics demos" },
{ "BootstrapAnimation",		BootstrapDemo::TEST_NORMAL, DEMO_MASK_ANIMATION,	"Running animation demos" },
{ "BootstrapBehavior",		BootstrapDemo::TEST_NORMAL, DEMO_MASK_BEHAVIOR,		"Running behavior demos" },
{ "BootstrapDestruction",	BootstrapDemo::TEST_NORMAL, DEMO_MASK_DESTRUCTION,	"Running destruction demos" },
{ "BootstrapCloth",			BootstrapDemo::TEST_NORMAL, DEMO_MASK_CLOTH,		"Running cloth demos" },
{ "BootstrapAI",			BootstrapDemo::TEST_NORMAL, DEMO_MASK_AI,			"Running AI demos" },
{ "BootstrapSerialize",		BootstrapDemo::TEST_NORMAL, DEMO_MASK_SERIALIZE,	"Running demos using serialization" },
{ "BootstrapCritical",		BootstrapDemo::TEST_NORMAL, DEMO_MASK_ALL_CRITICAL,	"Running all critical demos" },

{ "BootstrapStats",					BootstrapDemo::TEST_STATISTICS, 				DEMO_MASK_ALL_STATS,		"Running demos to gather statistics" },
{ "BootstrapStatsCritical",			BootstrapDemo::TEST_STATISTICS_CRITICAL,		DEMO_MASK_STATS_CRITICAL,	"Running critical demos to gather statistics" },
{ "BootstrapStatsDetailedTimings",	BootstrapDemo::TEST_STATISTICS_DETAILED, 		DEMO_MASK_ALL_STATS,		"Running demos to gather detailed statistics" },
{ "BootstrapStatsSingleThreaded",	BootstrapDemo::TEST_STATISTICS_SINGLE_THREADED, DEMO_MASK_ALL_STATS,		"Running demos to gather single threaded statistics" },

{ "BootstrapFpuExceptionsEnabled", BootstrapDemo::TEST_FPU_EXCEPTIONS, DEMO_MASK_ALL, "Running all demos, with FPU exceptions" },

{ "SerializeAll (Binary)", BootstrapDemo::TEST_SERIALIZE_BINARY, DEMO_MASK_ALL, "Binary serializing all demos" },
{ "SerializeAll (XML)",    BootstrapDemo::TEST_SERIALIZE_XML,    DEMO_MASK_ALL, "XML serializing all demos" },

#if defined(USING_HAVOK_PHYSICS) && defined(HK_ENABLE_DETERMINISM_CHECKS)
{ "DeterminismPhysics",    BootstrapDemo::TEST_DETERMINISM, DEMO_MASK_PHYSICS, "Determinism testing all demos" },
{ "Mt DeterminismPhysics", BootstrapDemo::TEST_MULTITHREADING_DETERMINISM, DEMO_MASK_PHYSICS, "Multi threading determinism testing all demos" },
#endif
};




static void hkMtSafeDeleteDemo(hkDemo* demo)
{
	if ( demo )
	{
		hkReferencedObject::setLockMode( hkReferencedObject::LOCK_MODE_AUTO );
		demo->preDeleteDemo();
		delete demo;
		hkReferencedObject::setLockMode( hkReferencedObject::LOCK_MODE_NONE );
	}
}

static void hkOutputStatsSummaryToFile( const char* filename, const hkObjectArray<BootstrapDemo::StatsRecord>& statsRecords, int numThreads, int numSpus );



void BootstrapDemo::preRenderDisplayWorld(hkgViewport* v)
{
	if (m_demo != HK_NULL )
	{
		m_demo->preRenderDisplayWorld(v);
	}
}

void BootstrapDemo::postRenderDisplayWorld(hkgViewport* v)
{
	if (m_demo != HK_NULL )
	{
		m_demo->postRenderDisplayWorld(v);
	}
}

//
// Constructor
//
BootstrapDemo::BootstrapDemo(hkDemoEnvironment* env)
	:	hkDemo(env),
		m_demoTypeMask(0),
		m_testType(TEST_NORMAL),
		m_demo(HK_NULL),
		m_demoIndex(0),
		m_steps(0),
		m_numThreads(2),
		m_allocatedData(HK_NULL),
		m_originalWorld(HK_NULL),
		m_counter(0),
		m_streamAnalyzer(HK_NULL),
		m_statsTotalTime(0.0f),
		m_statsNumSamples(0),
		m_runsPerGame(2),
		m_runIndex(0)
{
#if defined(USING_HAVOK_PHYSICS)
	m_forceMultithreadedSimulationBackup = hkpWorld::m_forceMultithreadedSimulation;
#endif

	//
	// Process variant settings
	//
	{
		const BootstrapVariant& variant =  g_variants[env->m_variantId];
		m_demoTypeMask = variant.m_demoTypeMask;
		m_testType = variant.m_testType;
	}

	if ( ( m_testType == TEST_STATISTICS ) 
		|| ( m_testType == TEST_STATISTICS_SINGLE_THREADED ) 
		|| ( m_testType == TEST_STATISTICS_DETAILED ) 
		|| ( m_testType == TEST_STATISTICS_CRITICAL ) )
	{
		m_streamAnalyzer = new hkMonitorStreamAnalyzer( 80000000 ); //80 megs for multithreading
	}

	if ( m_testType == TEST_DETERMINISM || m_testType == TEST_MULTITHREADING_DETERMINISM )
	{
		//m_options.m_stepsPerDemo = 150;
		hkCheckDeterminismUtil::createInstance();
	}

	//
	// Get suitable demo entries
	//
	{
		const hkObjectArray<hkDemoEntry>& db = hkDemoDatabase::getInstance().getDemos();
		for (int i=0; i<db.getSize(); i++)
		{
			const hkDemoEntry& entry = db[i];

			int useDemoTest = 0;
			useDemoTest = ( entry.m_demoTypeFlags & m_demoTypeMask );

			if ( m_demoTypeMask == DEMO_MASK_STATS_CRITICAL)
			{
				useDemoTest = ( !(( entry.m_demoTypeFlags & m_demoTypeMask ) ^ m_demoTypeMask ) );
			}

#if !defined(SPECIAL_DEMO)
			if ( useDemoTest )
#endif
			{
				bool useDemo = true;

					// take all demos if not special demo selected
#ifdef SPECIAL_DEMO
				useDemo = entry.m_menuPath.beginsWith(SPECIAL_DEMO);
#endif

				if ( m_testType == TEST_DETERMINISM || m_testType == TEST_MULTITHREADING_DETERMINISM )
				{
					// these don't work for determinism checks
					useDemo &=  ! entry.m_menuPath.beginsWith("CommonApi");

					useDemo &=  ! entry.m_menuPath.beginsWith("DemoCommon");
					useDemo &=  ! entry.m_menuPath.beginsWith("Animation");

					useDemo &=  ! entry.m_menuPath.beginsWith("Physics/Api/Dynamics/MemoryIssues");
					useDemo &=  ! entry.m_menuPath.beginsWith("PhysicsApi/VehiclePhysics/SerializedVehicle");
					useDemo &=  ! entry.m_menuPath.beginsWith("Physics/Test/Feature/Dynamics/AsynchronuousSpecialEffectsThread");	// nondeterministic second thread
					useDemo &=  ! entry.m_menuPath.beginsWith("Resources/Physics/UseCase/AssetStreaming");

					useDemo &=  ! entry.m_menuPath.beginsWith("Physics/UseCase/CharacterControl");						// non deterministic
					useDemo &=  ! entry.m_menuPath.beginsWith("Physics/UseCase/Fracture");								// slow and not deterministic

					// and those are not needed and/or too slow
					useDemo &=  ! entry.m_menuPath.beginsWith("Physics/Test/Performance/BenchmarkSuite");		// way too slow

					useDemo &=  ! entry.m_menuPath.beginsWith("Resources/Common/Api/Serialize/SimpleLoad/Simple"); // unrelevant

					useDemo &=  ! entry.m_menuPath.endsWith("BatchAddRemoveBodies"); // slow
					useDemo &=  ! entry.m_menuPath.beginsWith("PhysicsApi/CollisionDetection/Raycasting"); // unrelevant
					useDemo &=  ! entry.m_menuPath.beginsWith("PhysicsApi/ContinuousPhysics/DiscreteVsContinuous"); // two worlds in the demo, not supported

					// vehicles are not deterministic because of the phantom used
					useDemo &=  ! entry.m_menuPath.beginsWith("Physics/Api/Vehicle");							// non deterministic
					useDemo &=  ! entry.m_menuPath.beginsWith("ShowCase/Gdc2007");								// non deterministic



					useDemo &=  ! entry.m_menuPath.beginsWith("Physics/UseCase/Fountain");		// broadphase border resets position in nondeterminstic order
																										// <ag.todo.b> make the non-determinism warning disable the hkCheckDeterminismUtil at the same time ..
					useDemo &=  ! entry.m_menuPath.beginsWith("Examples/Physics/Ragdoll/SlidingRagdolls"); 
					useDemo &=  ! entry.m_menuPath.endsWith("DestructibleWalls/Destructible Walls");		// uses a second world to simulate the wall
					useDemo &=  ! entry.m_menuPath.endsWith("DestructibleWalls/Single Brick Tmp");			// uses a second world to simulate the wall

					useDemo &=  ! entry.m_menuPath.endsWith("RagdollVsMopp/60Hz Discrete RealTime");			// uses camera to set ragdoll velocity
					useDemo &=  ! entry.m_menuPath.endsWith("RagdollVsMopp/30Hz Discrete RealTime");			// uses camera to set ragdoll velocity
					useDemo &=  ! entry.m_menuPath.endsWith("RagdollVsMopp/15Hz Discrete RealTime");			// uses camera to set ragdoll velocity
					useDemo &=  ! entry.m_menuPath.endsWith("RagdollVsMopp/10Hz Discrete RealTime");			// uses camera to set ragdoll velocity
					useDemo &=  ! entry.m_menuPath.endsWith("RagdollVsMopp/15Hz Discrete 4xSlower");			// uses camera to set ragdoll velocity
					useDemo &=  ! entry.m_menuPath.endsWith("RagdollVsMopp/30Hz Continuous RealTime");			// uses camera to set ragdoll velocity
					useDemo &=  ! entry.m_menuPath.endsWith("RagdollVsMopp/15Hz Continuous RealTime");			// uses camera to set ragdoll velocity
					useDemo &=  ! entry.m_menuPath.endsWith("RagdollVsMopp/10Hz Continuous RealTime");			// uses camera to set ragdoll velocity
					useDemo &=  ! entry.m_menuPath.endsWith("RagdollVsMopp/7 Hz Continuous RealTime");			// uses camera to set ragdoll velocity
					useDemo &=  ! entry.m_menuPath.endsWith("RagdollVsMopp/5 Hz Continuous RealTime");			// uses camera to set ragdoll velocity


				}
				if( m_testType == TEST_SERIALIZE_BINARY || m_testType == TEST_SERIALIZE_XML )
				{
					if(    entry.m_demoTypeFlags != HK_DEMO_TYPE_COMPLETE
						&& entry.m_demoTypeFlags != HK_DEMO_TYPE_PHYSICS
						&& entry.m_demoTypeFlags != HK_DEMO_TYPE_PRIME )
					{
						useDemo = false;
					}
				}

				if ( useDemo )
				{
					DemoEntry newEntry;
					newEntry.m_entry = &entry;
					m_entries.pushBack(newEntry);
				}
			}
		}
	}

	hkError::getInstance().setEnabled( 0xAF55ADDE, false); // Disable hkFreeListMemory warnings


	//
	// Start from last/next demo?
	//
	if ((env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_2) || m_env->m_options->m_runLastDemo) ||
		(env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_3) || m_env->m_options->m_runNextDemo))
	{
		hkIfstream is(LASTDEMO_FILENAME_BOOTSTRAP);
		if( is.isOk() )
		{
			char name[1024]; is.getline(name, 1024);

			// Set demo index
			for (int i = 0; i < m_entries.getSize(); i++ )
			{
				if (m_entries[i].m_entry->m_menuPath.endsWith(name))
				{
					m_demoIndex = i;

					if( env->m_gamePad->isButtonPressed(HKG_PAD_BUTTON_3) || m_env->m_options->m_runNextDemo )
					{
						m_demoIndex++;
					}
					break;
				}
			}
		}
	}
}

BootstrapDemo::~BootstrapDemo()
{
	hkError::getInstance().setEnabled( 0xAF55ADDE, true); // Disable hkFreeListMemory warnings

	hkSetLastDemoBootstrap("[DONE]"); // For automated testing.
	hkcout << "[DONE]" << hkendl;

	hkMtSafeDeleteDemo(m_demo);

	delete m_streamAnalyzer;

#if defined(USING_HAVOK_PHYSICS)
	hkpWorld::m_forceMultithreadedSimulation = m_forceMultithreadedSimulationBackup;
	m_env->m_options->m_forceMT = m_forceMultithreadedSimulationBackup;
#endif

	if ( m_testType == TEST_DETERMINISM || m_testType == TEST_MULTITHREADING_DETERMINISM )
	{
		hkCheckDeterminismUtil::destroyInstance();
	}

	if ( ( m_testType == TEST_STATISTICS ) 
		|| ( m_testType == TEST_STATISTICS_SINGLE_THREADED ) 
		|| ( m_testType == TEST_STATISTICS_DETAILED ))
	{
		hkString defaultStatsDir = "Statistics/";
		hkString statsDir = (m_env->m_options->m_statsDir == HK_NULL ? defaultStatsDir : m_env->m_options->m_statsDir);

		// Add the stats to the multiplatform stats file.
		hkString allStatsFile = statsDir + hkString("Multiplatform_Performance_Statistics");
		hkOutputStatsSummaryToFile( allStatsFile.cString(), m_statsRecords, m_env->m_options->m_numThreads, m_env->m_options->m_numSpus );

		// Add the stats to the current platform's stats file.
		hkString platStatsFile = statsDir + getPlatform(true) + hkString("_Performance_Statistics");
		hkOutputStatsSummaryToFile( platStatsFile.cString(), m_statsRecords, m_env->m_options->m_numThreads, m_env->m_options->m_numSpus );

		// Create a unique stats file for this (day's) stats run.
		char stringDate[80];
		hkSystemDate::getStringDate(stringDate);
		hkString uniqueStatsFile = statsDir + getPlatform(true) +  hkString("_") + hkString(HAVOK_SDK_VERSION_STRING) 
								 + hkString("_") + hkString(stringDate) + hkString("_Performance_Statistics");
		hkOutputStatsSummaryToFile( uniqueStatsFile.cString(), m_statsRecords, m_env->m_options->m_numThreads, m_env->m_options->m_numSpus );
	}

}
static void fillStatus(hkArray<char>& bar, int cur, int max, int maxchars)
{
	if(cur > max)
	{
		cur = max;
	}
	bar.clear();
	if(maxchars > 0)
	{
		bar.reserve(maxchars+1);
		bar.setSize(maxchars, ' ');
		bar[0] = '[';
		for(int i = 1; i < maxchars * cur / max; ++i)
		{
			bar[i] = '#';
		}
		bar.back() = ']';
	}
	bar.pushBack(0);
}


//
// step demo
//
hkDemo::Result BootstrapDemo::stepDemo()
{
	const int START_INDEX = 0;
	const int END_INDEX = hkMath::min2( START_INDEX + 800, m_entries.getSize() );
	if( m_demoIndex < START_INDEX )
	{
		m_demoIndex = START_INDEX;
	}

	//
	// No demos left?
	//
	if( m_demoIndex >= END_INDEX )
	{
		return DEMO_STOP;
	}

	const hkDemoEntry* entry = m_entries[m_demoIndex].m_entry;

	//
	// Need to start the next demo?
	//
	if(m_demo == HK_NULL)
	{
		{
			if (( m_testType == TEST_MULTITHREADING_DETERMINISM ) 
				|| ( m_testType == TEST_STATISTICS ) 
				|| ( m_testType == TEST_STATISTICS_SINGLE_THREADED ) 
				|| ( m_testType == TEST_STATISTICS_DETAILED )
				|| ( m_testType == TEST_STATISTICS_CRITICAL ) )
			{
				if ( m_testType == TEST_STATISTICS_SINGLE_THREADED )
				{
#if defined(USING_HAVOK_PHYSICS)
					hkpWorld::m_forceMultithreadedSimulation = false;
#endif
					m_env->m_options->m_forceMT = false;

					// Force single-threaded execution.
					m_env->m_options->m_numSpus = 0;
					m_env->m_options->m_numThreads = 1;
				}
				else
				{
#if defined(USING_HAVOK_PHYSICS)
					hkpWorld::m_forceMultithreadedSimulation = true;
#endif
					m_env->m_options->m_forceMT = true;

					// Set the appropriate number of spus.
#if defined(HK_PLATFORM_PS3_PPU)
					m_env->m_options->m_numSpus = 5;
#else
					m_env->m_options->m_numSpus = 0;
#endif
					// Set the appropriate number of threads.
					hkHardwareInfo info;
					hkGetHardwareInfo(info);
					m_env->m_options->m_numThreads = info.m_numThreads;

#if defined(HK_PLATFORM_SIM)
					m_env->m_options->m_numSpus = 1;
					m_env->m_options->m_numThreads = 2;
#endif
				}
			}

			hkcout << m_demoIndex << ":" << entry->m_menuPath << hkendl;
			m_env->m_menuPath = entry->m_menuPath;
			m_env->m_demoPath = entry->m_demoPath;
			m_env->m_variantId = entry->m_variantId;
			hkSetLastDemoBootstrap(entry->m_menuPath.cString());

#		if defined(HK_ENABLE_DETERMINISM_CHECKS)
			if ( m_runIndex == 0 )
			{
				hkCheckDeterminismUtil::getInstance().startWriteMode();
			}
			else
			{
				hkCheckDeterminismUtil::getInstance().startCheckMode();
			}
#		endif

			// Create the demo
			hkReferencedObject::lockAll();
			m_env->m_window->getContext()->lock(); // ctors can assume that HKG is locked normally for it
			m_demo = (*entry->m_func)(m_env);
			m_demo->postConstruct();
			m_env->m_window->getContext()->unlock();
			hkReferencedObject::unlockAll();

			// Reset timer info

			{
				if (m_streamAnalyzer)
				{
					int numThreadStreams;
					int numSpuStreams;
					m_demo->getNumTimerStreams( numThreadStreams, numSpuStreams );
					m_streamAnalyzer->reset();
					m_streamAnalyzer->resetNumThreads( numThreadStreams, numSpuStreams );
				}
				hkMonitorStream::getInstance().reset();
				m_statsTotalTime = 0.0f;
				m_statsNumSamples = 0;
			}

			m_steps = 0;
		}

		if (m_testType == TEST_SERIALIZE_BINARY || m_testType == TEST_SERIALIZE_XML)
		{
#if defined(USING_HAVOK_PHYSICS)
			// keep our old one (so that the game dtor can do whatever it is it usually does)
			m_originalWorld = static_cast<hkDefaultPhysicsDemo*>(m_demo)->m_world;

			// remove the world from the context used by the visualize lib
			static_cast<hkDefaultPhysicsDemo*>(m_demo)->getPhysicsViewerContext()->removeWorld(m_originalWorld);
#endif
			m_counter = 0;
		}
	}

	const int stepsMax = m_demo->m_bootstrapIterations;

	//
	// Status bar
	//
	{
		int charwidth = (int)m_env->m_textDisplay->getFont()->getCharWidth();
		int charheight = (int)m_env->m_textDisplay->getFont()->getCharHeight();

		char statusBuf[512];
		hkOstream status(statusBuf, sizeof(statusBuf), true);
		status.printf("%s\n", entry->m_menuPath.cString());

		{
			hkArray<char> bar;
			int maxchars = (m_env->m_window->getWidth() / charwidth) - 20;
			fillStatus(bar, m_steps, stepsMax, maxchars);
			status.printf("iter %3i / %3i - %s\n", m_steps, stepsMax, bar.begin());

			int numdemos = m_entries.getSize();
			fillStatus(bar, m_demoIndex, numdemos, maxchars);
			status.printf("demo %3i / %3i - %s", m_demoIndex, numdemos, bar.begin());
		}

		int starty = m_env->m_window->getHeight() - 4 * charheight;
		m_env->m_textDisplay->outputText(statusBuf, charwidth*2, starty);
	}

	m_env->m_displayHandler->clear();

	//
	// Divert according to test types
	//
	switch (m_testType)
	{
	case TEST_NORMAL:
	case TEST_FPU_EXCEPTIONS:
		NormalStepDemo();
		break;

	case TEST_STATISTICS:
	case TEST_STATISTICS_SINGLE_THREADED:
	case TEST_STATISTICS_DETAILED:
	case TEST_STATISTICS_CRITICAL:
		StatsStepDemo();
		break;

	case TEST_SERIALIZE_BINARY:
	case TEST_SERIALIZE_XML:
		SerializeStepDemo();
		break;

	case TEST_DETERMINISM:
	case TEST_MULTITHREADING_DETERMINISM:
		DeterminismStepDemo();
		break;

	default:
		HK_ASSERT2(0x0, false, "Test type unaccounted for");
		break;
	}

	return DEMO_OK;
}


hkDemo::Result BootstrapDemo::stepVisualDebugger()
{
	if (m_demo)
	{
		m_demo->stepVisualDebugger();
	}
	return DEMO_OK;
}

bool BootstrapDemo::visualDebuggerEnabled()
{
	if (m_demo)
	{
		return m_demo->visualDebuggerEnabled();
	}
	else
	{
		return false;
	}
}

char* BootstrapDemo::getPlatform(hkBool simple)
{
	if (simple)
	{
		return "PC";
	}

	struct _SYSTEM_INFO info;
	GetSystemInfo( &info );

	const char* processor;
	switch (info.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_INTEL :
		// INTEL really means x86 here (see MSDN docs)
		// so check the vendor-specific wProcessorLevel as well.
		switch( info.wProcessorLevel )
		{
		case 6:
			processor = "x86_Intel";
			break;
		case 15:
			processor = "x86_AMD";
			break;
		default:
			hkString unknownProcessor;
			unknownProcessor.printf("x86_UnknownChipset%d", info.wProcessorLevel);
			processor = unknownProcessor.cString();
			break;
		}
		break;
	case PROCESSOR_ARCHITECTURE_IA64 :
		processor = "IA64";
		break;
	case PROCESSOR_ARCHITECTURE_AMD64 :
		processor = "x64";
		break;
	case PROCESSOR_ARCHITECTURE_UNKNOWN :
	default:
		processor = "UnknownArch";
		break;
	}

	static char buffer[1024];
	hkString::sprintf(buffer, "%s_rev_%x_%d_processors", processor, info.wProcessorRevision, info.dwNumberOfProcessors );
	return buffer;
}

void BootstrapDemo::StatsStepDemo()
{
	hkStopwatch timer;
	timer.start();
	HK_ON_DEBUG( Result result = ) m_demo->stepDemo();
	HK_ASSERT(0, result == DEMO_OK );
	timer.stop();

	// Skip 1st frame
	if (m_statsNumSamples > 0)
		m_statsTotalTime += timer.getSplitSeconds();
	m_statsNumSamples++;

	hkArray<hkTimerData> threadStreams;
	hkArray<hkTimerData> spuStreams;
	m_demo->getTimerStreamInfo( threadStreams, spuStreams );

	hkMonitorStreamFrameInfo frameInfo;
	frameInfo.m_heading = "Timer values are usecs";
	frameInfo.m_indexOfTimer0 = 0;
	frameInfo.m_indexOfTimer1 = -1;
	frameInfo.m_timerFactor0 = 1e6f / float(hkStopwatch::getTicksPerSecond());
	frameInfo.m_timerFactor1 = 1.0f;

	{
		for (int i =0; i < threadStreams.getSize(); i++ )
		{
			frameInfo.m_threadId = i;
			m_streamAnalyzer->captureFrameDetails( threadStreams[i].m_streamBegin, threadStreams[i].m_streamEnd, frameInfo );
		}
	}

	{
		for (int i =0; i < spuStreams.getSize(); i++ )
		{
			frameInfo.m_threadId = i + threadStreams.getSize();
			m_streamAnalyzer->captureFrameDetails( spuStreams[i].m_streamBegin, spuStreams[i].m_streamEnd, frameInfo );
		}
	}

	m_steps++;

	m_demo->resetTimerStreams();

	//
	// Check for demo ended
	//
	if ( m_steps >= m_demo->m_bootstrapIterations )
	{
		hkString demoName = m_entries[m_demoIndex].m_entry->m_menuPath.substr(m_entries[m_demoIndex].m_entry->m_menuPath.lastIndexOf('/') + 1);

		hkString statsFileName;
		{
			int a = m_entries[m_demoIndex].m_entry->m_menuPath.lastIndexOf( '/' );
			int b = m_entries[m_demoIndex].m_entry->m_menuPath.lastIndexOf('/', 0, a ) + 1;
			hkString fileName = m_entries[m_demoIndex].m_entry->m_menuPath.substr(b);
			fileName = fileName.replace('/', '_');

			hkString statsDir = "Statistics/";
			statsFileName += (m_env->m_options->m_statsDir == HK_NULL ? statsDir : m_env->m_options->m_statsDir);
			if ( m_demoTypeMask == DEMO_MASK_STATS_CRITICAL )
			{
				statsFileName += "Crit_";
			}
			statsFileName += getPlatform();
			statsFileName += "_";
			statsFileName += fileName;
			statsFileName += ".txt";
		}

		hkReal average = (m_statsTotalTime*1000000)/m_statsNumSamples;
		
		// critical statistics
		// will read in previous average times from a file and see if current
		// average time is significantly slower or faster
		if ( m_demoTypeMask == DEMO_MASK_STATS_CRITICAL )
		{
			char readString[80] = "";

			// temporary list to store previous average times
			hkArray< hkReal > previousAverageTimes;
			previousAverageTimes.clear();

			// istream has to be in its own scope so the handle on the file is lost
			// this allows the out stream to write the file later on, can cause problems on the xbox360 without this
			{
				hkIfstream istr(statsFileName.cString());
				if (istr.isOk())
				{
					do
					{
						istr.getline(readString, 80, '\n');

						// add previous average times to list
						if (hkString(readString).getLength() != 0)
						{
							previousAverageTimes.pushBack(hkString::atof(readString));
						}
					}
					while (hkString(readString).getLength() != 0);
				}
			}

			// if the file is empty
			if (!previousAverageTimes.isEmpty())
			{
				// check for a significant difference faster or slower than 10%
				hkReal oldAverage = previousAverageTimes.back();
				hkBool performanceDifference = (((oldAverage / average) > 1.1) || ((average / oldAverage) > 1.1));
				if (performanceDifference)
				{
					// throw error if significant performance difference
					char averageString[80];
					hkString::sprintf (averageString, "%f", average);
					HK_ERROR (0x55e10a7e, (hkString("Performance of demo: ") + demoName + hkString(" took ")
							+ hkString(averageString) +	hkString(" microseconds. (At least 10 percent difference.)")));
				}
			}
			// print out file again
			hkOfstream ostr(statsFileName.cString());

			for (int i = 0; i < previousAverageTimes.getSize(); i++)
			{
				ostr.printf("%f\n", previousAverageTimes[i] );
			}
			ostr.printf("%f\n", average );
		}
		// not critical statistics
		else
		{
			StatsRecord record;
			record.m_demoName = m_entries[m_demoIndex].m_entry->m_menuPath;
			record.m_avgStepDemoTime = average;
			m_statsRecords.pushBack( record );

			hkSort( m_statsRecords.begin(), m_statsRecords.getSize() );

			//hkString name; name.printf("Profile%s_%s.txt", prefix, type);
			hkOfstream ostr(statsFileName.cString());

			ostr.printf("DemoName: %s\n", demoName.cString());
			ostr.printf("============================\n\n");
			ostr.printf("Average time (microsecs): %f\n\n", average );
#if !defined(HK_PLATFORM_PSP) && !defined(HK_PLATFORM_PS3_PPU)
			
			int reportLevel = (	( m_testType == TEST_STATISTICS_SINGLE_THREADED )
								? hkMonitorStreamAnalyzer::REPORT_TOTAL
								: hkMonitorStreamAnalyzer::REPORT_ALL );
			if (( m_testType == TEST_STATISTICS_SINGLE_THREADED ) || ( m_testType == TEST_STATISTICS_DETAILED ))
			{
				m_streamAnalyzer->writeStatistics( ostr, reportLevel );
			}
#endif
		}
		m_streamAnalyzer->reset();

		hkMtSafeDeleteDemo(m_demo);
		m_demo = HK_NULL;
		m_env->m_displayHandler->clear(); // clear debug display

		m_demoIndex++;
	}

}

void BootstrapDemo::NormalStepDemo()
{
	pushDoubleConversionCheck(true);

	if (m_testType == TEST_FPU_EXCEPTIONS)
	{
		// Currently codebase is checked for zero divide and denormals
#if (HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED)
		// 0x0004 = _MM_EXCEPT_DIV_ZERO
		// 0x0002 = _MM_EXCEPT_DENORM
		hkPushFPUState( 0x0004 | 0x0002 );
#else
		// 0x00000008 = EM_ZERODIVIDE
		// 0x00080000 = EM_DENORMAL
		hkPushFPUState(0x00000008 | 0x00080000 );
#endif
	}

	m_demo->makeFakeInput();
	Result result = m_demo->stepDemo();

	if (m_testType == TEST_FPU_EXCEPTIONS)
	{
		hkPopFPUState();
	}

	popDoubleConversionCheck();

	// The PLAYSTATION(R)3 bootstrapper uses TTY output to determine if the executable has timed out.
	// This helps keep it alive.
	if ( (m_steps % 10) == 0)
	{
		hkcout << ".";
	}

	if (result != DEMO_PAUSED)
	{
		m_steps++;
	}

	const int stepsMax = m_demo->m_bootstrapIterations;

	//
	// Check for demo ended
	//
	if( (result != DEMO_OK && result != DEMO_PAUSED)
		|| m_steps >= stepsMax
		|| m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_0) )
	{
		if(result != DEMO_OK)
		{
			HK_WARN(0x2057da93, "Demo " << m_entries[m_demoIndex].m_entry->m_menuPath << " exited early!");
		}

		hkcout << hkendl;

		hkMtSafeDeleteDemo(m_demo);
		m_demo = HK_NULL;
		m_env->m_displayHandler->clear(); // clear debug display

		m_demoIndex++;
	}
}

void BootstrapDemo::SerializeStepDemo()
{
#if defined(USING_HAVOK_PHYSICS)
	//HK_ASSERT(0x0, m_demo->getType() == hkDefaultDemo::DEMO_TYPE_PHYSICS);
	hkDefaultPhysicsDemo* demo = static_cast<hkDefaultPhysicsDemo*>(m_demo);

	if (m_counter < 1 )
	{
		m_counter++;
		m_env->m_displayHandler->clearDisplay();
		demo->cleanupGraphics();

		if (demo->m_world)
		{
			hkArray<char> memStream;
			//
			// Save and dealloc old
			//
			{
				hkArrayStreamWriter writer( &memStream, hkArrayStreamWriter::ARRAY_BORROW );
 				HK_ON_DEBUG( hkBool res = ) hkpHavokSnapshot::save(demo->m_world, &writer, m_testType==TEST_SERIALIZE_BINARY );
				HK_ASSERT( 0x215d080d, res );
			}
			if(0) // debugging dumps
			{
				hkOstream dumpb("dump.bin");
				dumpb.write( memStream.begin(), memStream.getSize() );
				hkOstream dumpt("dump.txt");
				dumpt.write( memStream.begin(), memStream.getSize() );
			}

			if (m_originalWorld != demo->m_world)
			{
				demo->m_world->removeReference();
			}

			if (m_allocatedData)
			{
				m_allocatedData->removeReference();
				m_allocatedData = HK_NULL;
			}


			//
			// Test file load times on PlayStation(R)2 etc:
			if (0)
			{
				{
					hkOstream o("test.hkb");
					o.write(memStream.begin(), memStream.getSize());
				}
				hkIstream i("test.hkb");
				hkpPhysicsData* physicsData = hkpHavokSnapshot::load(i.getStreamReader(), &m_allocatedData);
				demo->m_world = physicsData->createWorld();
			}
			else
			{
				//
				// Load and create new
				//
				hkMemoryStreamReader reader( memStream.begin(), memStream.getSize(), hkMemoryStreamReader::MEMORY_INPLACE );
				hkpPhysicsData* physicsData = hkpHavokSnapshot::load(&reader, &m_allocatedData);
				if( (demo->m_flags & hkDefaultPhysicsDemo::DEMO_FLAGS_NO_SERIALIZE) == 0 )
				{
					demo->m_world = physicsData->createWorld();
				}
			}

			//
			// some extra debugging:
			//
			/*
			{
				game->m_debugViewerNames.pushBack( hkpBroadphaseViewer::getName()  );
				game->m_debugViewerNames.pushBack( hkpConstraintViewer::getName()  );
				game->m_debugViewerNames.pushBack( hkpContactPointViewer::getName()  );
				game->m_debugViewerNames.pushBack( hkpSimulationIslandViewer::getName()  );
			}
			*/
			demo->setupGraphics();
		}
	}

	pushDoubleConversionCheck(true);

	if	( demo->m_world &&
		( demo->m_flags & hkDefaultPhysicsDemo::DEMO_FLAGS_NO_SERIALIZE) == 0 )
	{
		demo->m_world->stepDeltaTime(.016f);
	}

	m_steps++;

	popDoubleConversionCheck();

	const int stepsMax = m_demo->m_bootstrapIterations;

	if( m_steps >= stepsMax || m_env->m_gamePad->wasButtonPressed(HKG_PAD_BUTTON_0) )
	{
		//if (m_demo->getType() == hkDefaultDemo::DEMO_TYPE_PHYSICS )
		{
			demo->m_world = m_originalWorld;
			m_env->m_displayHandler->clearDisplay();
			demo->cleanupGraphics();
			if (m_allocatedData)
			{
				m_allocatedData->removeReference();
				m_allocatedData = HK_NULL;
			}
		}

		hkMtSafeDeleteDemo(m_demo);
		m_demo = HK_NULL;

		m_demoIndex++;
	}
#endif // USING_HAVOK_PHYSICS
}

void BootstrapDemo::DeterminismStepDemo()
{
#if defined(USING_HAVOK_PHYSICS)
	hkDefaultPhysicsDemo* demo = static_cast<hkDefaultPhysicsDemo*>(m_demo);
	hkpWorld* world = demo->m_world;
	if( !world )
	{
		goto GOTO_NEXT_DEMO;
	}
#endif

#if ! defined (HK_ENABLE_DETERMINISM_CHECKS)
	HK_ASSERT2( 0xf0212345, false, "Please enable the determinism utility to use this feature");
#endif


	pushDoubleConversionCheck(true);
	m_demo->stepDemo();
	popDoubleConversionCheck();


		// increment step
	m_steps++;

	// if demo is not ended yet, continue
	if( m_steps < m_demo->m_bootstrapIterations)
	{
		return;
	}

#if defined(USING_HAVOK_PHYSICS)
GOTO_NEXT_DEMO:
#endif
	hkMtSafeDeleteDemo(m_demo);
	m_demo = HK_NULL;
	HK_ON_DETERMINISM_CHECKS_ENABLED( hkCheckDeterminismUtil::getInstance().finish() );
	m_env->m_displayHandler->clear(); // clear debug display

	if (++m_runIndex >= m_runsPerGame)
	{
		m_demoIndex++;
		m_runIndex = 0;
	}
}

static hkOfstream* createAndAppend(const char* filename)
{
	hkArray<char> existingContent;
	{
		hkIfstream existingFile( filename );
		if (existingFile.isOk())
		{
			existingFile.getStreamReader()->seek(0, hkStreamReader::STREAM_END);
			existingContent.setSize( existingFile.getStreamReader()->tell() );
			existingFile.getStreamReader()->seek(0, hkStreamReader::STREAM_SET);
			existingFile.read( existingContent.begin(), existingContent.getSize() );
		}
	}
	hkOfstream* outFile = new hkOfstream(filename);
	outFile->write( existingContent.begin(), existingContent.getSize() );
	return outFile;
}


static void hkOutputStatsSummaryToFile( const char* filename, const hkObjectArray<BootstrapDemo::StatsRecord>& statsRecords, int numThreads, int numSpus )
{
	// CSV
	{
		hkString csvFilename = hkString(filename) + hkString(".csv");

		hkString oldStats = "";
		hkString newStats = "";

		// Read the existing file into a hkString.
		{
			hkIfstream statsFile( csvFilename.cString() );
			if (statsFile.isOk())
			{
				statsFile.getStreamReader()->seek(0, hkStreamReader::STREAM_END);
				int bufSize = statsFile.getStreamReader()->tell();
				statsFile.getStreamReader()->seek(0, hkStreamReader::STREAM_SET);

				char* statsBuf = hkAllocate<char>( bufSize, HK_MEMORY_CLASS_DEMO );
				statsFile.read( statsBuf, bufSize );
				// Remove unwanted chars that may have crept in from everyday use.
				oldStats = hkString( statsBuf, bufSize ).replace("\r","");
				// Excel has a habit of appening commas to the info string, remove them
				oldStats = oldStats.substr( 0, oldStats.indexOf('\n') ).replace(",","") + oldStats.substr( oldStats.indexOf('\n') );
				hkDeallocate(statsBuf);
			}
		}

		// Remove the help message from the start of the file so that we can easily add new columns, etc.
		hkString csvInfoStr = "NOTE: The numerical values are the average StepDemo() time for each demo in microseconds.\n\n,";
		if( oldStats.substr( 0, csvInfoStr.getLength() ) == csvInfoStr )
		{
			oldStats = oldStats.substr( csvInfoStr.getLength() );
		}

		// Use the first line from the old stats and add version string, platform and number of threads.
		int idx = (oldStats.indexOf('\n') > 0) ? oldStats.indexOf('\n') : oldStats.getLength();
		newStats = oldStats.substr(0, idx) + "," + HAVOK_SDK_VERSION_STRING + " " + BootstrapDemo::getPlatform(true);

		hkString threadInfo;
#if defined(HK_PLATFORM_PS3_PPU)
		threadInfo.printf( " (%d PPU Threads %d SPU Threads)\n", numThreads, numSpus );
#else
		threadInfo.printf( " (%d Threads)\n", numThreads );
#endif
		newStats += threadInfo;

		oldStats = (oldStats.substr(idx).getLength() > 0) ? oldStats.substr(idx+1) : "";

		// Determine the number of columns in the csv file.
		int numCols = 0;
		{
			for( int i = 0; i != -1; )
			{
				i = newStats.indexOf(',', i);
				if( i >= 0 )
				{
					numCols++;
					i++;
				}
			}
		}

		// Add the stats.
		{
			for (int i=0; i < statsRecords.getSize(); i++)
			{
				int splitIndex = statsRecords[i].m_demoName.lastIndexOf('/');
				hkString curDemo = statsRecords[i].m_demoName.substr(0, splitIndex) + hkString(",\"") + statsRecords[i].m_demoName.substr(splitIndex+1) + hkString("\"");

				// Add any lines from the original file that come before the current stats demo.
				if( oldStats.getLength() > 0 )
				{
					while( hkString::strCmp( oldStats.cString(), curDemo.cString() ) < 0 )
					{
						idx = (oldStats.indexOf('\n') > 0) ? oldStats.indexOf('\n') : oldStats.getLength();
						newStats += oldStats.substr(0, idx) + ",\n";
						oldStats = (oldStats.substr(idx).getLength() > 0) ? oldStats.substr(idx+1) : "";
					}
				}

				// Add a line for the current stats demo, including old stats if present.
				if( oldStats.beginsWith( curDemo ) )
				{
					idx = (oldStats.indexOf('\n') > 0) ? oldStats.indexOf('\n') : oldStats.getLength();
					curDemo = oldStats.substr(0, idx);
					oldStats = (oldStats.substr(idx).getLength() > 0) ? oldStats.substr(idx+1) : "";
				}
				else
				{
					for( int j = 0; j < numCols-1; j++ )
					{
						curDemo += ",";
					}
				}
				hkString curLine;
				curLine.printf( "%s,%f\n", curDemo.cString(), statsRecords[i].m_avgStepDemoTime );
				newStats += curLine;
			}

			// Add any remaining lines from the original file.
			while( oldStats.getLength() > 0 )
			{
				idx = (oldStats.indexOf('\n') > 0) ? oldStats.indexOf('\n') : oldStats.getLength();
				newStats += oldStats.substr(0, idx) + ",\n";
				oldStats = (oldStats.substr(idx).getLength() > 0) ? oldStats.substr(idx+1) : "";
			}
		}

		// Add the help string to the start of the file.
		hkString csvFileContents = csvInfoStr + newStats;

		// Write out the new csv stats file.
		hkOfstream* newCsvFile = new hkOfstream( csvFilename.cString() );
		newCsvFile->write( csvFileContents.cString(), csvFileContents.getLength() );
		delete newCsvFile;
	}

	// HTML
	{
		hkString htmlFilename = hkString(filename) + hkString(".html");
		hkOfstream* htmlFile = createAndAppend( htmlFilename.cString() );

		hkString terminator("\n</body></html>");
		htmlFile->getStreamWriter()->seek(0, hkStreamWriter::STREAM_END);

		if (htmlFile->getStreamWriter()->tell() < terminator.getLength())
		{
			// New Empty File
			htmlFile->printf("<html>\n"
							"\t<link href=\"stats.css\" rel=\"stylesheet\" type=\"text/css\">\n"
							"\t<body>\n");
			htmlFile->printf(terminator.cString());
		}

		int eofPos = htmlFile->getStreamWriter()->tell();
		htmlFile->getStreamWriter()->seek( eofPos-terminator.getLength(), hkStreamWriter::STREAM_SET);

		htmlFile->printf("<p>Havok Version: %s</p>\n", HAVOK_SDK_VERSION_STRING);
		htmlFile->printf("<p>Platform: %s</p>\n", BootstrapDemo::getPlatform());
#if defined(HK_PLATFORM_PS3_PPU)
		htmlFile->printf("<p>PPU Threads: %d</p>\n", numThreads );
		htmlFile->printf("<p>SPU Threads: %d</p>\n", numSpus );
#else
		htmlFile->printf("<p>Threads: %d</p>\n", numThreads );
#endif
		htmlFile->printf("<table>\n<tr><td><p>Demo Path</p></td><td><p>Demo Name</p></td><td><p>Average StepDemo() time - microseconds</p></td></tr>\n");
		for (int i=0; i < statsRecords.getSize(); i++)
		{
			int splitIndex = statsRecords[i].m_demoName.lastIndexOf('/');

			htmlFile->printf("<tr> <td><p>%s</p></td> <td><p>%s</p></td> <td align=right><p>%7.3f</p></td></tr>\n",
				statsRecords[i].m_demoName.substr(0, splitIndex).cString(),
				statsRecords[i].m_demoName.substr(splitIndex+1).cString(),
				statsRecords[i].m_avgStepDemoTime );
		}
		htmlFile->printf("</table>\n");
		htmlFile->printf("<hr/>\n%s", terminator.cString());

		delete htmlFile;
	}
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


HK_DECLARE_DEMO_VARIANT_USING_STRUCT( BootstrapDemo, HK_DEMO_TYPE_OTHER, BootstrapVariant, g_variants, "Test bootstrap [Hold \222\\\223 and press \231 to continue from last\\next demo]" );

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
