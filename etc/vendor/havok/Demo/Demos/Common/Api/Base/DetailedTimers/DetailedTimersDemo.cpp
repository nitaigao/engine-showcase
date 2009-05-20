/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Demos/DemoCommon/DemoFramework/hkDoubleConvCheck.h>

#include <Common/Base/Monitor/MonitorStreamAnalyzer/hkMonitorStreamAnalyzer.h>
#include <Common/Base/System/Stopwatch/hkStopwatch.h>



#ifdef HK_PS2
#include <libpc.h> //for the SCE_PC_CTE timings
#endif

#define TEST_DEMO_NAME "MoppCodeStreaming"
#define TEST_DEMO_PATH "Physics/Api/Collide/OfflineGeneration/"
#define TEST_DEMO_FULLPATH (TEST_DEMO_PATH TEST_DEMO_NAME)
#define FILE_NAME TEST_DEMO_NAME "_stats.txt"

// The purpose of this example is to show how to get the most detailed timer information
// possible from havok.
class DetailedTimersDemo : public hkDemo
{

	public:
		DetailedTimersDemo(hkDemoEnvironment* env);
		~DetailedTimersDemo();

		Result stepDemo();

		// time a single demo for several iterations and store the results
		void timeDemo( hkDemo* demo, int iterations, const char* fileName );

		virtual Result stepVisualDebugger() { return DEMO_OK; }
		virtual bool visualDebuggerEnabled()  { return false; }
		void makeFakeInput() {}


	protected:
		int m_step;
};

	//
	// Constructor
	//
DetailedTimersDemo::DetailedTimersDemo(hkDemoEnvironment* environment)
	:	hkDemo(environment)
{
	m_step = 0;
}

	//
	// Destructor
	//
DetailedTimersDemo::~DetailedTimersDemo()
{
}

#include <Common/Base/Types/Color/hkColor.h>

void DetailedTimersDemo::timeDemo( hkDemo* demo, int iterations, const char* fileName )
{

	//
	//	Time the demo
	//
	hkMonitorStreamFrameInfo frameInfo;
#	ifdef HK_PS2
		frameInfo.m_indexOfTimer0 = 1;
		frameInfo.m_indexOfTimer1 = 0;
			// Assume that each instruction cache miss cost around 35 cycles.
			// So we can see how much of our overall time is spent waiting for the
			// memory system
		frameInfo.m_timerFactor0 = 35.0f / 300.f;
		frameInfo.m_timerFactor1 = 1.0f / 300.f;
		frameInfo.m_heading = "usec: IcachePenalty (assuming 35 cycle penalty)";
#	else
		frameInfo.m_indexOfTimer0 = 0;
		frameInfo.m_indexOfTimer1 = -1;
		frameInfo.m_heading = "usecs";
		frameInfo.m_timerFactor0 = 1e6f / hkReal(hkStopwatch::getTicksPerSecond());
#	endif

	int demoIdx = hkDemoDatabase::getInstance().findDemo( TEST_DEMO_FULLPATH );
	HK_ASSERT2(0x654e432e, demoIdx != -1, "Demo does not exist" );

	const bool isPhysicsDemo = (HK_DEMO_TYPE_PHYSICS == hkDemoDatabase::getInstance().getDemos()[ demoIdx ].m_demoTypeFlags);


	//
	//	Setup some memory to store all the timer information 
	//  for all frames
	//
	int numThreads = 1;

	if( isPhysicsDemo )
	{
		hkCpuJobThreadPool* mtUtil = (hkCpuJobThreadPool*)static_cast<hkDefaultPhysicsDemo*>(demo)->m_jobThreadPool;
		if (mtUtil)
		{
			numThreads += mtUtil->getNumThreads();
		}
	}

	hkMonitorStreamAnalyzer streamAnalyzer( 10000000, numThreads );

	for (int i = 0; i < iterations; i++ )
	{

		//
		//	Setup the timerinfo and memory on a per frame bases
		//
		hkMonitorStream& stream = hkMonitorStream::getInstance();
		stream.resize( 2 * 1024 * 1024  );	// 2 meg for timer info per frame
		stream.reset();

		//
		//	Start timers
		//
#		ifdef HK_PS2
			scePcStart( SCE_PC_CTE | SCE_PC_U0 | SCE_PC_U1 | SCE_PC0_ICACHE_MISS | SCE_PC1_CPU_CYCLE, 0 ,0 );
#		endif

		//
		//	Step the demo
		//
		demo->stepDemo();

		//
		//	Stop timers. This is necessary as a timer overflow on PlayStation(R)2 causes an exception
		//
#		ifdef HK_PS2
		scePcStop() ;
#		endif

		//
		//	Analyze the per frame info and copy the data over to the multi frame buffer
		//

		if( numThreads > 1 )
		{
			hkCpuJobThreadPool* mtUtil = (hkCpuJobThreadPool*)static_cast<hkDefaultPhysicsDemo*>(demo)->m_jobThreadPool;

			// Loop through each thread. Capture the frame details from the local
			// stream analyzer in each thread.
			for (int t = 0; t < mtUtil->getNumThreads(); ++t)
			{
				hkCpuJobThreadPool::WorkerThreadData& data = mtUtil->m_workerThreads[t];

				if (data.m_monitorStreamBegin != data.m_monitorStreamEnd )
				{
					frameInfo.m_threadId = t + 1;
					streamAnalyzer.captureFrameDetails( data.m_monitorStreamBegin,data.m_monitorStreamEnd, frameInfo );
				}
			}
		}
		frameInfo.m_threadId = 0;
		streamAnalyzer.captureFrameDetails(stream.getStart(), stream.getEnd(), frameInfo );
	}

	//
	// Write the results to a file
	//
	{
		// Disable double conversion check - we know this will fail
		pushDoubleConversionCheck(false);
		hkReferencedObject::lockAll();
		
		hkOstream ostr (fileName);
		ostr << TEST_DEMO_NAME "   Timers: \n";

		streamAnalyzer.writeStatistics( ostr );
		hkReferencedObject::unlockAll();
		
		popDoubleConversionCheck();
	}

}


//
// stepDemo
//
hkDemo::Result DetailedTimersDemo::stepDemo()
{
	m_step++;

	if ( m_step > 2 )
	{
		m_env->m_textDisplay->outputText("finished", 20, 250, 0xffffffff);
	}

	//
	//	Time demo at step 2
	//
	if ( m_step == 2 )
	{
		//
		//	Run our test
		//
		hkDemo* testDemo;

		hkReferencedObject::lockAll();
		testDemo = hkDemoDatabase::getInstance().createDemo( TEST_DEMO_FULLPATH, m_env );
		testDemo->postConstruct();
		hkReferencedObject::unlockAll();


		//
		//	Now do our timings
		//
		timeDemo( testDemo, 100, FILE_NAME );

		hkReferencedObject::setLockMode( hkReferencedObject::LOCK_MODE_AUTO );
		testDemo->preDeleteDemo();
		delete testDemo;
		hkReferencedObject::setLockMode( hkReferencedObject::LOCK_MODE_NONE );
	}
	return DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo shows how to initialize the Havok Timers properly. It also shows " \
"how to use our monitor stream utility to analyze the time information and dump " \
"a very detailed frame by frame timer analysis to a file. This is intended " \
"as a source code example.\n" \
"This demo simply times the " TEST_DEMO_NAME " demo " \
"and writes the results into the file " FILE_NAME;

HK_DECLARE_DEMO(DetailedTimersDemo, HK_DEMO_TYPE_PRIME, "Time another demo and write results to a file", helpString);

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
