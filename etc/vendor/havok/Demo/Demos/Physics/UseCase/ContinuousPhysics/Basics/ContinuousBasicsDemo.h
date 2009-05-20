/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

/// A demo which shoots one sphere onto another sphere.
/// The game changes a number of parameters to show the effects of those parameter in a game
///  - velocity of the first sphere
///  - starting position of the first sphere
///  - using slow motion
///  - using continuous collision detection

#ifndef HK_ContinuousBasicsDemo_H
#define HK_ContinuousBasicsDemo_H

class MarbleAction;

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Dynamics/Collide/hkpCollisionListener.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>



class ContinuousBasicsDemo : public hkDefaultPhysicsDemo, hkpCollisionListener
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		ContinuousBasicsDemo(hkDemoEnvironment* env);
		~ContinuousBasicsDemo();

		Result stepDemo(); 

	protected:
		void resetScene();
		void showTrajectories();

		virtual void postRenderDisplayWorld(class hkgViewport* v);
		virtual void setupContexts(hkArray<hkProcessContext*>& contexts);
		virtual void contactPointAddedCallback(	hkpContactPointAddedEvent& event);
		virtual void contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event);
		virtual void contactPointRemovedCallback( hkpContactPointRemovedEvent& event );

		hkpRigidBody* m_bodyA;
		hkpRigidBody* m_bodyB;
		hkReal		m_currentTime;
		hkPseudoRandomGenerator m_random;

		// position of the objects at the time of collision
		hkVector4	m_collA;
		hkVector4	m_collB;

		//
		// A history of the trajectory of the two balls
		//
		struct Segment
		{
			hkVector4 m_start;
			hkVector4 m_end;
		};

		hkArray<Segment> m_displayVectors;
};

#endif // HK_ContinuousBasicsDemo_H

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
