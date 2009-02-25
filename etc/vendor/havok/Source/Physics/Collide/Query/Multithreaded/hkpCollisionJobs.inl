/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


hkpCollisionQueryJob::hkpCollisionQueryJob( JobSubType subType, hkUint16 size )
	: hkJob( HK_JOB_TYPE_COLLISION_QUERY, subType, size, HK_JOB_SPU_TYPE_DISABLED )
{
	m_sharedJobHeaderOnPpu = HK_NULL;
}


// ===============================================================================================================================================================================================
//  SHAPE RAYCAST
// ===============================================================================================================================================================================================

hkpShapeRayCastJob::hkpShapeRayCastJob(hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpShapeRayCastCommand* commandArray, int numCommands, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask) 
:	hkpCollisionQueryJob(COLLISION_QUERY_SHAPE_RAYCAST, sizeof(hkpShapeRayCastJob))
{
	HK_ASSERT2( 0xaf236153, jobHeader,							"jobHeader has to be set." );
	HK_ASSERT2( 0xaf3647f2, numCommands > 0,					"numCommands has to be > 0." );
	HK_ASSERT2( 0xaf3647e3, commandArray,						"commandArray has to be set." );

	HK_ASSERT2( 0xaf236158, ((hkUlong)jobHeader & 0xf) == 0,	"jobHeader has to be 16 byte aligned." );
	HK_ASSERT2( 0xaf3647e9, ((hkUlong)commandArray & 0xf) == 0,	"commandArray has to be 16 byte aligned." );

	HK_ASSERT2( 0xaf3647ff, numCommandsPerTask > 0 && numCommandsPerTask <= MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK,	"numCommandsPerTask has to be > 0 and <= " << MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK << "." );

#if defined(HK_DEBUG)
	{
		for (int i=0; i<numCommands; i++)
		{
			const hkpShapeRayCastCommand* command = &commandArray[i];

			HK_ASSERT2( 0xaf3647e4, command->m_collidables, "command[ " << i << " ]->m_collidables has to be set." );
			HK_ASSERT2( 0xaf3647e5, command->m_results,		"command[ " << i << " ]->m_results has to be set." );

			HK_ASSERT2( 0xaf3647e6, ((hkUlong)command->m_collidables & 0xf) == 0, "command[ " << i << " ]->m_collidables (address 0x" << command->m_collidables << " has to be 16 byte aligned." );
			HK_ASSERT2( 0xaf3647e7, ((hkUlong)command->m_results     & 0xf) == 0, "command[ " << i << " ]->m_results (address 0x" << command->m_results << " has to be 16 byte aligned." );

			HK_ASSERT2( 0xaf3648e0, command->m_numCollidables > 0 && command->m_numCollidables <= hkpShapeRayCastCommand::MAXIMUM_NUM_COLLIDABLES,		"hkpShapeRayCastCommand::m_numCollidables has to be > 0 and <= " << hkpShapeRayCastCommand::MAXIMUM_NUM_COLLIDABLES << "." );
			HK_ASSERT2( 0xaf3648e2, command->m_resultsCapacity > 0 && command->m_resultsCapacity <= hkpShapeRayCastCommand::MAXIMUM_RESULTS_CAPACITY,	"hkpShapeRayCastCommand::m_resultsCapacity has to be > 0 and <= " << hkpShapeRayCastCommand::MAXIMUM_RESULTS_CAPACITY << "." );
		}
	}

	// This is a very simple and crude attempt to try to catch a common mistake where the user might
	// forget to actually advance the pointer to the results. Doing so could cause incorrect query results.
	// This check though will NOT catch trickier situations like e.g. partially overlapping results.
	if ( numCommands > 1 )
	{
		HK_ASSERT2(0xaf253411, commandArray[0].m_results != commandArray[1].m_results, "You are not allowed to re-use the same results buffer for two different query commands.");
	}
#endif

	m_collisionInput		= input;
	m_sharedJobHeaderOnPpu	= jobHeader;
	m_numCommandsPerTask	= numCommandsPerTask;
	m_semaphore				= semaphore;
	m_commandArray			= commandArray;
	m_numCommands			= numCommands;

	// precalculate the total number of jobs that will be spawned from the original job (incl. the original)
	m_sharedJobHeaderOnPpu->m_openJobs = ((numCommands-1)/numCommandsPerTask) + 1;
}


