/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/DemoCommon/DemoFramework/hkDemo.h>
#include <Demos/demos.h>
#include <Demos/DemoCommon/DemoFramework/hkDemo.h>
#include <Demos/DemoCommon/DemoFramework/hkPerformanceCounterUtility.h>
#include <Common/Base/System/Io/FileSystem/hkFileSystem.h>
#include <Common/Base/System/Stopwatch/hkStopwatch.h>

#include <Common/Base/Types/Color/hkColor.h>

#include <Common/Base/Monitor/hkMonitorStream.h>
#if defined(HK_XBOX_USE_PERFLIB) && defined(HK_PLATFORM_XBOX360)
#pragma warning(disable: 4201) //XXX or else you get about 10 warnings about namesless structs from the pcm header
#include <pmcpbsetup.h> // you must link to libpmcpb.lib for these functions
#include <pmcpb.h> 
#endif


#ifdef HK_PS2
#include <libpc.h> //for the SCE_PC_CTE timings
#endif


hkPerformanceCounterUtility::hkPerformanceCounterUtility()
{
	m_counterSelect = DISABLE;
	m_currentSelect = 0;
	m_demoName = "unknown";

#if defined(HK_XBOX_USE_PERFLIB) && defined(HK_PLATFORM_XBOX360)
	PMCInstallSetup(&PMCDefaultSetups[PMC_SETUP_OVERVIEW_PB0T0]);    
	//PMCInstallSetup(&PMCDefaultSetups[PMC_SETUP_VMXMIX_PB0T0]);    
	hkSetXbox360PerfReg(6);// LHS

#endif
}

hkPerformanceCounterUtility::~hkPerformanceCounterUtility()
{
	if ( m_counterSelect == DISABLE )
	{
		return;
	}
	delete m_streamUtility;
}

void hkPerformanceCounterUtility::enable( CounterSelect select )
{
#ifdef HK_PS2
	m_streamUtility = new hkMonitorStreamAnalyzer( 6000000  );
#else
	m_streamUtility = new hkMonitorStreamAnalyzer( 10000000 );
#endif
	
	m_streamUtility->m_nodeIdForFrameOverview = "Physics";

	m_counterSelect = select;	
	m_currentSelect = 0;
	setDemoName( m_demoName.cString() );
}

