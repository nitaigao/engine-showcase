/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef COLLISION_FILTER_VIEWER_UTIL_H
#define COLLISION_FILTER_VIEWER_UTIL_H

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>

// Typical way to use this in a demo, bound to e.g. the '2' key:
//
// In header: CollisionFilterViewerUtil m_filterViewer;
// In .cpp:
/*
	if( m_env->m_gamePad->isButtonPressed( HKG_PAD_BUTTON_2 ) )
	{
		// Make sure filter is up-to-date, just for sanity's sake.
		m_world->updateCollisionFilterOnWorld(HK_UPDATE_FILTER_ON_WORLD_FULL_CHECK, HK_UPDATE_COLLECTION_FILTER_PROCESS_SHAPE_COLLECTIONS);

		if( !m_filterViewer.isActive() )
		{
			hkpRigidBody* rb = CollisionFilterViewerUtil::getBodyUnderMousePointer(m_env, m_world);
			m_filterViewer.activate( rb, m_world );
		}
	}
	else
	{
		if( m_filterViewer.isActive() )
		{
			m_filterViewer.deactivate();
		}
	}
*/

// This viewer utility will allow you to color all bodies currently 
// filtered to *collide* with another body ORANGE
// All bodies *not colliding* with that body will be coloured GREEN
// The body itself will be colored red.
class CollisionFilterViewerUtil
{

public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, CollisionFilterViewerUtil);

	CollisionFilterViewerUtil();

	hkBool isActive() const;
	
	// Helpers to select a body
	static void HK_CALL castRayIntoWorldAtMousePointer(const hkDemoEnvironment* env, const hkpWorld* world, hkpWorldRayCastInput& input, hkpWorldRayCastOutput& output);
	static hkpRigidBody* HK_CALL  getBodyUnderMousePointer(const hkDemoEnvironment* env, const hkpWorld* world);

	// Color all bodies in world appropriately
	void activate(const hkpRigidBody* rb, const hkpWorld* world);

	// Set colors back
	void deactivate();

private:
	hkArray<const hkpRigidBody*> m_bodiesColored;
	hkArray<hkInt32> m_colors;
	hkBool m_isActive;

};

#endif //COLLISION_FILTER_VIEWER_UTIL_H

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