hkJobQueue::JobPopFuncResult hkpShapeRayCastJob::popJobTask( hkpShapeRayCastJob& out )
{
	//
	// split off a fully filled child job if there are more commands left than one job can handle
	//
	if ( m_numCommands > m_numCommandsPerTask )
	{
		out.m_numCommands	 = m_numCommandsPerTask;
		m_numCommands		-= m_numCommandsPerTask;
		m_commandArray		 = hkAddByteOffsetConst(m_commandArray, m_numCommandsPerTask * sizeof(hkpShapeRayCastCommand));

		return hkJobQueue::DO_NOT_POP_QUEUE_ENTRY;
	}

	return hkJobQueue::POP_QUEUE_ENTRY;
}


// ===============================================================================================================================================================================================
//  WORLD RAYCAST
// ===============================================================================================================================================================================================

hkpWorldRayCastJob::hkpWorldRayCastJob(hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpWorldRayCastCommand* commandArray, int numCommands, const hkpBroadPhase* broadphase, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask)
:	hkpCollisionQueryJob( COLLISION_QUERY_WORLD_RAYCAST, sizeof(hkpWorldRayCastJob) )
{
	HK_ASSERT2( 0xaf236152, jobHeader && ((hkUlong)jobHeader & 0xf) == 0,											"jobHeader has to be set and be 16 byte aligned." );
	HK_ASSERT2( 0xaf3647f6, numCommands > 0,																		"numCommands has to be > 0." );
	HK_ASSERT2( 0xaf3647f7, commandArray && ((hkUlong)commandArray & 0xf) == 0,										"commandArray has to be set and 16 byte aligned." );
	HK_ASSERT2( 0xaf3647fd, numCommandsPerTask > 0 && numCommandsPerTask <= MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK,	"numCommandsPerTask has to be > 0 and <= " << MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK << "." );

#if defined(HK_DEBUG)
	{
		for (int i=0; i<numCommands; i++)
		{
			const hkpWorldRayCastCommand* command = &commandArray[i];
			HK_ASSERT2( 0xaf3647fe, command->m_results && ((hkUlong)command->m_results & 0xf) == 0,	"hkpWorldRayCastCommand::m_results has to be set and 16 byte aligned." );

			HK_ASSERT2( 0xaf3638e2, command->m_resultsCapacity > 0 && command->m_resultsCapacity <= hkpWorldRayCastCommand::MAXIMUM_RESULTS_CAPACITY, "hkpWorldRayCastCommand::m_resultsCapacity has to be > 0 and <= " << hkpWorldRayCastCommand::MAXIMUM_RESULTS_CAPACITY << "." );
		}
	}

	// This is a very simple and crude attempt to try to catch a common mistake where the user might
	// forget to actually advance the pointer to the results. Doing so could cause incorrect query results.
	// This check though will NOT catch trickier situations like e.g. partially overlapping results.
	if ( numCommands > 1 )
	{
		HK_ASSERT2(0xaf253412, commandArray[0].m_results != commandArray[1].m_results, "You are not allowed to re-use the same results buffer for two different query commands.");
	}
#endif

	m_collisionInput		= input;

	m_sharedJobHeaderOnPpu	= jobHeader;

	m_numCommandsPerTask	= numCommandsPerTask;
	m_semaphore				= semaphore;
	m_commandArray			= commandArray;
	m_numCommands			= numCommands;

	m_broadphase			= broadphase;

	// precalculate the total number of jobs that will be spawned from the original job (incl. the original)
	m_sharedJobHeaderOnPpu->m_openJobs = ((numCommands-1)/numCommandsPerTask) + 1;
}


