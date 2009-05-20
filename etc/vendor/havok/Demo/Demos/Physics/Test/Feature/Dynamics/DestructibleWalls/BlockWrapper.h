/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef BLOCK_WRAPPER
#define BLOCK_WRAPPER

#include <Demos/DemoCommon/Utilities/GameUtils/hkMap.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BlockRangeFinder.h>

class hkUnionFind;



// Wrapper for a rectangular block of a brick wall 
// Important Note : it assumes all bricks are ordered from bottom to top, from left to right
// i.e. the first is the bottom-leftmost, and going on column by column
class BlockWrapper 
{
public :
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, BlockWrapper);

	BlockWrapper(const hkArray<hkpRigidBody*>& bricks, const BrickRange& range, const hkVector4& brickExtents);
	~BlockWrapper();

	inline int getWidth() const;
	inline int getHeight() const;
	inline BrickPos getPosition() const;
	inline void getPosition(BrickPos& pos) const;
	inline int getPositionI() const;
	inline int getPositionJ() const;
	inline const BrickRange& getBrickRange() const;
	inline void getBrickRange(BrickRange& range) const;

	// access to a brick at brickPos
	inline const hkpRigidBody* getBrick(const BrickPos& brickPos) const;

	// non const access
	inline hkpRigidBody* getBrick(const BrickPos& brickPos);
	inline const hkpRigidBody* getBrick(int ind);

	
	// true if the brick with the specified position is in this block
	inline hkBool hasBrick(const BrickPos& brickPos) const;
	inline hkBool hasBrick(int i, int j) const;
	bool isInside(hkVector4& point) const;

	// Retrieves the detached sub-blocks and removes the data from
	// the block. Can be called just one time because after the first call 
	// the structure may be corrupted (deleted bricks are just pointers to NULL). you should call FindSubBlocks and 
	// replace the block with the result.
	// returns true if at least one detached block is found
	hkBool extractDetachedBlocks(hkObjectArray< hkArray<BlockWrapper*> >& newBlocks, hkUnionFind& unionFinder, int numGroups, int parentsOffset);

	// Finds sub-blocks (maybe useful after removal of sub-blocks or when a new wall is built from the block)
	// returns true if at least one sub-block is found
	inline hkBool findSubBlocks(hkArray<BlockWrapper*>& newBlocks, const BlockRangeFinder& finder);
	hkBool findSubBlocks(const BrickRange& range, hkArray<BlockWrapper*>& newBlocks, const BlockRangeFinder& finder, int parentsOffset) const;

	inline void shiftRange(const BrickPos& offset);

	// copies m_bricks contents in dest
	inline void getBricks(hkArray<hkpRigidBody*>& dest) const;

	// returns the number of bricks of this block
	inline int getBricksNum() const;

private:
	// Compute the width and height of this block from the bricks.
	// Again it assumes bricks are ordered from bottom to top, left to right
	void computeDimensions();

	// returns true if the range of bricks positions identifies the whole block
	inline bool isWholeBlock(const BrickRange& lhs) const;

	// Builds a sub-block of this block invalidating copied bricks 
	BlockWrapper* buildSubBlock(const BrickRange& brickInfo) const;

	// returns true if two ranges are overlapping
	inline hkBool areOverlappingRanges(BrickRange* r1, BrickRange* r2) const;
	// merge ranges and creates identifies new blocks, storing block-info 
	void mergeRanges(hkArray<BrickRange*>& startRanges, hkArray<BrickRange*>& endRanges, hkArray<BrickRange*>& blocksInfo) const;

private:
	// block extents as number of bricks
	int m_blockWidth;
	int m_blockHeight;

	// lower left corner of this block
	hkVector4 m_blockMin;
	hkVector4 m_blockMax;

	// brick extents
	const hkVector4 m_brickExtents;
	
	// the range of brick (i,j) positions of this block's bricks in the original wall space
	BrickRange m_brickRange;
	// the bricks of this block
	hkArray<hkpRigidBody*> m_bricks;
};


#include<Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BlockWrapper.inl>

#endif

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
