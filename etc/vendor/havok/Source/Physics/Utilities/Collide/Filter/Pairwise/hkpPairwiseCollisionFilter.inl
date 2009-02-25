/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


inline hkpPairwiseCollisionFilter::CollisionPair::CollisionPair(hkpEntity* a, hkpEntity* b)
{
	HK_ASSERT2(0x35e4d82a, a != b, "Collision pairs must be between two different collidables");

	if (a > b)
	{
		m_a = a;
		m_b = b;
	}
	else
	{
		m_a = b;
		m_b = a;
	}
}


inline hkBool operator == (const hkpPairwiseCollisionFilter::CollisionPair& a, const hkpPairwiseCollisionFilter::CollisionPair& b)
{
	return ((a.m_a == b.m_a) && (a.m_b == b.m_b)); 
}

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20080925)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2008
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
