/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_BULLET_VS_PAPER_H
#define HK_BULLET_VS_PAPER_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>


	/// A demo demonstrating a bullet-through-paper solution.
	/// When objects move sufficiently fast, it is possible for them to "tunnel" 
	/// or move completely through each other in Havok, because simulation is
	/// carried out at discrete timesteps. For example, between one timestep and 
	/// the next, a fast moving object can move entirely from one side of the wall 
	/// to the other. This is also known as the "bullet through paper" problem. 
	/// There are two mechanisms in Havok that you can use to prevent this problem.	
	/// The first is known as pairwise backstepping, which is a simple backstepping 
	/// algorithm which works out the exact collision point between pairs of objects 
	/// in the collision detector. The second is the hkConvexSweepShape which is used 
	/// to limit the velocities of objects when they collide.
	/// This game utilises and demonstrates both approaches.
class DiscreteVsContinuousDemo: public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		DiscreteVsContinuousDemo(hkDemoEnvironment* environment);
		~DiscreteVsContinuousDemo();

		/*
		 * Game setup
		 */

		Result stepDemo(); 
		
	private:
		hkpWorld* m_world2;

		hkArray<hkpRigidBody*> m_rigidBodies;
		hkArray<hkpRigidBody*> m_rigidBodies2;

		hkVector4 m_startPos;

		hkReal m_velocity;
		hkReal m_time;

		int m_nextBody;
		int m_counter;
		int m_duration;

		int m_simulationBeingControlled;
		hkReal m_simulationFrequency[2];

#define HK_TIMING_SAMPLES 20
		hkReal m_avgTimings[2][HK_TIMING_SAMPLES];
		int m_nextTimingIndex;

		// Simulation frame time elapsed -- used for discrete simulation
		hkReal m_elapsedSimulationFrameTime;

		// Added to find a currentTime<->eventTime inconsitency bug
		int m_frameCount;
};

#endif // HK_BULLET_VS_PAPER_H

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
