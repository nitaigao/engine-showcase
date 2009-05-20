/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// A demo which shows how to dynamically update the friction of a rigid body, including all contacts associated with the body.
// Merely changing the body's material property will not correctly update the friction associated with any contact point which
// currently exist between the body and other bodies.
//
// It is necessary to call hkpContactUpdater::updateContacts() on the body as Havok will cache all contact data to speed up
// subsequent accesses. Therefore we need to explicitly tell Havok to disregard the previously cached data and update the
// contact cache to take into account the new information.

#ifndef HK_FrictionChangeDemo_H
#define HK_FrictionChangeDemo_H


#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkpRigidBody;


class FrictionChangeDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		FrictionChangeDemo(hkDemoEnvironment* env);
		
		Result stepDemo();

	protected:

		hkpRigidBody* m_movingBody;
		hkInt32 m_frictionChangePeriod;
		hkInt32 m_frameCount;
};

#endif	// HK_FrictionChangeDemo_H

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
