/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CHAR_PHANTOM_INTERACTION_H
#define HK_CHAR_PHANTOM_INTERACTION_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkpShapePhantom;
class hkpCharacterProxy;
class hkpCharacterContext;
class hkColor;

class hkpMoppBvTreeShape;

// The simplest (and least accurate) technique is simply to listen for overlapping collidables
// and identify either rigid bodies or phantoms as being defined as "triggers". 
// Very little code is needed, and the logic is very simple, however the "accuracy" of overlap is 
// determined entirely by the tightness of fit of the AABB of the character and trigger since we're only
// actually using a broadphase overlap. This cause four artifacts:
//
// 1. If either the character or trigger is badly approximated by 
// an AABB, we might wish to use another method. You can see this by moving towards the angled box 
// hanging in the air (a fixed RB). The trigger overlap is fired as soon as the AABB of the character 
// overlaps this body's RB which is clearly not when the capsule itself overlaps the angled box.
//
// 2. Another problem is that "overlaps" are detected whenever the character controller actually
// does its "linear cast to see where we can go" call, so even it it never actually gets there, it may pick 
// up extra objects. This artifact will become much more apparent the faster the character moves 
// (because this causes the AABB of the linear cast to be much larger as it must encompass the AABB of the
// character at both the start and end of query). You can see this by walking up to the "wall" of triangles in the
// air - although the AABB of the phantom and the trigger behind the wall never actually overlap, the 
// length of the query when running up to the wall is sufficient to hit the 'trigger'. Note 
// that sliding along the wall won't cause an overlap since the velocity is clipped enough by the
// wall's 'plane' constraint to ensure the query AABB does not overlap the trigger.
//
// 3. A similar problem is that it will pick up objects when moving diagonally even though its AABB 
// never actually overlaps the object's AABB at any point along the path from start to end position
// again because the query encompassess the start and end AABB.
//
// 4. A final artifact is a result of the fix of the bug HVK-1360. The size of the AABB used by the phantom
//  now changes between calls (a bigger aabb is used for hkpSimpleShapePhantom::setPositionAndLinearCast() than for 
// hkpSimpleShapePhantom::setPosition()). The difference in size if (by default) 0.2 all the way around (see 
// hkpSimpleShapePhantom::setPositionAndLinearCast()). This means that if you are within this ditance of another object 
// you will get 'new' overlaps constantly added and removed. This may not be a problem for triggers.
class CharacterPhantomInteractionDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		CharacterPhantomInteractionDemo(hkDemoEnvironment* env);
		
		~CharacterPhantomInteractionDemo();

		hkDemo::Result stepDemo();

	protected:


		void swapPhantomShape(hkpShape* newShape);

		void triggerifyWorldObject(hkpWorldObject* wo, int color);

		hkpMoppBvTreeShape* getTiledPlanarMoppBvTreeShape(int sideDivisions, hkReal sideLength);


		hkpCharacterProxy* m_characterProxy;

		hkpCharacterContext* m_characterContext;

		hkReal m_currentAngle;

		hkpShapePhantom* m_phantom;

		hkpShape* m_standShape;

		hkpShape* m_crouchShape;

		hkObjectArray<hkString> m_strings;

		int m_frameNumber;
		
		hkReal m_characterVelocity;

		hkReal m_wallDistance;

};

#endif // HK_CHAR_PHANTOM_INTERACTION_H

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
