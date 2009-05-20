/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SIMPLE_FRACTURE_DEMO_H
#define HK_SIMPLE_FRACTURE_DEMO_H


#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Fracture/EntityTimeQueue.h>

/// A demo which shows to how to use some simple plane equations utility
/// to create some fake fracture
class SimpleFracturePatternsDemo : public hkDefaultPhysicsDemo
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		SimpleFracturePatternsDemo(hkDemoEnvironment* env);

		~SimpleFracturePatternsDemo();

		void makeFakeInput();
		Result stepDemo(); 


	protected:

			// A fracture listener.
		class FractureCollisionListener* m_collisionListener;

		hkReal m_shootingDirX;
		hkReal m_shootingDirY;
		int m_generatorSeed;
		int m_gunCounter;

		hkArray<hkpRigidBody*> m_fracturedBodies;

		EntityTimeQueue m_entitiesWaitingForCollisionListeners;
};

#endif // HK_SIMPLE_FRACTURE_DEMO_H

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