hkJobQueue::JobPopFuncResult hkpWorldRayCastJob::popJobTask( hkpWorldRayCastJob& out )
{
	//
	// split off a fully filled child job if there are more commands left than one job can handle
	//
	if ( m_numCommands > m_numCommandsPerTask )
	{
		out.m_numCommands	 = m_numCommandsPerTask;
		m_numCommands		-= m_numCommandsPerTask;
		m_commandArray		 = hkAddByteOffsetConst(m_commandArray, m_numCommandsPerTask * sizeof(hkpWorldRayCastCommand));

		return hkJobQueue::DO_NOT_POP_QUEUE_ENTRY;
	}

	return hkJobQueue::POP_QUEUE_ENTRY;
}


// ===============================================================================================================================================================================================
// PAIR LINEAR CAST
// ===============================================================================================================================================================================================

hkpPairLinearCastJob::hkpPairLinearCastJob(hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpPairLinearCastCommand* commandArray, int numCommands, const hkpShapeCollectionFilter* filter, hkReal tolerance, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask) 
:	hkpCollisionQueryJob(COLLISION_QUERY_PAIR_LINEAR_CAST, sizeof(hkpPairLinearCastJob))
{
	HK_ASSERT2( 0xaf136151, jobHeader && ((hkUlong)jobHeader & 0xf) == 0,											"jobHeader has to be set and be 16 byte aligned." );
	HK_ASSERT2( 0xaf1647f5, numCommands > 0,																		"numCommands has to be > 0." );
	HK_ASSERT2( 0xaf1647e0, commandArray && ((hkUlong)commandArray & 0xf) == 0,										"commandArray has to be set and 16 byte aligned." );
	HK_ASSERT2( 0xaf1647b4, numCommandsPerTask > 0 && numCommandsPerTask <= MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK,	"numCommandsPerTask has to be > 0 and <= " << MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK << "." );

#if defined(HK_DEBUG)
	// check memory allocated on commands
	{
		for (int i = 0; i < numCommands; i++)
		{
			hkpPairLinearCastCommand* command = const_cast<hkpPairLinearCastCommand*>( &commandArray[i] );

			HK_ASSERT2( 0xaf2647fe, command->m_results && ((hkUlong)command->m_results & 0xf) == 0,														"hkpPairLinearCastCommand::m_results has to be set and 16 byte aligned." );
			HK_ASSERT2( 0xaf2638e2, command->m_resultsCapacity > 0 && command->m_resultsCapacity <= hkpPairLinearCastCommand::MAXIMUM_RESULTS_CAPACITY,	"hkpPairLinearCastCommand::m_resultsCapacity has to be > 0 and <= " << hkpPairGetClosestPointsCommand::MAXIMUM_RESULTS_CAPACITY << "." );
		}
	}

	// This is a very simple and crude attempt to try to catch a common mistake where the user might
	// forget to actually advance the pointer to the results. Doing so could cause incorrect query results.
	// This check though will NOT catch trickier situations like e.g. partially overlapping results.
	if ( numCommands > 1 )
	{
		HK_ASSERT2(0xaf253413, commandArray[0].m_results != commandArray[1].m_results, "You are not allowed to re-use the same results buffer for two different query commands.");
	}
#endif

	m_collisionInput		= input;

	m_sharedJobHeaderOnPpu	= jobHeader;

	m_tolerance				= tolerance;

	m_numCommandsPerTask	= numCommandsPerTask;
	m_semaphore				= semaphore;
	m_commandArray			= commandArray;
	m_numCommands			= numCommands;

	m_filter				= filter;

	
	m_maxExtraPenetration					= HK_REAL_EPSILON;
	m_iterativeLinearCastEarlyOutDistance	= 0.01f;
	m_iterativeLinearCastMaxIterations		= 10;

	// precalculate the total number of jobs that will be spawned from the original job (incl. the original)
	m_sharedJobHeaderOnPpu->m_openJobs = ((numCommands-1)/numCommandsPerTask) + 1;
}


