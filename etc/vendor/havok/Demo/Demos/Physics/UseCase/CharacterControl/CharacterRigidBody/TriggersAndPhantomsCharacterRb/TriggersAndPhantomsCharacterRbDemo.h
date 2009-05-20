/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_TRIGGERS_AND_PHANTOMS_CHARACTER_RIGIDBODY_H
#define HK_TRIGGERS_AND_PHANTOMS_CHARACTER_RIGIDBODY_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

class hkpRigidBody;
class hkpCharacterRigidBody;
class hkpCharacterContext;
class hkpSimpleShapePhantom;
class MyPhantomForCharacterRbListener;


class TriggersAndPhantomsCharacterRbDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		TriggersAndPhantomsCharacterRbDemo(hkDemoEnvironment* env);
		
		~TriggersAndPhantomsCharacterRbDemo();

		// Collision Layers used in this demo.
		// By enabling and disabling collisions between these layers, we can tweak the behaviour of the demo
		enum
		{
			LAYER_GROUND = 1,
			LAYER_CHARACTER = 2,
			LAYER_RIGIDBODIES = 3,
			LAYER_PHANTOMS = 4			
		};

		hkDemo::Result stepDemo();


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

private:

		hkReal m_currentAngle;

		hkpShape* m_standShape;
		
		hkpCharacterRigidBody* m_characterRigidBody;

		hkpCharacterContext* m_characterContext;

		hkArray<hkpSimpleShapePhantom*> m_phantoms;
		hkArray<MyPhantomForCharacterRbListener*> m_phantomListeners;
		
		hkPseudoRandomGenerator m_generator;
};

#endif // HK_TRIGGERS_AND_PHANTOMS_CHARACTER_RIGIDBODY_H

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