void hkPerformanceCounterUtility::startCounter()
{

	if ( m_counterSelect == DISABLE )
	{
		return;
	}

#ifdef HK_PS2
	m_frameInfo.m_timerFactor0 = 1.f/300.0f;
	m_frameInfo.m_timerFactor1 = 1.f/300.0f;

	while(1)
	{
		m_currentSelect--;
		if ( m_currentSelect<=0 )
		{
			m_currentSelect = COUNTER_LD_MAX-1;
		}
		int counter = 1<<m_currentSelect;
		if ( !(counter & m_counterSelect) )
		{
			continue;
		}

		switch (counter)
		{
			case ICACHE_MISSES_PENALTY:		m_frameInfo.m_heading = "CPU(usec) icache(usec)";			break;
			case ICACHE_MISSES:				m_frameInfo.m_heading = "CPU(usec) icache(count)";			break;
			case DCACHE_MISSES_PENALTY:		m_frameInfo.m_heading = "CPU(usec) dcache(usec)";			break;
			case DCACHE_MISSES:				m_frameInfo.m_heading = "CPU(usec) dcache(count)";			break;

			default:					m_frameInfo.m_heading = "Unknown";							break;
		}

		switch ( counter )
		{
			case ICACHE_MISSES_PENALTY:
							m_frameInfo.m_timerFactor0 *= 30;
			case ICACHE_MISSES:
							m_frameInfo.m_indexOfTimer0 = 1;
							m_frameInfo.m_indexOfTimer1 = 0;
					scePcStart( SCE_PC_CTE | SCE_PC_U0 | SCE_PC_U1 | SCE_PC0_ICACHE_MISS | SCE_PC1_CPU_CYCLE, 0 ,0 );
					break;

					case DCACHE_MISSES_PENALTY:
							m_frameInfo.m_timerFactor1 *= 30;
			case DCACHE_MISSES:
							m_frameInfo.m_indexOfTimer0 = 0;
							m_frameInfo.m_indexOfTimer1 = 2;
					scePcStart( SCE_PC_CTE | SCE_PC_U0 | SCE_PC_U1 | SCE_PC1_DCACHE_MISS | SCE_PC0_CPU_CYCLE, 0 ,0 );
					break;
		}
		break;
	}

#elif defined(HK_XBOX_USE_PERFLIB) && defined(HK_PLATFORM_XBOX360)
	
	m_frameInfo.m_timerFactor0 = 1e6f / float(hkStopwatch::getTicksPerSecond());
	m_frameInfo.m_timerFactor1 = 1;
	m_frameInfo.m_indexOfTimer0 = 0;

	while(1)
	{
		m_currentSelect--;
		if ( m_currentSelect<=0 )
		{
			m_currentSelect = COUNTER_LD_MAX-1;
		}
		int counter = 1<<m_currentSelect;
		if ( !(counter & m_counterSelect) )
		{
			continue;
		}

		switch (counter)
		{
			case ICACHE_MISSES_PENALTY:		
			case ICACHE_MISSES:				m_frameInfo.m_heading = "CPU(usec) icache(cycles)";			break;
			case DCACHE_MISSES_PENALTY:		
			case DCACHE_MISSES:				m_frameInfo.m_heading = "CPU(usec) dcache(misses)";			break;
			case LOAD_HIT_STORE:			m_frameInfo.m_heading = "CPU(usec) loadhitstore(hits)";	break;
			case LOAD_HIT_STORE_PENALTY:	m_frameInfo.m_heading = "CPU(usec) loadhitstore(usecs)";	break;
			case INSTRUCTIONS_FLUSHED:		m_frameInfo.m_heading = "CPU(usec) iflushed(num)";		break;
			default:						m_frameInfo.m_heading = "Unknown";							break;
		}

			/*
			[ 0] CPB_IU_PFM_EVENT_IL1_MISS_CYCLES (.. L1 Cache Miss)
			[ 1] EPB_IU_PFM_EVENT_INSTR_FLUSHED
			[ 2] CPB_IU_PFM_EVENT_INSTR_STALL 
			[ 3] CPB_IU_PFM_EVENT_INSTR_DEP_STALL
			[ 4] CPB_PFM_EVENT_T0_PPC_COMMIT (.. Instructions computed, needs * 2 to get actual count)
			[ 5] SPB_XU_PFM_EVENT_LOAD_MISS
			[ 6] SPB_XU_PFM_EVENT_LOAD_MISS_LHS  (.. Load-Hit-Store)
			[ 7] SPB_XU_PFM_EVENT_DERAT_MISS
			[ 8] EL2 P2 d miss(D/3-4)
			[ 9] EL2 P0 d hit(D/3-5)
			[10] EL2 P1 d hit(D/3-6)
			[11] EL2 P2 d hit(D/3-7)
			[12] CAll RC busy(D/3-0)
			[13] CRC disp due to dload,iload,dcbt,st(D/3-1)
			[14] EL2 P0 d miss(D/3-2)  (.. L2 DCache Miss)
			[15] EL2 P1 d miss(D/3-3)
			*/

		switch ( counter )
		{
		case ICACHE_MISSES_PENALTY:
		case ICACHE_MISSES:
			m_frameInfo.m_indexOfTimer1 = 1;
			hkSetXbox360PerfReg(0);
			break;

		case DCACHE_MISSES_PENALTY:
		case DCACHE_MISSES:
			m_frameInfo.m_indexOfTimer1 = 2;
			hkSetXbox360PerfReg(14);
			break;

		case LOAD_HIT_STORE_PENALTY:
			m_frameInfo.m_timerFactor1 = m_frameInfo.m_timerFactor0;
		case LOAD_HIT_STORE:	
			m_frameInfo.m_indexOfTimer1 = 3;
			hkSetXbox360PerfReg(6);
			break;

		case INSTRUCTIONS_FLUSHED:
			m_frameInfo.m_indexOfTimer1 = 1;
			hkSetXbox360PerfReg(1);
			break;

		default: break;
		}

		break;
	}

	PMCResetCounters();
	PMCStart();

#else
	m_frameInfo.m_heading = "Timer values are usecs";
	m_frameInfo.m_indexOfTimer0 = 0;
	m_frameInfo.m_indexOfTimer1 = 1;
	//hkReal test = hkStopwatch::getTicksPerSecond();
	m_frameInfo.m_timerFactor0 = 1e6f / float(hkStopwatch::getTicksPerSecond());
	m_frameInfo.m_timerFactor1 = 1.0f;
#endif
}

