/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// A demo which shows continuous addition/removal of bodies from a hkpWorld.
// Every NUM_FRAMES frames a body is added to the world. At most NUM_BODIES are allowed in the 
// World, so once NUM_BODIES have been added, the "added" body is recycled by removing "oldest" 
// body, and re-adding it. 
// The actual removal/addition is done in the removeBody()/addBody() methods using calls to
// hkpWorld::removeEntity() and hkpWorld::addEntity().

#ifndef HK_AddRemoveBodiesDemo_H
#define HK_AddRemoveBodiesDemo_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkpShape;
class hkpRigidBody;

static const int NUM_BODIES = 10;
static const int NUM_FRAMES = 60;


class AddRemoveBodiesDemo : public hkDefaultPhysicsDemo
{
	public:

		AddRemoveBodiesDemo(hkDemoEnvironment* env);
		~AddRemoveBodiesDemo();

		Result stepDemo();

	protected:

		void createGround();
		void createBodies();

		void addBody(hkpRigidBody* body);
		void removeBody(hkpRigidBody* body);

	protected:

		hkpRigidBody*	m_bodies[NUM_BODIES];

		int				m_frameCount;
		int				m_currentBody;
};

#endif	// HK_AddRemoveBodiesDemo_H

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
