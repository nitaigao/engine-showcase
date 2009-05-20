/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEMO_PERFORMANCE_COUNTER_UTILITY_H
#define HK_DEMO_PERFORMANCE_COUNTER_UTILITY_H

#include <Common/Base/Monitor/MonitorStreamAnalyzer/hkMonitorStreamAnalyzer.h>

class hkPerformanceCounterUtility
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO_FRAMEWORK, hkPerformanceCounterUtility);

			/// An enumeration of possible countersources.
			/// This is currently only implemented for PlayStation(R)2 and Xbox360
		enum CounterSelect
		{
				/// disabled
			DISABLE = 0,
			ICACHE_MISSES = 1,
			DCACHE_MISSES = 2,
			IDCACHE_MISSES = 3,
			ICACHE_MISSES_PENALTY = 4,
			DCACHE_MISSES_PENALTY = 8,

				/// Alternate I and D cache misses
			IDCACHE_MISSES_PENALTY = 12,
			LOAD_HIT_STORE = 16, // Xbox360 only
			LOAD_HIT_STORE_PENALTY = 32, // Xbox360 only, in usecs
			INSTRUCTIONS_FLUSHED = 64,   // Xbox360 only, can't time this and ICache at same time (use the same analyzer slot in this impl)
			COUNTER_LD_MAX = 6	// log dual from max counter
		};

		hkPerformanceCounterUtility();
		~hkPerformanceCounterUtility();

			/// enables/disables the counter utility
		void enable( CounterSelect select = IDCACHE_MISSES_PENALTY );

		bool isEnabled() const;

			/// start the counter
		void startCounter();

			/// stop the counter and capture the default hkMonitorStream
		void stopCounter();

			/// capture additional streams (e.g. when running in multithreaded mode)
		void captureExternalFrame( const char* monitorStreamBegin, const char* monitorStreamEnd, int threadId );

			/// Sets the demo name. This name will simply appear in the output text file, so
			/// you can identify the demo later on more easily
		void setDemoName( const char* name );

			/// Call this if you are finished with the demo and want to safe all
			/// accumulated statistics to a file.
			/// Also this function calls reset
		void saveFileAndReset();

			/// clear all internal data
		void resetNumThreads( int numThreads, int numSpuThreads );

	protected:
		CounterSelect	m_counterSelect;
		int				m_currentSelect;

		hkString m_demoName;
		hkString m_timerHeading;
		hkMonitorStreamAnalyzer* m_streamUtility;
		hkMonitorStreamFrameInfo m_frameInfo;

};

#endif // HK_DEMO_PERFORMANCE_COUNTER_UTILITY_H

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
