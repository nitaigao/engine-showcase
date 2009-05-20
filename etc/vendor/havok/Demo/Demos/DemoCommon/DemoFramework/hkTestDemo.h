/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_TEST_DEMO_H
#define HK_TEST_DEMO_H

#include <Common/Base/System/Error/hkDefaultError.h>
#include <Demos/DemoCommon/DemoFramework/hkDemoDatabase.h>
#include <Demos/DemoCommon/DemoFramework/hkDefaultDemo.h>
#include <Demos/DemoCommon/DemoFramework/hkDemoConsole.h>

class TestDemoError : public hkDefaultError
{
public:

	TestDemoError();

	// Error Handling
	virtual int message(Message msg, int id, const char* description, const char* file, int line);

	class TestDemo* m_owner;
};

class TestDemo : public hkDefaultDemo
{
public:

	HK_DECLARE_CLASS_ALLOCATOR_UNCHECKED(HK_MEMORY_CLASS_DEMO);

	TestDemo(hkDemoEnvironment* env, hkBool autoRecoverFromErrors=false );

	~TestDemo();

	Result stepDemo();

	enum FailReason
	{
		FAIL_NORMAL,
		FAIL_ASSERT,
		FAIL_ERROR,
		FAIL_MAX
	};

	void testRun(hkTestEntry* e);
	void failCurrentTest(FailReason f);
	void readFileAndAppend(hkString outPut);

	/// now a member to get rid of the multiple inheritance
	TestDemoError m_error;

	hkString m_testString;
	hkString m_passString;
	hkString m_failString;

private:

	hkArray<hkTestEntry*> m_tests;
	int m_currentTest;
	hkBool m_currentTestOk;
	hkBool m_autoRecoverFromErrors;
	int m_firstLineShown;
	int m_numPasses;
	int m_numFails;	
	hkString m_TotalOutput;
	hkString m_unitTestFileName;
	
};


#endif // HK_TEST_DEMO_H

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
