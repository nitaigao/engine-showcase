/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_BATCHADDREMOVEAPI_H
#define HK_BATCHADDREMOVEAPI_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Common/Base/System/Stopwatch/hkStopwatch.h>

#include <Common/Base/Monitor/MonitorStreamAnalyzer/hkMonitorStreamAnalyzer.h>

class hkpEntity;
class myFlatLand;

class BatchAddRemoveBodiesDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		BatchAddRemoveBodiesDemo(hkDemoEnvironment* env);
		~BatchAddRemoveBodiesDemo();
		
		double addToGrid( int num, hkBool addBatch = false );
		double removeFromGrid( int num, hkBool removeBatch = false );


		int m_gridWidth;
		int m_gridHeight;

		float m_cellSize;

		hkArray<hkBool> m_gridOccupied;
		hkArray<hkpEntity*> m_entities;

		hkDemo::Result stepDemo(); 

		hkPseudoRandomGenerator m_rand;

		hkBool m_batchMode;

		int m_numBodies;

		hkStopwatch m_timer;

		hkMonitorStreamAnalyzer m_capture;

		hkBool m_addRemoveToggle;

		hkString m_textOutput;

		hkBool m_showGraphics;

		myFlatLand*	m_myFlatLand;

};


#endif  //HK_BATCHADDREMOVEAPI_H

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
