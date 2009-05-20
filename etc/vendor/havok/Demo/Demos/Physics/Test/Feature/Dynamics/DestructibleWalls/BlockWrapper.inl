/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

inline hkBool BlockWrapper::findSubBlocks(hkArray<BlockWrapper*>& newBlocks, const BlockRangeFinder& finder)
{
	return findSubBlocks(BrickRange(0, 0, m_blockHeight-1, m_blockWidth-1), newBlocks, finder, 0);
}

inline void BlockWrapper::shiftRange(const BrickPos& offset)
{
	m_brickRange.sub( offset );
	m_blockMin(0) = m_brickRange.minJ*m_brickExtents(0);
	m_blockMin(1) = m_brickRange.minI*m_brickExtents(1);
	m_blockMax(0) = (m_brickRange.maxJ + 1)*m_brickExtents(0);
	m_blockMax(1) = (m_brickRange.maxI + 1)*m_brickExtents(1);
}

inline hkBool BlockWrapper::areOverlappingRanges(BrickRange* r1, BrickRange* r2) const
{
	return ( static_cast<unsigned int>( r1->maxI - r2->minI) <= static_cast<unsigned int>( (r1->maxI - r1->minI) + (r2->maxI - r2->minI) ) );
}


inline bool BlockWrapper::isWholeBlock(const BrickRange& lhs) const
{
	return (lhs.minI==m_brickRange.minI &&  lhs.minJ==m_brickRange.minJ && lhs.maxI==m_brickRange.maxI && lhs.maxJ==m_brickRange.maxJ);
}

int BlockWrapper::getWidth() const
{
	return m_blockWidth;
}

int BlockWrapper::getHeight() const
{
	return m_blockHeight;
}

BrickPos BlockWrapper::getPosition() const
{
	return BrickPos(m_brickRange.minI, m_brickRange.minJ);
}

void BlockWrapper::getPosition(BrickPos& pos) const
{
	pos.i = m_brickRange.minI;
	pos.j = m_brickRange.minJ;
}

int BlockWrapper::getPositionI() const
{
	return m_brickRange.minI;
}

int BlockWrapper::getPositionJ() const
{
	return m_brickRange.minJ;
}


const BrickRange& BlockWrapper::getBrickRange() const
{
	return m_brickRange;
}

void BlockWrapper::getBrickRange(BrickRange& range) const
{
	range = m_brickRange;
}

inline hkBool BlockWrapper::hasBrick(int i, int j) const
{
	return ( ( m_brickRange.minI <= i  && i <= m_brickRange.maxI) &&
		( m_brickRange.minJ <= j  && j <= m_brickRange.maxJ ) );
}

inline hkBool BlockWrapper::hasBrick(const BrickPos& brickPos) const
{
	return hasBrick(brickPos.i, brickPos.j);
}

inline const hkpRigidBody* BlockWrapper::getBrick(const BrickPos& brickPos) const
{
	HK_ASSERT2(0x355d3a62, hasBrick(brickPos) ,"out of block bounds");
	return m_bricks[(brickPos.j-m_brickRange.minJ)*m_blockHeight + (brickPos.i-m_brickRange.minI)];
}

inline hkpRigidBody* BlockWrapper::getBrick(const BrickPos& brickPos)
{
	HK_ASSERT2(0x355d3a62, hasBrick(brickPos) ,"out of block bounds");
	return m_bricks[(brickPos.j-m_brickRange.minJ)*m_blockHeight + (brickPos.i-m_brickRange.minI)];
}

inline const hkpRigidBody* BlockWrapper::getBrick(int ind)
{
	return m_bricks[ind];
}

inline void BlockWrapper::getBricks(hkArray<hkpRigidBody*>& dest) const
{
	dest.insertAt(dest.getSize(), &m_bricks[0], m_bricks.getSize());
}

inline int BlockWrapper::getBricksNum() const
{
	return m_bricks.getSize();
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
