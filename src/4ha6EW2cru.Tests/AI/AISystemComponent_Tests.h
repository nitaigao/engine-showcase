#ifndef __AI_SYSTEMS_COMPONENT_TESTS
#define __AI_SYSTEMS_COMPONENT_TESTS

#include <cppunit/extensions/HelperMacros.h>

#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/hkThreadMemory.h>
#include <Common/Base/Memory/Memory/Pool/hkPoolMemory.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/Job/ThreadPool/Spu/hkSpuJobThreadPool.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>	

class AISystemComponent_Tests : public CPPUNIT_NS::TestFixture
{

	CPPUNIT_TEST_SUITE( AISystemComponent_Tests );
	CPPUNIT_TEST( Should_Return_Name_On_GetName );
	CPPUNIT_TEST( Should_Set_BehaviorName_On_SetBehavior );
	CPPUNIT_TEST_SUITE_END( );

public:

	void setUp( );
	void tearDown( );

protected:

	void Should_Return_Name_On_GetName( );
	void Should_Set_BehaviorName_On_SetBehavior( );

};

#endif