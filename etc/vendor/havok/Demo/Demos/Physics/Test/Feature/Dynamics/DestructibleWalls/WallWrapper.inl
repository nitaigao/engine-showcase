/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline int WallWrapper::getID() const
{
	return m_wallID;
}

inline hkBool WallWrapper::hasSubBlocks() const
{
	return (m_blocks.getSize() > 1);
}

inline int WallWrapper::getNumBlocks() const
{
	return m_blocks.getSize();
}

inline const BlockWrapper* WallWrapper::getBlock(int i) const
{
	return m_blocks[i];
}

inline BrickPos WallWrapper::getBlockPos(int blockInd) const
{
	return BrickPos( getBlock(blockInd)->getBrickRange().minI , getBlock(blockInd)->getBrickRange().minJ );
}

inline hkpRigidBody * const * WallWrapper::getBricks() const
{
	return m_bricks.begin();
}

inline const hkpRigidBody& WallWrapper::getBrick(int brickInd) const
{
	HK_ASSERT2(0x7bfb0599, (brickInd >=0 && brickInd < getNumBricks()), "Invalid index");
	return *(m_bricks[brickInd]);
}

inline hkArray<hkpConstraintInstance*> const * WallWrapper::getConstraints() const
{
	return &m_constraints;
}

inline int WallWrapper::getNumBricks() const
{
	return m_bricks.getSize();
}

inline int WallWrapper::getHeight() const
{
	return m_wallHeight;
}

inline int WallWrapper::getWidth() const
{
	return m_wallWidth;
}


inline void WallWrapper::getBrickExtents(hkReal& extentX, hkReal& extentY) const
{
	extentX = m_brickExtents(0);
	extentY = m_brickExtents(1);
}

inline const hkReal& WallWrapper::getBrickExtentsX() const
{
	return m_brickExtents(0);
}

inline const hkReal& WallWrapper::getBrickExtentsY() const
{
	return m_brickExtents(1);
}

inline const hkVector4& WallWrapper::getWallHalfExtents() const
{
	return m_wallHalfExtents;
}


inline bool WallWrapper::isInside(hkVector4& point) const
{
	BlockWrapper* b;
	return isInsideGetBlock(point, &b);
}

inline void WallWrapper::getAllBricksPositions(hkArray<hkVector4>& positions) const
{
	if(!m_bricksPositions.isEmpty())
		positions.insertAt( positions.getSize(), &m_bricksPositions[0], m_bricksPositions.getSize() );
}


inline const BrickPos& WallWrapper::getMinIJ() const
{
	return m_minIJ;
}

inline void WallWrapper::getMinIJ(BrickPos& minIJ) const
{
	minIJ = m_minIJ;
}

inline int WallWrapper::getMinI() const
{
	return m_minIJ.i;
}

inline int WallWrapper::getMinJ() const
{
	return m_minIJ.j;
}

hkBool WallWrapper::isValidID(int id)
{
	return (id > 0);
}

inline hkBool WallWrapper::isOneBrick() const
{
	return ( m_bricks.getSize()==1 );
}

inline const hkVector4& WallWrapper::getCenterOfMass() const
{
	return m_centerOfMass;
}

inline hkBool WallWrapper::isAttachedToGround() const
{
	return m_isAttachedtoGround;
}

inline void WallWrapper::attachToGround(hkpRigidBody* ground)
{
	m_theGround = ground;
	m_isAttachedtoGround = true;
}

inline hkpRigidBody* WallWrapper::getGround() const
{
	return m_theGround;
}

inline void WallWrapper::toggleFracturableState( hkBool fracturable )
{
	m_fracturable = fracturable;
}

inline hkBool WallWrapper::isFracturable() const
{
	return m_fracturable;
}

inline void WallWrapper::makeShapes(const hkArray<WallWrapper*>& newWallsWrappers, hkArray<const hkpShape*>& shapesOut, hkArray<hkReal>& wallMassesOut ) const
{
	int numElements = newWallsWrappers.getSize();
	shapesOut.setSize(numElements);
	wallMassesOut.setSize(numElements);
	// build the shape for each wall wrapper and compute mass
	for(int i=0; i<numElements; ++i)
	{
		makeShape(newWallsWrappers[i], &(shapesOut[i]), wallMassesOut[i]);
	}
}

inline void WallWrapper::makeWall( const hkpShape* const shape, const hkReal& wallMass, int wallID ,hkpRigidBody** newWall, hkpRigidBodyCinfo& info ) const
{
	// create the wall rigid body and set filter info
	BrickWallBuilder::computeBrickInfo(shape, wallMass, info);
	(*newWall) = new hkpRigidBody(info);
	(*newWall)->setCollisionFilterInfo( BrickFilter::calcFilterInfo(0, 0, wallID) );
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
