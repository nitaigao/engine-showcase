/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>


#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BlockRangeFinder.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>

BlockRangeFinder::BlockRangeFinder(const BrickRange& blockRange):
m_blockRange(blockRange),
m_blockHeight(blockRange.maxI - blockRange.minI + 1),
m_blockWidth(blockRange.maxJ - blockRange.minJ + 1)
{
	
}

hkBool BlockRangeFinder::findRanges(int baseInd, int lastInd, hkArray<BrickRange*>& endRanges, int baseOffset) const
{
	bool foundRange=false;
	bool hole=false;
	BrickRange* br = HK_NULL;
	// the wall colon we are in
	int currentCol = static_cast<int>(floor(static_cast<hkReal>( (baseInd-baseOffset) / m_blockHeight)));

	HK_ASSERT2(0x6636f385, currentCol==static_cast<int>(floor(static_cast<hkReal>( (lastInd-baseOffset)  / m_blockHeight))), "Ranges can be searched just in one column");

	int currentRow = (baseInd-baseOffset) - currentCol*m_blockHeight;

	HK_ASSERT2(0x6f740668, ((currentCol >=0)&&(currentCol <= m_blockWidth)), "invalid column index");
	HK_ASSERT2(0x6ef8eac2, ((currentRow >=0)&&(currentRow < m_blockHeight)), "Invalid row index");

	currentCol += m_blockRange.minJ;
	currentRow += m_blockRange.minI;

	while(baseInd <= lastInd)
	{
		if( isRangeElement( baseInd ) )
		{
			// the brick is invalid
			if(hole)
			{
				// if we are already in a range update the range end
				br->maxI = currentRow;
			} else {
				// if it's a new hole
				hole=true;
				// create a new range
				br = new BrickRange(currentRow, currentCol, currentRow, currentCol);
			}
		} else // the brick is not missing
			if(hole) 
			{
				// it's the end of a range
				hole = false;
				foundRange=true;
				endRanges.pushBack(br);
				br=HK_NULL;
			}
		++baseInd;
		++currentRow;
	}// end of while
	if(hole) // if the column ended with a hole we must close and add the range	
	{
		foundRange=true;
		endRanges.pushBack(br);
	}

	return foundRange;	
}


BlockRangeFinderHoles::BlockRangeFinderHoles(const BrickRange& blockRange, hkArray<hkpRigidBody*>* bricks):
BlockRangeFinder(blockRange),
m_bricks(bricks)
{
}




BlockRangeFinderGroups::BlockRangeFinderGroups(const BrickRange& blockRange, int searchedGroup, hkFixedArray<int>* groups):
BlockRangeFinder(blockRange),
m_groups(groups),
m_searchedGroup(searchedGroup)
{
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
