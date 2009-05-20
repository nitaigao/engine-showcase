/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef DEMOS_UTILITIES_BOOTSTRAPDEMO_H
#define DEMOS_UTILITIES_BOOTSTRAPDEMO_H

#include <Demos/demos.h>
#include <Demos/DemoCommon/DemoFramework/hkDemoDatabase.h>
#include <Common/Serialize/Packfile/hkPackfileReader.h>

class hkMonitorStreamAnalyzer;

// Havok Bootstrapper
class BootstrapDemo : public hkDemo
{
	public:

		BootstrapDemo(hkDemoEnvironment* env);
		~BootstrapDemo();

		void makeFakeInput() {}

		virtual void preRenderDisplayWorld(class hkgViewport* v);
		virtual void postRenderDisplayWorld(class hkgViewport* v);

		virtual Result stepDemo();
		virtual Result stepVisualDebugger();
		virtual bool visualDebuggerEnabled();


		// Bootstrap test type
		enum TestType
		{
			TEST_NORMAL,
			TEST_FPU_EXCEPTIONS,
			TEST_SERIALIZE_BINARY,
			TEST_SERIALIZE_XML,
			TEST_DETERMINISM,
			TEST_MULTITHREADING_DETERMINISM,
			TEST_STATISTICS,
			TEST_STATISTICS_DETAILED,
			TEST_STATISTICS_SINGLE_THREADED,
			TEST_STATISTICS_CRITICAL,
		};

		//// Tweakable parameters
		//struct Options
		//{
		//	HK_DECLARE_REFLECTION();
		//	Options() { m_stepsPerDemo = 100; }
		//	//int m_expireDemo,
		//	int m_stepsPerDemo; //+default(100) +absmin(1) +absmax(500)
		//	//int m_timePerDemo,
		//};
		//DEMO_OPTIONS_DECLARE(Options);

		// For stats summaries
		struct StatsRecord
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO, StatsRecord );

			hkString m_demoName;
			hkReal	 m_avgStepDemoTime;
			bool operator < (const StatsRecord& other) const { return hkString::strCmp( m_demoName.cString(), other.m_demoName.cString() ) < 0 ? true : false; }
		};

		static char* getPlatform(hkBool simple = false);

	private:

		// Submethods
		void NormalStepDemo();
		void SerializeStepDemo();
		void DeterminismStepDemo();
		void StatsStepDemo();

		// Common
		int m_demoTypeMask;
		TestType m_testType;

		hkObjectArray< StatsRecord > m_statsRecords;

		struct DemoEntry
		{
			const hkDemoEntry* m_entry;
			//int m_numIterations;
		};

		hkArray<DemoEntry> m_entries;
		hkDemo* m_demo;
		int m_demoIndex;
		int m_steps;

		int m_numThreads;

		// Serialization
		hkPackfileReader::AllocatedData* m_allocatedData;
		hkpWorld* m_originalWorld;
		int m_counter;


		// Stats
		hkMonitorStreamAnalyzer* m_streamAnalyzer;
		hkReal m_statsTotalTime;
		int m_statsNumSamples;

		// Determinism
		int m_runsPerGame;
		int m_runIndex;

		hkBool m_forceMultithreadedSimulationBackup;
};

#endif // DEMOS_UTILITIES_BOOTSTRAPDEMO_H

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
