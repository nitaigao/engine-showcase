/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>

#include <Demos/DemoCommon/Utilities/GameUtils/CollisionFilterViewerUtil/CollisionFilterViewerUtil.h>
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>

#include <Common/Visualize/hkDebugDisplay.h>

static const unsigned int DEFAULT_OBJECT_COLOR = hkColor::rgbFromChars( 240, 240, 240, 255 );
static const unsigned int DEFAULT_FIXED_OBJECT_COLOR = hkColor::rgbFromChars( 120, 120, 120, 255 );

CollisionFilterViewerUtil::CollisionFilterViewerUtil()
{
	m_isActive = false;
}

hkBool CollisionFilterViewerUtil::isActive() const
{
	return m_isActive;
}

void HK_CALL CollisionFilterViewerUtil::castRayIntoWorldAtMousePointer(const hkDemoEnvironment* env, const hkpWorld* world, hkpWorldRayCastInput& input, hkpWorldRayCastOutput& output)
{
	hkgWindow* w = env->m_window;
	hkgViewport* v = w->getCurrentViewport();
	hkgCamera* c = v->getCamera();
	const hkgMouse& m = w->getMouse();

	int vx, vy;
	v->getLowerLeftCoord(vx, vy);

	const int x = m.getPosX() - vx;
	const int y = m.getPosY() - vy;

	hkVector4 from;
	hkVector4 dir;
	hkVector4 to;

	c->unProject( (x - vx), (y - vy), 0, v->getWidth(), v->getHeight(), &from(0) );
	c->unProject( (x - vx), (y - vy), 1, v->getWidth(), v->getHeight(), &to(0) );

	dir.setSub4( to, from );
	dir.normalize3();
	to.setAddMul4( from, dir, 5000.0f );

	hkVector4	normal;

	input.m_from = from;
	input.m_to = to;

	world->castRay(input, output );
}

hkpRigidBody* CollisionFilterViewerUtil::getBodyUnderMousePointer(const hkDemoEnvironment* env, const hkpWorld* world)
{
	hkpWorldRayCastInput input;
	hkpWorldRayCastOutput output;
	hkVector4 intersectionPointWorld;
	castRayIntoWorldAtMousePointer(env, world, input, output);

	if(output.hasHit())
	{
		hkpRigidBody* rb = hkGetRigidBody(output.m_rootCollidable);
		return rb;
	}

	return HK_NULL;
}

void CollisionFilterViewerUtil::activate(const hkpRigidBody* rb, const hkpWorld* world)
{
	if( rb != HK_NULL )
	{
		m_isActive = true;

		if(m_bodiesColored.getSize() == 0)
		{
			// Print out filter info 
			{
				hkUint32 filterInfo = rb->getCollidable()->getCollisionFilterInfo();
				hkcout << "Body has filter info " << filterInfo << 
					" which means layer " << hkpGroupFilter::getLayerFromFilterInfo(filterInfo) <<
					" system group " << hkpGroupFilter::getSystemGroupFromFilterInfo(filterInfo) <<
					" subsystem group " << hkpGroupFilter::getSubSystemIdFromFilterInfo(filterInfo) <<
					" subsystem group nocollide " << hkpGroupFilter::getSubSystemDontCollideWithFromFilterInfo(filterInfo) <<
					"\n";
			}
			// Color the delected body red
			HK_SET_OBJECT_COLOR((hkUlong)rb->getCollidable(), hkColor::rgbFromChars(255, 0, 0));

			// Store original color
			m_bodiesColored.pushBack(rb);
			m_colors.pushBack( rb->isFixed() ? DEFAULT_FIXED_OBJECT_COLOR : DEFAULT_OBJECT_COLOR );

			// Now find all bodies colliding with this and colour orange, otherwise green
			hkpPhysicsSystem* system;
			system = world->getWorldAsOneSystem();

			for(int i = 0; i < system->getRigidBodies().getSize(); i++)
			{
				hkpRigidBody* rb2 = system->getRigidBodies()[i];

				if( rb == rb2) continue;

				if( world->getCollisionFilter()->isCollisionEnabled( *rb->getCollidable(), *rb2->getCollidable()) )
				{
					// Color orange
					HK_SET_OBJECT_COLOR((hkUlong)rb2->getCollidable(), hkColor::rgbFromChars(255, 128, 0));
				}
				else
				{
					// Color Green
					HK_SET_OBJECT_COLOR((hkUlong)rb2->getCollidable(), hkColor::rgbFromChars(0, 255, 0));
				}
				m_bodiesColored.pushBack(rb2);
				m_colors.pushBack(  rb2->isFixed() ? DEFAULT_FIXED_OBJECT_COLOR : DEFAULT_OBJECT_COLOR );
			}
			system->removeReference();

		}
	}
}


void CollisionFilterViewerUtil::deactivate()
{
	// Recolor all boides to their original color.
	for(int i = 0; i < m_bodiesColored.getSize(); i++)
	{
		HK_SET_OBJECT_COLOR((hkUlong)m_bodiesColored[i]->getCollidable(), m_colors[i]);
	}

	// Clear and mark as inactive.
	m_bodiesColored.clear();
	m_colors.clear();
	m_isActive = false;
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