hkpPairLinearCastJob::hkpPairLinearCastJob() : hkpCollisionQueryJob(COLLISION_QUERY_PAIR_LINEAR_CAST, sizeof(hkpPairLinearCastJob))
{
}


hkJobQueue::JobPopFuncResult hkpPairLinearCastJob::popJobTask( hkpPairLinearCastJob& out )
{
	//
	// split off a fully filled child job if there are more tasks left than one job can handle
	//
	if ( m_numCommands > m_numCommandsPerTask )
	{
		out.m_numCommands	 = m_numCommandsPerTask;
		m_numCommands		-= m_numCommandsPerTask;
		m_commandArray		 = hkAddByteOffsetConst(m_commandArray, m_numCommandsPerTask * sizeof(hkpPairLinearCastCommand));

		return hkJobQueue::DO_NOT_POP_QUEUE_ENTRY;
	}

	return hkJobQueue::POP_QUEUE_ENTRY;
}


// ===============================================================================================================================================================================================
//  WORLD LINEAR CAST
// ===============================================================================================================================================================================================

hkpWorldLinearCastJob::hkpWorldLinearCastJob(hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpWorldLinearCastCommand* commandArray, int numCommands, const hkpBroadPhase* broadphase, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask) 
:	hkpCollisionQueryJob(COLLISION_QUERY_WORLD_LINEAR_CAST, sizeof(hkpWorldLinearCastJob))
{
	HK_ASSERT2( 0xaf736152, jobHeader && ((hkUlong)jobHeader & 0xf) == 0,											"jobHeader has to be set and be 16 byte aligned." );
	HK_ASSERT2( 0xaf7647f6, numCommands > 0,																		"numCommands has to be > 0." );
	HK_ASSERT2( 0xaf7647f7, commandArray && ((hkUlong)commandArray & 0xf) == 0,										"commandArray has to be set and 16 byte aligned." );
	HK_ASSERT2( 0xaf7647f8, numCommandsPerTask > 0 && numCommandsPerTask <= MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK,	"numCommandsPerTask has to be > 0 and <= " << MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK << "." );

#if defined(HK_DEBUG)
	{
		for (int i=0; i<numCommands; i++)
		{
			const hkpWorldLinearCastCommand* command = &commandArray[i];
			HK_ASSERT2( 0xaf7647f9, command->m_results && ((hkUlong)command->m_results & 0xf) == 0,	"hkpWorldLinearCastCommand::m_results has to be set and 16 byte aligned." );

			HK_ASSERT2( 0xaf7647fb, command->m_resultsCapacity > 0 && command->m_resultsCapacity <= hkpWorldLinearCastCommand::MAXIMUM_RESULTS_CAPACITY, "hkpWorldLinearCastCommand::m_resultsCapacity has to be > 0 and <= " << hkpWorldLinearCastCommand::MAXIMUM_RESULTS_CAPACITY << "." );

		}
	}

	// This is a very simple and crude attempt to try to catch a common mistake where the user might
	// forget to actually advance the pointer to the results. Doing so could cause incorrect query results.
	// This check though will NOT catch trickier situations like e.g. partially overlapping results.
	if ( numCommands > 1 )
	{
		HK_ASSERT2(0xaf253414, commandArray[0].m_results != commandArray[1].m_results, "You are not allowed to re-use the same results buffer for two different query commands.");
	}
#endif

	m_collisionInput		= input;
	m_sharedJobHeaderOnPpu	= jobHeader;

	m_numCommandsPerTask	= numCommandsPerTask;
	m_semaphore				= semaphore;
	m_commandArray			= commandArray;
	m_numCommands			= numCommands;

	m_broadphase			= broadphase;

	// precalculate the total number of jobs that will be spawned from the original job (incl. the original)
	m_sharedJobHeaderOnPpu->m_openJobs = ((numCommands-1)/numCommandsPerTask) + 1;
}


