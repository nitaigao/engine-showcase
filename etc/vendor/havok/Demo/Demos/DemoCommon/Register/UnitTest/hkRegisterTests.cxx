/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Common/Base/Config/hkConfigVersion.h>
#include <Demos/DemoCommon/DemoFramework/hkTestDemo.h>

#if defined( HK_COMPILER_MWERKS ) && (HAVOK_BUILD_NUMBER != 0)

static const struct hkTestEntry* havokTests[] = 
{
	HK_NULL
};

#else

#define HK_TEST_DECLARE(func) extern hkTestEntry register ## func;
#include HK_UNIT_TEST_LIST
#undef HK_TEST_DECLARE
static const struct hkTestEntry* havokTests[] = 
{
#	define HK_TEST_DECLARE(func) &register ## func,
#	include HK_UNIT_TEST_LIST
	HK_NULL
};

#endif

static hkDemo* HK_CALL hkCreateUnitTestDemo( hkDemoEnvironment* env )
{
	return new TestDemo(env);
}
static hkDemo* HK_CALL hkCreateUnitTestDemoWithAutoRecovery( hkDemoEnvironment* env )
{
	return new TestDemo(env, true);
}

void HK_CALL registerUnitTests( hkArray<hkString*>& extraNames, hkArray<hkDemoEntryRegister*>& allocedDemos)
{
	// extraDemos keeps track of all extra items which are registered at the end.
	hkArray<hkDemoEntryRegister*> extraDemos;

	//
	// Go through the test database and add new demos for each test
	// These ones are dynamically allocated
	//
	{
		int ti = 0;
		const hkTestEntry* testEntry = havokTests[0];
		while( testEntry )
		{
			hkString path(testEntry->m_path);
			path = path.replace("\\","/");
			path = path.replace("./UnitTest/","");
            path += "/";

			hkString* l_path = new hkString(path);
			extraNames.pushBack(l_path);

			hkDemoEntryRegister* registeredDemo = new hkDemoEntryRegister(
				hkCreateUnitTestDemo, HK_DEMO_TYPE_TEST, l_path->cString(), -1, HK_NULL, "", "", true);
			extraDemos.pushBack(registeredDemo);
			allocedDemos.pushBack(registeredDemo);

			testEntry = havokTests[++ti];
		}
	}
	//
	//	Extract the categories from each test game and make them a new game as well
	//
	{
		if( extraNames.getSize() != 0 )
		{
			// Push back each category as a separate game too
			hkObjectArray<hkString> categories;
			for ( hkTestEntry* testEntry = hkUnitTestDatabase; testEntry; testEntry = testEntry->m_next )
			{
				if (categories.indexOf(testEntry->m_category) == -1)
				{
					categories.pushBack(testEntry->m_category);
				}
			}


			hkString prefix = "./UnitTest/";

			for (int i=0; i < categories.getSize(); i++)
			{
				hkString* l_path = new hkString();
				l_path->printf("%s%s/", prefix.cString(), categories[i].cString() );

				hkDemoEntryRegister* registeredDemo = new hkDemoEntryRegister( 
					hkCreateUnitTestDemoWithAutoRecovery, HK_DEMO_TYPE_TEST, l_path->cString(), -1, HK_NULL, "Run Test", "");
				extraDemos.pushBack(registeredDemo);
				allocedDemos.pushBack( registeredDemo );
				extraNames.pushBack(l_path);
			}

			// Special case 'All tests'
			hkString* l_path = new hkString();
			l_path->printf("%sAll Tests/", prefix.cString() );
			extraNames.pushBack(l_path);
			hkDemoEntryRegister* registeredDemo = new hkDemoEntryRegister(
				hkCreateUnitTestDemoWithAutoRecovery, HK_DEMO_TYPE_TEST, l_path->cString(), -1, HK_NULL, "Run All Unit Tests", "");
			extraDemos.pushBack(registeredDemo);
			allocedDemos.pushBack( registeredDemo );
		}
	}
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