void hkPerformanceCounterUtility::stopCounter()
{
	if ( m_counterSelect == DISABLE )
	{
		return;
	}

#ifdef HK_PS2
	scePcStop() ;
#elif defined(HK_XBOX_USE_PERFLIB) && defined(HK_PLATFORM_XBOX360)
	PMCStop();
#endif
}

void hkPerformanceCounterUtility::captureExternalFrame( const char* monitorStreamBegin, const char* monitorStreamEnd, int threadId )
{
	if ( m_counterSelect == DISABLE )
	{
		return;
	}
	m_frameInfo.m_threadId = threadId;
	m_streamUtility->captureFrameDetails( monitorStreamBegin, monitorStreamEnd, m_frameInfo);
}

void hkPerformanceCounterUtility::setDemoName( const char* name )
{
	if ( m_counterSelect == DISABLE )
	{
		return;
	}

	m_demoName = name;
}

void hkPerformanceCounterUtility::resetNumThreads( int numThreads, int numSpuThreads )
{
	if ( m_counterSelect == DISABLE )
	{
		return;
	}

	m_streamUtility->resetNumThreads( numThreads, numSpuThreads  );
}

void hkPerformanceCounterUtility::saveFileAndReset()
{
	if ( m_counterSelect == DISABLE )
	{
		return;
	}
	
	const char* type = "pc";
	const char* prefix = "";

#if defined(HK_PS2) 
	switch ( m_counterSelect )
	{
		case ICACHE_MISSES_PENALTY:
			type = "ps2_icache_pen";
			break;
		case ICACHE_MISSES:
			type = "ps2_icache_miss";
			break;
		case DCACHE_MISSES:
			type = "ps2_dcache_miss";
			break;
		case DCACHE_MISSES_PENALTY:
			type = "ps2_dcache_pen";
			break;
		default:
			type = "ps2";
	}
#elif defined(HK_XBOX_USE_PERFLIB) && defined(HK_PLATFORM_XBOX360)
	switch ( m_counterSelect )
	{
		case ICACHE_MISSES_PENALTY:
		case ICACHE_MISSES:
			type = "xbox360_icache";
			break;
		case DCACHE_MISSES:
		case DCACHE_MISSES_PENALTY:
			type = "xbox360_dcache";
			break;
		case LOAD_HIT_STORE_PENALTY:
		case LOAD_HIT_STORE:
			type = "xbox360_loadhitstore";
			break;
		case INSTRUCTIONS_FLUSHED:
			type = "xbox360_iflushed";
			break;
		default:
			type = "xbox360";
			break;
	}
#endif

	hkString name; name.printf("Profile%s_%s.txt", prefix, type);
	hkOfstream ostr(name.cString());

#if defined(HK_XBOX_USE_PERFLIB) && defined(HK_PLATFORM_XBOX360)
	PMCState pmcstate;
	PMCGetCounters(&pmcstate);
	PMCDumpCountersVerbose(&pmcstate, PMC_VERBOSE_MAX);
#endif

	ostr.printf("\nDemoName: %s**\n", m_demoName.cString());
	ostr.printf("============================\n\n");


	m_streamUtility->writeStatistics( ostr );


	// Currently the picture output does not work for the PLAYSTATION(R)3 because the absolute time values
	// between SPUs and PPU threads are not synchronized.
#ifndef HK_PLATFORM_PS3_PPU
	{
		hkMonitorStreamColorTable colorTable;
		colorTable.setupDefaultColorTable();
		
		hkMonitorStreamAnalyzer::ThreadDrawInput input;
		input.m_heightPerThread = 3;
		input.m_gapBetweenThreads = 2;
		input.m_gapBetweenFrames = 10;
		input.m_outputPixelWidth = 1800;
		input.m_colorTable = &colorTable;
		input.m_frameStart = 1;
		input.m_numFrames  = 50;
		input.m_limitFrameTime = 0;
		input.m_limitStartTime = 0;
		input.m_warnAboutMissingTimers = true;

		hkOstream ostrP("ProfileMultiThreading.tga");
		//m_streamUtility->drawThreadsToTga(input,ostrP);
	}
#endif

	m_streamUtility->reset();
}


bool hkPerformanceCounterUtility::isEnabled() const
{
	return m_counterSelect != DISABLE;
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
