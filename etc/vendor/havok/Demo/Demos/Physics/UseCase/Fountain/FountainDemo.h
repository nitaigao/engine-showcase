/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_FOUNTAIN_H
#define HK_FOUNTAIN_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Dynamics/World/Listener/hkpWorldPostSimulationListener.h>
#include <Physics/Dynamics/World/Listener/hkpWorldDeletionListener.h>

class hkpAabbPhantom;

/// A demo which uses a phantom object to act as a "fountain", applying upward forces to anything which
/// falls inside it.
/// Additionally, this game is a "SimulationListener", and when notified by the world that a body has
/// moved outside the world limits, it warps it back to the center of the "fountain phantom"
/// Initially, many bodies are interpenetrating, which creates an explosion-like effect
class FountainDemo : public hkDefaultPhysicsDemo, protected hkpWorldPostSimulationListener, protected hkpWorldDeletionListener
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		FountainDemo(hkDemoEnvironment* env);
		~FountainDemo();

	protected:

			/// Called at the end of the hkpWorld::simulate call, Attention: this might change
		virtual void postSimulationCallback( hkpWorld* world );

		virtual void worldDeletedCallback( hkpWorld* world );


		hkpAabbPhantom*	m_phantom;
		hkReal			m_localTime;
};

#endif // HK_FOUNTAIN_H

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
