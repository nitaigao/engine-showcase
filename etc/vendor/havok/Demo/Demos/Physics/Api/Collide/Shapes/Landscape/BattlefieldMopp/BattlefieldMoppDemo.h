/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_BATTLEFIELDMOPPDEMO_H
#define HK_BATTLEFIELDMOPPDEMO_H



#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Common/Base/Thread/CriticalSection/hkCriticalSection.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>



	/// A demo on how to modify an existing terrain on-the-fly.
class BattlefieldMoppDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		BattlefieldMoppDemo(hkDemoEnvironment* env);
		~BattlefieldMoppDemo();

	protected:

		friend class BattlefieldTerrainModifier;

	protected:


		struct Explosions
		{
			struct Data
			{
				hkpEntity* m_bomb;
				hkVector4 m_position;
			};

			hkCriticalSection	m_criticalSection;
			hkArray<Data>		m_explosionsData;

			Explosions() : m_criticalSection(1000) {}
		};

	protected:

		hkDemo::Result stepDemo();

		void processExplosions();

		void processSingleExplosion(const Explosions::Data* explosion);

		void removeTrianglesWithinSphere(hkpRigidBody& rigidBody, hkpBvTreeShape& moppShape, const hkVector4& sphereCenterWorldSpace, float sphereRadius, hkArray<hkpTriangleShape*>& collectedTrianglesWithinRange);

			// This method simply removes the supplied triangles from the object's visualization.
		void removeTrianglesFromDisplayObject(hkpRigidBody& rigidBody, hkpBvTreeShape& bvTreeShape, hkArray<hkpShapeKey>& trianglesToRemove);


	protected:

		hkPseudoRandomGenerator	m_randomGenerator;
		hkReferencedObject*		m_terrain;
		Explosions				m_explosions;
};



#endif // HK_BATTLEFIELDMOPPDEMO_H


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
