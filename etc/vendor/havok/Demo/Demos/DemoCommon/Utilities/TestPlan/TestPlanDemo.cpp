/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/TestPlan/TestPlanDemo.h>
#include <Common/Base/Config/hkConfigVersion.h>

struct TestPlanVariant
{
	char* m_name;
	TestPlanDemo::TestPlanType m_testPlanType;
	const char* m_details;
};

static const TestPlanVariant g_variants[] =
{
	{ "AllDemosAndTests",   TestPlanDemo::TEST_PLAN_ALL,             "Generates a test plan spreadsheet for all the demos and tests."},
	{ "CommonDemos",        TestPlanDemo::TEST_PLAN_COMMON_DEMOS,    "Generates a test plan spreadsheet for the common demos."},
	{ "PhysicsDemos",       TestPlanDemo::TEST_PLAN_PHYSICS_DEMOS,   "Generates a test plan spreadsheet for the physics demos."},
	{ "AnimationDemos",     TestPlanDemo::TEST_PLAN_ANIMATION_DEMOS, "Generates a test plan spreadsheet for the animation demos."},
	{ "BehaviorDemos",      TestPlanDemo::TEST_PLAN_BEHAVIOR_DEMOS,  "Generates a test plan spreadsheet for the behavior demos."},
	{ "SerializationDemos", TestPlanDemo::TEST_PLAN_SERIALIZE_DEMOS, "Generates a test plan spreadsheet for the serialization demos."},
	{ "UnitTests",          TestPlanDemo::TEST_PLAN_UNIT_TESTS,      "Generates a test plan spreadsheet for the unit tests."},
	{ "VisualTests",        TestPlanDemo::TEST_PLAN_VISUAL_TESTS,    "Generates a test plan spreadsheet for the visual tests."},
};

const char* demoCols     = "Demo Name,Success,Assert,Crash,Warning,Comments,,Memory Leak,Memory Leak Comment/Information,,Multithreaded Success,Multithreaded Assert,Multithreaded Crash,Multithreaded Comments,,FPU Assert,FPU Crash,FPU Comments,,Bootstrap Serialization Assert,Bootstrap Serialization Crash,Bootstrap Serialization Comments";
const char* unitTestCols = "Unit Test Name, Pass (0), Fail (F), Error (E), Assert (A), Warning (W)";

TestPlanDemo::TestPlanDemo(hkDemoEnvironment* env)
:	hkDemo(env),
	m_step(0)
{
	const TestPlanVariant& variant =  g_variants[env->m_variantId];

	m_testPlanFilename = variant.m_name;
	m_testPlanType = variant.m_testPlanType;
}

hkDemo::Result TestPlanDemo::stepDemo()
{
	hkString testPlanFilename;
	testPlanFilename.printf( "%s_%i_%sReport.csv", HAVOK_SDK_VERSION_STRING, HAVOK_BUILD_NUMBER, m_testPlanFilename );

	if( m_step == 0 )
	{
		hkOfstream ostr( testPlanFilename.cString() );

		if( m_testPlanType == TEST_PLAN_UNIT_TESTS )
		{
			ostr.printf( "%s,\n", unitTestCols );
		}
		else
		{
			ostr.printf( "%s,\n", demoCols );
		}

		//
		// Get a list of the demos and build a test plan
		//
		hkObjectArray<hkString> demoPathItems;
		const hkObjectArray<hkDemoEntry>& db = hkDemoDatabase::getInstance().getDemos();

		for( int i=0; i<db.getSize(); i++ )
		{
			const hkDemoEntry& entry = db[i];
			hkString demoName = entry.m_menuPath.replace(","," -");

			bool unitTest = entry.m_demoTypeFlags & HK_DEMO_TYPE_TEST
				&& demoName.substr( demoName.indexOf('/')+1, 4 ) == "Test" 
				&& demoName.substr( demoName.indexOf('/')+6, 8 ) == "UnitTest";

			// Skip demos based on their location in the demos tree.
			switch (m_testPlanType)
			{
			case TEST_PLAN_COMMON_DEMOS:
				if ( demoName.substr(0,6) != hkString("Common") || unitTest )
				{
					continue;
				}
				break;
			case TEST_PLAN_PHYSICS_DEMOS:
				if ( demoName.substr(0,7) != hkString("Physics") || unitTest )
				{
					continue;
				}
				break;
			case TEST_PLAN_ANIMATION_DEMOS:
				if ( demoName.substr(0,9) != hkString("Animation") || unitTest )
				{
					continue;
				}
				break;
			case TEST_PLAN_BEHAVIOR_DEMOS:
				if ( demoName.substr(0,8) != hkString("Behavior") || unitTest )
				{
					continue;
				}
				break;
			case TEST_PLAN_SERIALIZE_DEMOS:
				if( (entry.m_demoTypeFlags & HK_DEMO_TYPE_SERIALIZE) == 0 )
				{
					continue;
				}
				break;
			case TEST_PLAN_UNIT_TESTS:
				if( !unitTest )
				{
					continue;
				}
				break;
			case TEST_PLAN_VISUAL_TESTS:
				if( demoName.substr( demoName.indexOf('/')+1, 4 ) != "Test"  || unitTest )
				{
					continue;
				}
				break;
			case TEST_PLAN_ALL:
				// Don't skip any demos.
				break;
			default:
				break;
			}

			int offset = 0;
			int depth = 0;

			while( offset != -1 )
			{
				offset = demoName.indexOf('/');

				if( offset == -1 )
				{
					for ( int indent = 0; indent < depth; indent++ )
					{
						ostr.printf( "        " );
					}
					ostr.printf( "%s,\n", demoName.cString() );
				}
				else
				{
					hkString demoPath = demoName.substr(0, offset+1);

					if( depth >= demoPathItems.getSize() || demoPathItems[depth] != demoPath )
					{
						for ( int indent = 0; indent < depth; indent++ )
						{
							ostr.printf( "        " );
						}
						ostr.printf( "%s,\n", demoPath.cString() );

						int itemsToRemove = demoPathItems.getSize() - depth;
						for( int count = 0; count < itemsToRemove; count++ )
						{
							demoPathItems.popBack();
						}

						demoPathItems.pushBack( demoPath );
					}
					depth++;
					demoName = demoName.substr(offset+1);
				}
			}
		}
		m_step++;
	}

	if( m_step > 0 )
	{
		m_env->m_textDisplay->outputText(hkString( "Done. Output is in " ) + testPlanFilename, 20, 250, 0xffffffff);
	}

	return DEMO_OK;
}

TestPlanDemo::~TestPlanDemo()
{

}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( TestPlanDemo, HK_DEMO_TYPE_OTHER, TestPlanVariant, g_variants, "Generate Test Plans" );

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
