/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef KD_TREE_DEMO_UTILS_H
#define KD_TREE_DEMO_UTILS_H

#include <Common/Internal/KdTree/QueryUtils/hkKdTreeUtils.h>

class KdTreeDemoUtils
{
	public:

		static void HK_CALL createRandomBodies( hkpWorld* world, const class hkAabb& worldAabb, int numBodies, hkpMotion::MotionType motionType, class hkPseudoRandomGenerator* prng, hkArray<const hkpCollidable*>& collidablesOut );
		static void HK_CALL makeRaysForBundle(hkVector4& startIn, hkVector4& endIn, hkVector4* startsOut, hkVector4* endsOut, hkReal variance, class hkPseudoRandomGenerator& rand );
		static void HK_CALL makeBundleFromInputs(hkpWorldRayCastInput* inputs, hkKdTreeUtils::RayBundle& fromBundle, hkKdTreeUtils::RayBundle& toBundle);

};

#endif //KD_TREE_DEMO_UTILS_H

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
