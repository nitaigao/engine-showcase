/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef MONITORHELPER_MULTITHREADED_H
#define MONITORHELPER_MULTITHREADED_H

#include <Common/Base/Monitor/MonitorStreamAnalyzer/hkMonitorStreamAnalyzer.h>

//
// A simple class for extracting specific timers from a demo each frame
//
class MultithreadedMonitorHelper : public hkReferencedObject
{
public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	MultithreadedMonitorHelper(hkDefaultDemo* demo);
	~MultithreadedMonitorHelper();

	enum TrackingType
	{
		TRACK_FRAME_MAX, 						// getMaxTime()
		TRACK_FRAME_AVERAGE, 					// getAvgTime()
		TRACK_FRAME_MAX_EXCLUDING_CHILDREN,		// getMaxTimeExcludingChildren()
		TRACK_FRAME_AVERAGE_EXCLUDING_CHILDREN,	// getAvgTimeExcludingChildren()
	};

	struct MtMonitorHelperDetails
	{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, MtMonitorHelperDetails);

		MtMonitorHelperDetails(const char *name)
		{
			m_name = name;
			m_node = HK_NULL;
		}

		const char* m_name;
		hkMonitorStreamAnalyzer::Node* m_node;

		hkReal m_times		[ hkMonitorStreamAnalyzer::NUM_VALUES ];
		hkReal m_childTimes	[ hkMonitorStreamAnalyzer::NUM_VALUES ];
		

		// Gets the max time spent in the timer section
		// Useful e.g. when a task is running on 1 SPU/thread
		hkReal getMaxTime() const; 

		// Gets the average time spent in the timer section
		// Useful e.g. when a task is running on multiple SPUs/threads
		hkReal getAvgTime() const; 

		// Time spent in the node, minus time spent in all of its children.
		// Example: raycast timing includes time spent in broadphase + time spent in narrowphase
		//		The time spent in the narrowphase is in child nodes
		hkReal getMaxTimeExcludingChildren() const;
		hkReal getAvgTimeExcludingChildren() const;

		// Mainly for internal use
		void fillInFromNode(hkMonitorStreamAnalyzer::Node* node);
		void zero();
	};

	struct TrackedTimerEntry
	{

		TrackedTimerEntry( const char* name, const char* childName, TrackingType type) :	m_name(name), m_childName(childName), m_type(type)
		{	reset(); }

		const char* m_name;
		const char* m_childName;
		int m_numFrames; // number of frames that it's been found

		hkReal m_maxValue;
		hkReal m_minValue;
		hkReal m_totalValue;

		hkEnum<TrackingType, hkUint8> m_type;

		hkReal getMinFrameValue() const { return m_minValue; }
		hkReal getMaxFrameValue() const { return m_maxValue; }
		hkReal getAvgFrameValue() const { return m_totalValue / hkReal(m_numFrames); }

		void reset()
		{
			m_maxValue = -HK_REAL_MAX;
			m_minValue = HK_REAL_MAX;
			m_totalValue = 0.0f;
			m_numFrames = 0;
		}

		void updateFromDetails(const MtMonitorHelperDetails& );

		bool operator==(const TrackedTimerEntry& other)
		{
			const hkBool typeMatch  = (other.m_type == m_type);
			const hkBool nameMatch  = (hkString::strCmp(m_name, other.m_name) == 0);
			const hkBool childMatch = m_childName && other.m_childName && (hkString::strCmp(m_childName, other.m_childName) == 0); //can't strCmp a null string

			return  ( typeMatch && nameMatch && childMatch );
		}
	};

public:
	// Should be called every frame, after the main work for the demo has been done.
	void updateTree();

	// Given a MtMonitorHelperDetails with the name specified already, find the timing information
	void findTimerValues(MtMonitorHelperDetails& details) const;

	// Finds the child timings under the specified parent
	// Useful if a timer string might appear in multiple places
	void findTimerValuesForChild(MtMonitorHelperDetails& parent, MtMonitorHelperDetails& child) const;

	void trackTimer(const char* timerName, TrackingType type);
	void trackTimerWithChild(const char* parentName, const char* childName, TrackingType type);
	
	TrackedTimerEntry* findTrackedTimer(const char* name, TrackingType type);
	TrackedTimerEntry* findTrackedTimerWithChild(const char* name, const char* child, TrackingType type);

	void resetTrackedTimers();

public:
	hkDefaultDemo* m_demo;

protected:
	void updateTrackedTimers();

	hkMonitorStreamAnalyzer::Node* m_tree;
	hkArray<TrackedTimerEntry> m_trackedTimers;

};

#endif	// MONITORHELPER_MULTITHREADED_H

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
