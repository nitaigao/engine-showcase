/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>



#include <Physics/Collide/hkpCollide.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BrickFilter.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>
#include <Physics/Collide/Shape/Compound/Tree/hkpBvTreeShape.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Shape/Compound/Tree/hkpBvTreeShape.h>
#include <Common/Base/Reflection/hkTypeInfo.h>

// actual collision fitering function, the member functions of the interface call this one
hkBool BrickFilter::isCollisionEnabled( hkUint32 infoA, hkUint32 infoB ) const 
{
	bool sameWall = ((infoA^infoB) & 0x3fffc) == 0;
	if(!sameWall) // different walls => collision enabled
		return true;

	// extract constraint info
	int constrMaskA = infoA & 0x3;
	int constrMaskB = infoB & 0x3;
	
	// if there aren't any constraints enable coll.
	if( !(constrMaskA | constrMaskB) )
		return true;
	
	// extract position info
	int IA = (infoA >> 25) & 0x7f;
	int IB = (infoB >> 25) & 0x7f;
	int JA = (infoA >> 18) & 0x7f;
	int JB = (infoB >> 18) & 0x7f;

	if( /*abs(JA - JB) < 2*/ JA == JB ) 
	{
		// bricks on the same col
		// if there is a constraint check if they are neighbour in the same direction, in that case disable collisions
		if( (IA == IB+1) && (constrMaskA & 0x2) ) return false;
		else if( (IA+1 == IB) && (constrMaskB & 0x2) ) return false;
	} else if( /*abs(IA - IB) < 2*/ IA == IB) 
	{
		// bricks on the same row
		// if neighbours check constraint mask, if there is a constraint disable collisions
		if( (JA == JB+1) && (constrMaskA & 0x1) ) return false;
		else if( (JA+1 == JB) && (constrMaskB & 0x1) ) return false;
	}

	return true;
}

int BrickFilter::removeConstaintFromMask(hkpRigidBody* a, hkpRigidBody* b) 
{
	hkUint32 infoA = a->getCollisionFilterInfo();
	hkUint32 infoB = b->getCollisionFilterInfo();
	
	// extract position info
	int IA = (infoA >> 25) & 0x3f;
	int IB = (infoB >> 25) & 0x3f;
	int JA = (infoA >> 18) & 0x3f;
	int JB = (infoB >> 18) & 0x3f;
	
	if( (JA==JB) && (IA==IB) )
		return 0;

	// extract constraint info
	int constrMaskA = infoA & 0x3;
	//int constrMaskB = infoB & 0x3;

	int res=0;
	if( JA == JB) 
	{
		// bricks on the same column
		// check if there is a constraint and they are neighbor in the same direction, in that case delete constraint 
   		if( (IA == IB+1) && (constrMaskA & 0x1) ) 
		{
			removeConstraintAndUpdateInfo(constrMaskA, 0x1, infoA, a);
			res=1;
		}
		/*else if( (IA == IB-1) && (constrMaskB & 0x1) ) 
		{
			removeConstraintAndUpdateInfo(constrMaskB, 0x1, infoB, b);
			res=2;
		}*/
	} else if( IA == IB) 
	{
		// bricks on the same col
		// if neighbours check constraint mask, if there is a constraint disable collisions
		if( (JA == JB+1) && (constrMaskA & 0x2) ) 
		{
			removeConstraintAndUpdateInfo(constrMaskA, 0x2, infoA, a);
			res=1;
		}
		/*else if( (JA == JB-1) && (constrMaskB & 0x2) ) 
		{
			removeConstraintAndUpdateInfo(constrMaskB, 0x2, infoB, b);
			res=2;
		}*/
	}
	return res;
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
