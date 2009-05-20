/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Physics/Dynamics/Constraint/Breakable/hkpBreakableConstraintData.h>


inline void BrickWallBuilder::computeBrickPropertiesShape(const hkpShape* shape, const hkReal& mass, hkpMassProperties& result) 
{
	hkpInertiaTensorComputer::computeShapeVolumeMassProperties(shape, mass, result);
}

inline void BrickWallBuilder::computeBrickPropertiesBox(const hkpBoxShape* shape, const hkReal& mass, hkpMassProperties& result)
{
	hkpInertiaTensorComputer::computeBoxVolumeMassProperties(shape->getHalfExtents(), mass, result);
}


inline hkpConstraintInstance* BrickWallBuilder::buildConstraintInstance(hkpRigidBody* body1, hkpRigidBody* body2) 
{
	hkFixedConstraintData* cData = makeContraintData(body1, body2);
	hkpConstraintInstance* constraint = new hkpConstraintInstance( body1, body2, cData );
	cData->removeReference();

	return constraint;
}


inline hkpConstraintInstance* BrickWallBuilder::buildBreakableConstraintInstance(hkpRigidBody* body1, hkpRigidBody* body2, const hkReal& threshold) 
{
	hkFixedConstraintData* bs = makeContraintData(body1, body2);
	// Create breakable constraint wrapper
	hkpBreakableConstraintData* breaker = makeConstraintBreakable(bs,threshold);
	// and build constraint
	hkpConstraintInstance* constraint = new hkpConstraintInstance(body1, body2, breaker);
	breaker->removeReference();
	bs->removeReference();

	return constraint;
}

inline hkpBoxShape* StiffWallBuilder::createBoxShape(int width, int height, const hkVector4& brickHalfExtents)
{
	hkVector4 halfExtents( brickHalfExtents );
	halfExtents(0) *= width;
	halfExtents(1) *= height;
	return new hkpBoxShape(halfExtents,0);
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