hkJobQueue::JobPopFuncResult hkpWorldLinearCastJob::popJobTask( hkpWorldLinearCastJob& out )
{
	//
	// split off a fully filled child job if there are more commands left than one job can handle
	//
	if ( m_numCommands > m_numCommandsPerTask )
	{
		out.m_numCommands	 = m_numCommandsPerTask;
		m_numCommands		-= m_numCommandsPerTask;
		m_commandArray		 = hkAddByteOffsetConst(m_commandArray, m_numCommandsPerTask * sizeof(hkpWorldLinearCastCommand));

		return hkJobQueue::DO_NOT_POP_QUEUE_ENTRY;
	}

	return hkJobQueue::POP_QUEUE_ENTRY;
}


// ===============================================================================================================================================================================================
//  MOPP AABB
// ===============================================================================================================================================================================================

hkpMoppAabbJob::hkpMoppAabbJob(hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpMoppAabbCommand* commandArray, int numCommands, const hkUint8* moppCodeData, const hkpMoppCode::CodeInfo& moppCodeInfo, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask) 
:	hkpCollisionQueryJob(COLLISION_QUERY_MOPP_AABB, sizeof(hkpMoppAabbJob))
{
	m_jobSpuType = HK_JOB_SPU_TYPE_ENABLED;

	HK_ASSERT2( 0xaf236154, jobHeader && ((hkUlong)jobHeader & 0xf) == 0,											"jobHeader has to be set and be 16 byte aligned.");
	HK_ASSERT2( 0xaf3647d2, numCommands > 0,																		"numCommands has to be > 0." );
	HK_ASSERT2( 0xaf3647d3, commandArray && ((hkUlong)commandArray & 0xf) == 0,										"commandArray has to be set and 16 byte aligned." );
	HK_ASSERT2( 0xaf3647df, numCommandsPerTask > 0 && numCommandsPerTask <= MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK,	"numCommandsPerTask has to be > 0 and <= " << MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK << "." );

#if defined(HK_DEBUG)
	{
		for (int i=0; i<numCommands; i++)
		{
			const hkpMoppAabbCommand* command = &commandArray[i];
			HK_ASSERT2( 0xaf3647d5, command->m_results && ((hkUlong)command->m_results & 0xf) == 0, "hkpMoppAabbCommand::m_results has to be set and 16 byte aligned." );
		}
	}

	// This is a very simple and crude attempt to try to catch a common mistake where the user might
	// forget to actually advance the pointer to the results. Doing so could cause incorrect query results.
	// This check though will NOT catch trickier situations like e.g. partially overlapping results.
	if ( numCommands > 1 )
	{
		HK_ASSERT2(0xaf253415, commandArray[0].m_results != commandArray[1].m_results, "You are not allowed to re-use the same results buffer for two different query commands.");
	}
#endif

	m_collisionInput		= input;
	m_sharedJobHeaderOnPpu	= jobHeader;

	m_moppCodeInfo			= moppCodeInfo;
	m_moppCodeData			= moppCodeData;

	m_numCommandsPerTask	= numCommandsPerTask;
	m_semaphore				= semaphore;
	m_commandArray			= commandArray;
	m_numCommands			= numCommands;

	// precalculate the total number of jobs that will be spawned from the original job (incl. the original)
	m_sharedJobHeaderOnPpu->m_openJobs = ((numCommands-1)/numCommandsPerTask) + 1;
}


hkJobQueue::JobPopFuncResult hkpMoppAabbJob::popJobTask( hkpMoppAabbJob& out )
{
	//
	// split off a fully filled child job if there are more commands left than one job can handle
	//
	if ( m_numCommands > m_numCommandsPerTask )
	{
		out.m_numCommands	 = m_numCommandsPerTask;
		m_numCommands		-= m_numCommandsPerTask;
		m_commandArray		 = hkAddByteOffsetConst(m_commandArray, m_numCommandsPerTask * sizeof(hkpMoppAabbCommand));

		return hkJobQueue::DO_NOT_POP_QUEUE_ENTRY;
	}

	return hkJobQueue::POP_QUEUE_ENTRY;
}


