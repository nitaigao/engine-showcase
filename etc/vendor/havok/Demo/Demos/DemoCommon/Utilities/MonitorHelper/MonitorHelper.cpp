/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/MonitorHelper/MonitorHelper.h>

MonitorHelper::MonitorHelper()
	: m_monitorStreamAnalyser(HK_NULL),
		m_rootnode(HK_NULL)
{
}

MonitorHelper::~MonitorHelper()
{
	delete m_monitorStreamAnalyser;
}

void MonitorHelper::init(int size)
{
	m_monitorStreamAnalyser = new hkMonitorStreamAnalyzer(size);
}


bool MonitorHelper::nodeExists(const char* name)
{
	hkMonitorStreamAnalyzer::Node* node = findFirstNodeByName( m_rootnode, name );
	return ( node != HK_NULL );
}
	

hkMonitorStreamAnalyzer::Node* HK_CALL MonitorHelper::findFirstNodeByName( hkMonitorStreamAnalyzer::Node* node, const char* childName )
{
	if( hkString::strCmp( childName, node->m_name ) == 0 )
	{
		return node;
	}

	// search children
	for( int j = 0; j < node->m_children.getSize(); j++ )
	{
		hkMonitorStreamAnalyzer::Node *n = findFirstNodeByName(node->m_children[j], childName);
		if(n)
				return n;
	}
	return HK_NULL;
}

void MonitorHelper::grabAllStats()
{
	HK_ASSERT2(0x0, m_rootnode == HK_NULL, "m_rootnode not null. Did you clean up from last call by calling freeAllMem()?");

	HK_ASSERT2(0x0, m_monitorStreamAnalyser, "m_monitorStreamAnalyser not yet initialized. Did you call MonitorHelper::init() yet?");
	if(!m_monitorStreamAnalyser)
	{
		return;
	}
	m_monitorStreamAnalyser->reset();
	hkMonitorStream& stream = hkMonitorStream::getInstance();
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
	m_monitorStreamAnalyser->captureFrameDetails(stream.getStart(), stream.getEnd(), frameInfo); 

	m_rootnode = m_monitorStreamAnalyser->makeStatisticsTreeForSingleFrame(stream.getStart(), stream.getEnd(), frameInfo);
}


void MonitorHelper::freeAllMem()
{
	delete m_rootnode;
	m_rootnode = HK_NULL;
}

void MonitorHelper::findTimerValues(hkArray<MonitorHelperDetails>& details)
{
	for(int i = 0; i < details.getSize(); i++)
	{
		hkMonitorStreamAnalyzer::Node* node = findFirstNodeByName( m_rootnode, details[i].m_name );
		HK_ASSERT2(0x0, node, "Couldn't find this timer!");
		details[i].m_time = node->m_value[0];
	}
}

void MonitorHelper::saveSingleFrameStatsToFile(const char* filename)
{
	HK_ASSERT2(0x0, m_monitorStreamAnalyser, "m_monitorStreamAnalyser not yet initialized. Did you call MonitorHelper::init() yet?");
	if(!m_monitorStreamAnalyser)
	{
		return;
	}
	m_monitorStreamAnalyser->reset();
	hkMonitorStream& stream = hkMonitorStream::getInstance();
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
	m_monitorStreamAnalyser->captureFrameDetails(stream.getStart(), stream.getEnd(), frameInfo); 


	hkOstream ostr (filename);
	m_monitorStreamAnalyser->writeStatistics( ostr );

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
