/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkUint32 HK_CALL BrickFilter::calcConstraintMask(bool leftconstr, bool bottconstr)
{
	hkUint32 constrMask=0;
	constrMask |= (leftconstr  ? 1 : 0) << 1;
	constrMask |= (bottconstr  ? 1 : 0) ;

	// the same
	//hkUint32 constrMask  = ( ((leftconstr  ? 1 : 0) << 1) | ((bottconstr  ? 1 : 0)) );

	return constrMask;
}

inline hkUint32 HK_CALL BrickFilter::calcFilterInfo(hkUint32 i, hkUint32 j, hkUint32 wallID, hkUint32 constrMask) 
{
	return hkUint32(constrMask | (wallID<<2) | (j<<18) | (i<<25) );
}

inline hkUint32 HK_CALL BrickFilter::calcFilterInfo(hkUint32 i, hkUint32 j, hkUint32 wallID, bool leftconstr, bool bottconstr) 
{
	return calcFilterInfo( i, j, wallID, calcConstraintMask(leftconstr, bottconstr) );
}

inline void HK_CALL BrickFilter::setMask(hkUint32& info, hkInt32 mask) {
	info &= 0xfffffffC;
	info &= mask;
}

inline void HK_CALL BrickFilter::setWallID(hkUint32& info, int wallID)
{
	info &= 0xfffc0003;
	info |= ( wallID << 2 );
}


inline void BrickFilter::removeConstraintAndUpdateInfo(hkUint32 mask, hkUint32 direction, hkUint32 info, hkpRigidBody* b) 
{
	mask ^= direction;
	info &= (~3 | mask);
	b->setCollisionFilterInfo(info);
}

inline void BrickFilter::extractPositionFromInfo(const hkpRigidBody& brick, int& i, int& j)  
{
	hkUint32 info = brick.getCollisionFilterInfo();

	i = (info >> 25) & 0x7f;
	j = (info >> 18) & 0x7f;
}


inline hkBool BrickFilter::hasLeftConstraint(const hkpRigidBody& brick) 
{
	return ( ( (brick.getCollisionFilterInfo() & 0x3) & 0x2) != 0 );
}

inline hkBool BrickFilter::hasBottomConstraint(const hkpRigidBody& brick) 
{
	return ( ( (brick.getCollisionFilterInfo() & 0x3) & 0x1) != 0 );
}

inline int BrickFilter::getWallID(const hkpRigidBody& brick)
{
	/*hkUint32 tmp = brick.getCollisionFilterInfo();
	tmp &= 0x3fffc;
	int tmp2 = tmp >> 2;
	return ( tmp2 );*/
	return ((brick.getCollisionFilterInfo() & 0x3fffc) >> 2);
}

inline hkBool BrickFilter::isCollisionEnabled( const hkpCollidable& a, const hkpCollidable& b ) const 
{ 
	return isCollisionEnabled( a.getCollisionFilterInfo(), b.getCollisionFilterInfo() );
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