// ===============================================================================================================================================================================================
//  PAIR GET CLOSEST POINTS
// ===============================================================================================================================================================================================

hkpPairGetClosestPointsJob::hkpPairGetClosestPointsJob(hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpPairGetClosestPointsCommand* commandArray, int numCommands, hkReal tolerance, hkSemaphoreBusyWait* semaphore, int numCommandsPerTask) 
:	hkpCollisionQueryJob(COLLISION_QUERY_PAIR_GET_CLOSEST_POINTS,  sizeof(hkpPairGetClosestPointsJob))
{
	HK_ASSERT2( 0xaf236151, jobHeader && ((hkUlong)jobHeader & 0xf) == 0,											"jobHeader has to be set and be 16 byte aligned." );
	HK_ASSERT2( 0xaf3647f5, numCommands > 0,																		"numCommands has to be > 0." );
	HK_ASSERT2( 0xaf3647e0, commandArray && ((hkUlong)commandArray & 0xf) == 0,										"commandArray has to be set and 16 byte aligned." );
	HK_ASSERT2( 0xaf3647b4, numCommandsPerTask > 0 && numCommandsPerTask <= MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK,	"numCommandsPerTask has to be > 0 and <= " << MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK << "." );

	{
		for (int i = 0; i < numCommands; i++)
		{
			hkpPairGetClosestPointsCommand* command = const_cast<hkpPairGetClosestPointsCommand*>( &commandArray[i] );

#if defined(HK_DEBUG)
			HK_ASSERT2( 0xaf1647fe, command->m_results && ((hkUlong)command->m_results & 0xf) == 0,																"hkpPairGetClosestPointsCommand::m_results has to be set and 16 byte aligned." );
			HK_ASSERT2( 0xaf1638e2, command->m_resultsCapacity > 0 && command->m_resultsCapacity <= hkpPairGetClosestPointsCommand::MAXIMUM_RESULTS_CAPACITY,	"hkpPairGetClosestPointsCommand::m_resultsCapacity has to be > 0 and <= " << hkpPairGetClosestPointsCommand::MAXIMUM_RESULTS_CAPACITY << "." );
#endif

			command->m_indexIntoSharedResults = HK_NULL;
		}
	}

#if defined(HK_DEBUG)
	// This is a very simple and crude attempt to try to catch a common mistake where the user might
	// forget to actually advance the pointer to the results. Doing so could cause incorrect query results.
	// This check though will NOT catch trickier situations like e.g. partially overlapping results.
	if ( numCommands > 1 )
	{
		HK_ASSERT2(0xaf253416, commandArray[0].m_results != commandArray[1].m_results, "You are not allowed to re-use the same results buffer for two different query commands.");
	}
#endif

	m_collisionInput		= input;

	m_sharedJobHeaderOnPpu	= jobHeader;

	m_tolerance				= tolerance;

	m_numCommandsPerTask	= numCommandsPerTask;
	m_semaphore				= semaphore;
	m_commandArray			= commandArray;
	m_numCommands			= numCommands;

	// precalculate the total number of jobs that will be spawned from the original job (incl. the original)
	m_sharedJobHeaderOnPpu->m_openJobs = ((numCommands-1)/numCommandsPerTask) + 1;
}


hkpPairGetClosestPointsJob::hkpPairGetClosestPointsJob()
:	hkpCollisionQueryJob(COLLISION_QUERY_PAIR_GET_CLOSEST_POINTS, sizeof(hkpPairGetClosestPointsJob))
{
}


