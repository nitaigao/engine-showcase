/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/MonitorHelper/MonitorHelper.h>
#include <Demos/DemoCommon/Utilities/MonitorHelper/MonitorHelperMultithreaded.h>

MultithreadedMonitorHelper::MultithreadedMonitorHelper(hkDefaultDemo* demo)
:	m_demo(demo),
	m_tree(HK_NULL)
{

}

MultithreadedMonitorHelper::~MultithreadedMonitorHelper()
{
	if (m_tree)
		delete m_tree;
}


void MultithreadedMonitorHelper::updateTree()
{
	if (m_tree)
	{
		delete m_tree;
	}

	hkArray<hkTimerData> threadStreams;
	hkArray<hkTimerData> spuStreams;

	m_demo->getTimerStreamInfo( threadStreams, spuStreams, 6 );

	hkMonitorStreamFrameInfo frameInfo;

	frameInfo.m_indexOfTimer0 = 0;
	frameInfo.m_indexOfTimer1 = -1;
	frameInfo.m_timerFactor0 = 1e3f / float(hkStopwatch::getTicksPerSecond());
	frameInfo.m_timerFactor1 = 1;
	frameInfo.m_absoluteTimeCounter = hkMonitorStreamFrameInfo::ABSOLUTE_TIME_TIMER_0;

	m_tree = hkMonitorStreamAnalyzer::makeStatisticsTreeForSingleFrame(threadStreams[0].m_streamBegin, threadStreams[0].m_streamEnd, frameInfo, "/", true );

	hkReal blendFromLastFrame = 0.0f;

	for (int destId = 0; destId < threadStreams.getSize(); ++destId)
	{
		hkMonitorStreamAnalyzer::Node* currentTree = hkMonitorStreamAnalyzer::makeStatisticsTreeForSingleFrame(threadStreams[destId].m_streamBegin, threadStreams[destId].m_streamEnd, frameInfo, "/", true );
		hkMonitorStreamAnalyzer::mergeTreesForCombinedThreadSummary(m_tree, currentTree, destId, 0, blendFromLastFrame );
		delete currentTree;
	}
	for (int destId = 0; destId < spuStreams.getSize(); ++destId)
	{
		hkMonitorStreamAnalyzer::Node* currentTree = hkMonitorStreamAnalyzer::makeStatisticsTreeForSingleFrame(spuStreams[destId].m_streamBegin, spuStreams[destId].m_streamEnd, frameInfo, "/", true );
		hkMonitorStreamAnalyzer::mergeTreesForCombinedThreadSummary(m_tree, currentTree, destId + threadStreams.getSize(), 0, blendFromLastFrame );
		delete currentTree;
	}

	updateTrackedTimers();
}

void MultithreadedMonitorHelper::findTimerValues(MtMonitorHelperDetails& detail) const
{
	hkMonitorStreamAnalyzer::Node* node = MonitorHelper::findFirstNodeByName( m_tree, detail.m_name );
	if (node)
	{
		detail.fillInFromNode(node);
	}

	else
	{
		detail.zero();
	}
}

void MultithreadedMonitorHelper::findTimerValuesForChild(MtMonitorHelperDetails& parent, MtMonitorHelperDetails& child) const
{
	if (!parent.m_node)
	{
		findTimerValues(parent);
	}

	if (!parent.m_node)
	{
		child.zero(); // parent was zeroed already
		return;
	}

	hkMonitorStreamAnalyzer::Node* childNode = MonitorHelper::findFirstNodeByName( parent.m_node, child.m_name );

	if (childNode)
	{
		child.fillInFromNode( childNode );
	}
	else
	{
		child.zero();
	}


}

hkReal MultithreadedMonitorHelper::MtMonitorHelperDetails::getMaxTime() const
{
	hkReal maxVal = m_times[0];
	for (int i=1; i<hkMonitorStreamAnalyzer::NUM_VALUES; i++)
	{
		maxVal = hkMath::max2(maxVal, m_times[i]);
	}
	return maxVal;
}

hkReal MultithreadedMonitorHelper::MtMonitorHelperDetails::getAvgTime() const
{
	hkReal total = 0.0f;
	for (int i=0; i<hkMonitorStreamAnalyzer::NUM_VALUES; i++)
	{
		total += m_times[i];
	}

	return total / (hkReal) (hkMonitorStreamAnalyzer::NUM_VALUES);
}

hkReal MultithreadedMonitorHelper::MtMonitorHelperDetails::getMaxTimeExcludingChildren() const
{
	hkReal maxVal = m_times[0] - m_childTimes[0];
	for (int i=1; i<hkMonitorStreamAnalyzer::NUM_VALUES; i++)
	{
		maxVal = hkMath::max2(maxVal, m_times[i]  - m_childTimes[i]);
	}
	return maxVal;
}

