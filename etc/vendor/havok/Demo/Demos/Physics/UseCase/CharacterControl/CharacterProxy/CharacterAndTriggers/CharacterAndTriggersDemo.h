/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_PHANTOM_AND_TRIGGERS_H
#define HK_PHANTOM_AND_TRIGGERS_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

class hkpShapePhantom;
class hkpCharacterProxy;
class hkpCharacterContext;
class hkColor;

class hkpMoppBvTreeShape;
class MyAllCdPointCollector;

/// This is a test scene for one technique to implement 'phantom' trigger objects
/// for the Havok Character Controller : Using the custom collectors.
/// Rigid bodies which have been 'tagged' with a 'trigger' property are coloured red.
/// Phantoms which have been 'tagged' with a 'trigger' property are transparent red.
///
/// Triggers are marked by assigning them a hkpProperty
/// They are 'recognized' using using a custom collector (MyTriggerIdentifyingPointCollector) for the character controller which
/// examines all hkCdBodies as they are collected for this hkpProperty. If they have the property they
/// are pushed onto a list.
/// This allows us to customize the character controller by passing in this collector for both the 'cast' queries and
/// the 'start' query so that:
/// 1. All triggers are 'collected' during the call to hkpCharacterProxy::integrate(), and not passed on to the controler code.
///		Thus only those shapes (both hkRigidBodies and hkPhantoms) which intersect the character's linear casts, or which are
///		penetrating the character at the start of hkpCharacterProxy::integrate() are flagged as bieng 'hit'.
/// 2. All 'normal' hkPhantoms are ignored, so we can walk through them.
///
/// Thus when we examine the list of objects recorded by the 'start' collector (all of which come from a call to getClosestPoints(),
/// we can identify all those with a negative distance as ones which we are correctly overlapping. And we can be sure that the character 
/// code never mistakenly tries to resolve collisions against these bodies because we didn't record them in our hitlist.
///
/// We remove and re-add each trigger when they are 'hit' so you can see visually that the logic is sound.
///
/// N.B. This is only ONE way of implementing triggers with the character controller. It is more accurate than simply
/// using a phantom (which only uses AABB overlaps). It has the advantage that it 'piggybacks' the collision queries 
/// already used by the character controller, and so you get this collision info 'for free'. 
class CharacterAndTriggersDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		CharacterAndTriggersDemo(hkDemoEnvironment* env);
		
		~CharacterAndTriggersDemo();

		hkDemo::Result stepDemo();

	protected:


		void swapPhantomShape(hkpShape* newShape);

		void triggerifyWorldObject(hkpWorldObject* wo, int color);

			// Helper class to get a flat mesh shape
		hkpMoppBvTreeShape* getTiledPlanarMoppBvTreeShape(int sideDivisions, hkReal sideLength, hkReal radius = 0.05f);

			// Helper method to remove a hkpWorldObject from simulation, and add it back in
		void removeAndReaddObject(hkpWorldObject* wo);

			// Helper method to generate a 'random' shape (type)
		hkpShape* getRandomShape();

			// Helper method to find a position far enough away from the current character position that it's
			// safe to add in a new hkRigidBopdy or Phantom at this spot.
		hkVector4 getRandomPositionAwayFromCharacter(hkReal planerange, hkReal heightrange, hkReal mindist);


		hkpCharacterProxy* m_characterProxy;
		hkpCharacterContext* m_characterContext;
		hkReal m_currentAngle;

		hkpShapePhantom* m_phantom;

		hkpShape* m_standShape;
		hkpShape* m_crouchShape;

		hkReal m_characterVelocity;

		hkReal m_wallDistance;

		hkPseudoRandomGenerator m_generator;
};

#endif // HK_PHANTOM_AND_TRIGGERS_H

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