hkJobQueue::JobPopFuncResult hkpPairGetClosestPointsJob::popJobTask( hkpPairGetClosestPointsJob& out )
{
	//
	// split off a fully filled child job if there are more tasks left than one job can handle
	//
	if ( m_numCommands > m_numCommandsPerTask )
	{
		out.m_numCommands	 = m_numCommandsPerTask;
		m_numCommands		-= m_numCommandsPerTask;
		m_commandArray		 = hkAddByteOffsetConst(m_commandArray, m_numCommandsPerTask * sizeof(hkpPairGetClosestPointsCommand));

		return hkJobQueue::DO_NOT_POP_QUEUE_ENTRY;
	}

	return hkJobQueue::POP_QUEUE_ENTRY;
}


// ===============================================================================================================================================================================================
//  WORLD GET CLOSEST POINTS
// ===============================================================================================================================================================================================

hkpWorldGetClosestPointsJob::hkpWorldGetClosestPointsJob(hkpProcessCollisionInput* input, hkpCollisionQueryJobHeader* jobHeader, const hkpWorldGetClosestPointsCommand* commandArray, int numCommands, hkpPairGetClosestPointsCommand* pairGetClosestPointsCommandBuffer, int pairGetClosestPointsCommandBufferCapacity, const hkpBroadPhase* broadphase, hkReal tolerance, hkSemaphoreBusyWait* semaphore) 
:	hkpCollisionQueryJob(COLLISION_QUERY_WORLD_GET_CLOSEST_POINTS, sizeof(hkpWorldGetClosestPointsJob))
{
	HK_ASSERT2( 0xaf236152, jobHeader && ((hkUlong)jobHeader & 0xf) == 0,							"jobHeader has to be set and be 16 byte aligned." );
	HK_ASSERT2( 0xaf3647f6, numCommands > 0 && numCommands <= MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK,	"numCommands has to be > 0 and <= " << MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK << "." );
	HK_ASSERT2( 0xaf3647f7, commandArray && ((hkUlong)commandArray & 0xf) == 0,						"commandArray has to be set and 16 byte aligned." );
	HK_ASSERT2( 0xaf3647f8, ((hkUlong)pairGetClosestPointsCommandBuffer & 0xf) == 0,				"pairGetClosestPointsCommandBuffer has to be either HK_NULL or 16 byte aligned." );

	{
		for (int i=0; i<numCommands; i++)
		{
			hkpWorldGetClosestPointsCommand* command = const_cast<hkpWorldGetClosestPointsCommand*>( &commandArray[i] );

#if defined(HK_DEBUG)
			HK_ASSERT2( 0xaf3647f9, command->m_resultsCapacity > 0,									"hkpWorldGetClosestPointsCommand::m_resultsCapacity has to be > 0." );
			HK_ASSERT2( 0xaf3647fb, command->m_results && ((hkUlong)command->m_results & 0xf) == 0,	"hkpWorldGetClosestPointsCommand::m_results has to be set and 16 byte aligned." );
#endif

			command->m_numResultsOut = 0;
		}
	}

#if defined(HK_DEBUG)
	// This is a very simple and crude attempt to try to catch a common mistake where the user might
	// forget to actually advance the pointer to the results. Doing so could cause incorrect query results.
	// This check though will NOT catch trickier situations like e.g. partially overlapping results.
	if ( numCommands > 1 )
	{
		HK_ASSERT2(0xaf253417, commandArray[0].m_results != commandArray[1].m_results, "You are not allowed to re-use the same results buffer for two different query commands.");
	}
#endif

	m_collisionInput							= input;
	m_sharedJobHeaderOnPpu						= jobHeader;

	m_broadphase								= broadphase;
	m_tolerance									= tolerance;

	m_semaphore									= semaphore;
	m_commandArray								= commandArray;
	m_numCommands								= numCommands;

	m_pairGetClosestPointsCommandBuffer			= pairGetClosestPointsCommandBuffer;
	m_pairGetClosestPointsCommandBufferCapacity	= pairGetClosestPointsCommandBufferCapacity;

	m_sharedJobHeaderOnPpu->m_openJobs = OPEN_JOBS_PRESET;
}



/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20080925)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2008
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