hkReal MultithreadedMonitorHelper::MtMonitorHelperDetails::getAvgTimeExcludingChildren() const
{
	hkReal total = 0.0f;
	for (int i=0; i<hkMonitorStreamAnalyzer::NUM_VALUES; i++)
	{
		total += m_times[i] - m_childTimes[i];
	}

	return total / (hkReal) (hkMonitorStreamAnalyzer::NUM_VALUES);
}

void MultithreadedMonitorHelper::MtMonitorHelperDetails::zero()
{
	hkString::memSet(m_times,      0, sizeof(hkReal) * hkMonitorStreamAnalyzer::NUM_VALUES);
	hkString::memSet(m_childTimes, 0, sizeof(hkReal) * hkMonitorStreamAnalyzer::NUM_VALUES);
}

void MultithreadedMonitorHelper::MtMonitorHelperDetails::fillInFromNode(hkMonitorStreamAnalyzer::Node* node)
{
	HK_ASSERT(0x0, node);
	m_node = node;
	hkString::memCpy(m_times, node->m_value, sizeof(hkReal) * hkMonitorStreamAnalyzer::NUM_VALUES);

	for (int k=0; k<hkMonitorStreamAnalyzer::NUM_VALUES; k++)
	{
		m_childTimes[k] = 0.0f;

		for (int j=0; j<node->m_children.getSize(); j++)
		{
			m_childTimes[k] += node->m_children[j]->m_value[k];
		}
	}
}

//
//	Timer Tracking
//
void MultithreadedMonitorHelper::trackTimer(const char* name, TrackingType type)
{
	m_trackedTimers.pushBack( TrackedTimerEntry(name, HK_NULL, type) );
}

void MultithreadedMonitorHelper::trackTimerWithChild(const char* name, const char* child, TrackingType type)
{
	m_trackedTimers.pushBack( TrackedTimerEntry(name, child, type) );
}

MultithreadedMonitorHelper::TrackedTimerEntry* MultithreadedMonitorHelper::findTrackedTimer(const char* name, TrackingType type)
{
	for (int i=0; i<m_trackedTimers.getSize(); i++)
	{
		TrackedTimerEntry& entry = m_trackedTimers[i];

		if ((entry.m_type == type) && (hkString::strCmp(name, entry.m_name) == 0))
		{
			return &m_trackedTimers[i];
		}
	}
	return HK_NULL;
}

MultithreadedMonitorHelper::TrackedTimerEntry* MultithreadedMonitorHelper::findTrackedTimerWithChild(const char* name, const char* childName, TrackingType type)
{
	TrackedTimerEntry temp(name, childName, type);
	int idx = m_trackedTimers.indexOf(temp);

	if (idx >= 0)
		return &m_trackedTimers[idx];
	else
		return HK_NULL;
}

void MultithreadedMonitorHelper::TrackedTimerEntry::updateFromDetails(const MtMonitorHelperDetails& details)
{
	if(details.m_node == HK_NULL)
	{
		return;
	}
	m_numFrames++;
	hkReal value = 0.0f;

	switch (m_type)
	{
		case TRACK_FRAME_MAX:
			value = details.getMaxTime();
			break;
		case TRACK_FRAME_AVERAGE:
			value = details.getAvgTime();
			break;
		case TRACK_FRAME_MAX_EXCLUDING_CHILDREN:
			value = details.getMaxTimeExcludingChildren();
			break;
		case TRACK_FRAME_AVERAGE_EXCLUDING_CHILDREN:
			value = details.getAvgTimeExcludingChildren();
			break;
		default:
			HK_WARN_ONCE(0x0, "Bad tracking type");
			value = 0.0f;
			break;
	}

	m_totalValue += value;
	m_maxValue = hkMath::max2(m_maxValue, value);
	m_minValue = hkMath::min2(m_minValue, value);
}

void MultithreadedMonitorHelper::updateTrackedTimers()
{
	for (int i=0; i<m_trackedTimers.getSize(); i++)
	{
		TrackedTimerEntry& entry = m_trackedTimers[i];
		
		if (!entry.m_childName)
		{
			MtMonitorHelperDetails details(entry.m_name);
			findTimerValues(details);
			entry.updateFromDetails(details);
		}
		else // with child
		{
			MtMonitorHelperDetails parent(entry.m_name);
			MtMonitorHelperDetails child(entry.m_childName);
			findTimerValuesForChild(parent, child);
			entry.updateFromDetails(child);
		}

		
	}
}

void MultithreadedMonitorHelper::resetTrackedTimers()
{
	for (int i=0; i<m_trackedTimers.getSize(); i++)
	{
		m_trackedTimers[i].reset();
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
