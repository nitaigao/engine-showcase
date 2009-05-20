/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef DEMOS_UTILITIES_TESTPLANDEMO_H
#define DEMOS_UTILITIES_TESTPLANDEMO_H

#include <Demos/demos.h>

class TestPlanDemo : public hkDemo
{
	public:

		TestPlanDemo(hkDemoEnvironment* env);
		~TestPlanDemo();

		Result stepDemo();
		Result stepVisualDebugger(){ return DEMO_OK; }
		bool visualDebuggerEnabled() { return false; }
		void makeFakeInput() {}

		// Test plan type
		enum TestPlanType
		{
			TEST_PLAN_ALL,
			TEST_PLAN_COMMON_DEMOS,
			TEST_PLAN_PHYSICS_DEMOS,
			TEST_PLAN_ANIMATION_DEMOS,
			TEST_PLAN_BEHAVIOR_DEMOS,
			TEST_PLAN_SERIALIZE_DEMOS,
			TEST_PLAN_UNIT_TESTS,
			TEST_PLAN_VISUAL_TESTS,
		};

	private:
		char* m_testPlanFilename;
		TestPlanType m_testPlanType;

		int m_step;
};

#endif // DEMOS_UTILITIES_TESTPLANDEMO_H

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
