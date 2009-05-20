/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>


#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BlockWrapper.h>
#include <Common/Base/Container/LocalArray/hkLocalBuffer.h>
#include <Common/Base/Container/PointerMap/hkPointerMap.h>
#include <Common/Base/Algorithm/Sort/hkSort.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BrickFilter.h>
#include <Common/Base/Algorithm/UnionFind/hkUnionFind.h>


BlockWrapper::BlockWrapper(const hkArray<hkpRigidBody*>& bricks, const BrickRange& range, const hkVector4& brickExtents):
m_brickExtents(brickExtents),
m_brickRange(range),
m_bricks( bricks.getSize() )
{
	HK_ASSERT2(0x213fd8e7, bricks.getSize()!=0 , "Creating a block with no bricks!");
	m_bricks.copy( m_bricks.begin(), bricks.begin(), bricks.getSize() );
	computeDimensions();
}

BlockWrapper::~BlockWrapper()
{
}


hkBool BlockWrapper::extractDetachedBlocks(hkObjectArray< hkArray<BlockWrapper*> >& newBlocks, hkUnionFind& unionFinder, int numGroups, int parentsOffset) 
{
	hkBool res = false;

	// check if there are any detached blocks

	// groupRanges[i] will hold the range in the wall matrix that encloses all bricks of group i
	// note that NOT all the bricks in the range necessarily belong to the group
	hkArray<BrickRange> groupRanges(numGroups, BrickRange(10000, 10000, -1, -1));

	// update the range for each group (with wall space indices)
	for(int j=m_brickRange.minJ; j<=m_brickRange.maxJ; ++j) 
	{
		int baseInd = (j - m_brickRange.minJ) * m_blockHeight + parentsOffset; 
		for(int i=m_brickRange.minI; i<= m_brickRange.maxI; ++i)
		{
			int ind = baseInd + i - m_brickRange.minI; // to access the bricks the index must be local
			int groupId = unionFinder.m_parents[ind];
			if(groupRanges[groupId].maxI < i) groupRanges[groupId].maxI =  i;
			if(groupRanges[groupId].maxJ < j) groupRanges[groupId].maxJ =  j;
			if(groupRanges[groupId].minI > i) groupRanges[groupId].minI =  i;
			if(groupRanges[groupId].minJ > j) groupRanges[groupId].minJ =  j;
		}
	} // end for

	// now we must :
	// 1) find all the detached blocks
	// 2) for each block found divide it into rectangular sub blocks
	// the finder will recognize the ranges 
	BlockRangeFinderGroups detachedBlocksFinder(m_brickRange, 0, &unionFinder.m_parents);
	for(int group=0; group < numGroups; ++group)
	{
		if(groupRanges[group].maxI >= 0) // some of the groups may have not been initialized (if this block had no bricks of those groups)
		{
			// find the detached blocks
			detachedBlocksFinder.setGroup(group);
			res = findSubBlocks(groupRanges[group], newBlocks[group], detachedBlocksFinder, parentsOffset) || res;
			if(!res) // if there aren't any sub blocks this block must be added to the list
				newBlocks[group].pushBack(this);
		}
	}

	return res;
}

hkBool BlockWrapper::findSubBlocks(const BrickRange& range, hkArray<BlockWrapper*>& newBlocks, const BlockRangeFinder& finder, int parentsOffset) const
{
	HK_ASSERT2(0x43d1b005, (range.minI>=m_brickRange.minI) && (range.maxI<=m_brickRange.maxI) , "range out of block bounds");
	HK_ASSERT2(0x1f7c2fcb, (range.minJ>=m_brickRange.minJ) && (range.maxJ<=m_brickRange.maxJ) , "range out of block bounds");

	hkBool hasSubblocks = false;

	// ranges of wall bricks 
	hkArray<BrickRange*> startRanges(0); // these will start blocks
	hkArray<BrickRange*> endRanges(0); // these will end blocks
	hkArray<BrickRange*> blocksInfo; // start and end of each new block are stored in ranges

	// starting from the first brick (lower left) we decompose the wall in rectangular sub-blocks
	// insert a fake range before the beginning of the wall, the first real range will be compared with this one
	startRanges.pushBack(new BrickRange(range.minI, range.minJ-1, range.maxI, range.minJ-1));
	for(int j=range.minJ; j<=range.maxJ; ++j)
	{
		int baseInd=(j-m_brickRange.minJ)*m_blockHeight + range.minI - m_brickRange.minI + parentsOffset;
		// find ranges in current column
		if( finder.findRanges(baseInd, baseInd + (range.maxI - range.minI), endRanges, parentsOffset) )
		{
			mergeRanges(startRanges, endRanges, blocksInfo);
			// end ranges become start ranges
			startRanges.insertAt(startRanges.getSize(), &endRanges[0], endRanges.getSize());
			endRanges.clear();
		}
	}
	// close all ranges that were not closed by end-ranges by inserting a fake range after the end of the wall
	endRanges.pushBack(new BrickRange(range.minI, range.maxJ+1, range.maxI, range.maxJ+1));
	mergeRanges(startRanges, endRanges, blocksInfo);

	hasSubblocks = !(blocksInfo.isEmpty());
	// add new blocks
	if(hasSubblocks)
	{
		for(int i=0; i<blocksInfo.getSize(); ++i)
		{
			newBlocks.pushBack( buildSubBlock(*(blocksInfo[i])) );
			delete blocksInfo[i];
		}
	} 
	return hasSubblocks;
}

BlockWrapper* BlockWrapper::buildSubBlock(const BrickRange& brickInfo) const
{
	hkArray<hkpRigidBody*> bricks;
	bricks.reserve( (brickInfo.maxI-brickInfo.minI+1) * (brickInfo.maxJ-brickInfo.minJ+1) );

	// simply copy the bricks and create a new block
	for(int j=brickInfo.minJ - m_brickRange.minJ; j<=brickInfo.maxJ - m_brickRange.minJ; ++j)
	{
		int baseInd = m_blockHeight*j;
		for(int i=brickInfo.minI - m_brickRange.minI; i<= brickInfo.maxI - m_brickRange.minI; ++i)
		{
			int ind = baseInd+i;
			bricks.pushBack(m_bricks[ind]);
		}
	}

	return new BlockWrapper(bricks, brickInfo, m_brickExtents);
}


void BlockWrapper::mergeRanges(hkArray<BrickRange*>& startRanges, hkArray<BrickRange*>& endRanges, hkArray<BrickRange*>& blocksInfo) const
{
	// for each start range
	hkArray<BrickRange*> newStarts(0);
	int endsToSave=endRanges.getSize();
	int endsSaved=0;
	hkArray<BrickRange*> newEnds;
	hkArray<BrickRange*> ends2remove;
	newEnds.reserve(endsToSave);
	
	for( int i=0; i < startRanges.getSize(); ++i )
	{

		BrickRange* r1=startRanges[i];
		// compare with an end edge
		for(int j=0; j<endRanges.getSize() && startRanges[i]!=HK_NULL ; ++j)
		{
			BrickRange* r2=endRanges[j];
			
			// if the first range overlaps with the second 
			if(areOverlappingRanges(r1,r2))
			{
				if(endsSaved < endsToSave)
				{
					// the end range will be no more a valid end, but it must be saved to become a new start
					newEnds.pushBack(endRanges[j]);
					++endsSaved;
				} else
					ends2remove.pushBack(endRanges[j]);
				endRanges.removeAtAndCopy(j);

				// if the ranges overlap we may be able to create a block and/or split the start range into smaller, non overlapping, new ranges
				int dImax = r1->maxI - r2->maxI;
				int dImin = r1->minI - r2->minI;

				// a block can be created only between ranges in non-consecutive columns
				if(r2->minJ > r1->minJ+1)
				{
					// lower left corner of block
					BrickPos p1(r1->minI, r1->minJ+1);   
					if(dImin<0)
						p1.i -= dImin;
					// top right corner
					BrickPos p2(r2->maxI, r2->maxJ-1);
					if(dImax<0)
						p2.i += dImax;
					// make a block enclosed by the overlapping parts
					BrickRange* br = new BrickRange(p1.i ,p1.j, p2.i, p2.j);

					if(!isWholeBlock(*br))
						blocksInfo.pushBack(br);
				}
				

				// check if and how new ranges should substitute the current start range			
				if(dImin < 0)
				{
					BrickRange* nr = new BrickRange(r1->minI, r1->minJ, r2->minI-1, r1->minJ);
					//nr->maxI -= (dImin-1);
					startRanges.pushBack(nr);
				} else if(dImin >0)
				{	
					BrickRange* nr = new BrickRange(r2->minI, r2->minJ, r1->minI-1, r2->minJ);
					//nr->maxI += (dImin-1);
					endRanges.pushBack(nr);
				}
				if(dImax > 0)
				{
					BrickRange* nr = new BrickRange(r2->maxI+1, r1->maxJ, r1->maxI, r1->maxJ);
					//nr->maxI += (dImax-1);
					startRanges.pushBack(nr);
				} else if(dImax < 0)
				{	
					BrickRange* nr = new BrickRange(r1->maxI+1, r2->maxJ, r2->maxI, r2->maxJ);
					//nr->maxI -= (dImax-1);
					endRanges.pushBack(nr);
				}

				// invalidate the current start range 
				delete r1;
				startRanges[i] = HK_NULL;
			}// end of if(AreOverlapping(...) )
		}// end for(..j..)
		if(startRanges[i] != HK_NULL)
		{
			// the range doesn't overlap with any end, we'll keep it for the next MergeRanges call
			newStarts.pushBack(startRanges[i]);
		}
	}// end for(..i..)
	// we keep only the start ranges which didn't overlap
	startRanges.clear();
	if(!newStarts.isEmpty())
		startRanges.insertAt(0, &newStarts[0], newStarts.getSize());
	// and the old end ranges
	if(!newEnds.isEmpty())
		endRanges.insertAt(0, &newEnds[0], newEnds.getSize());
	for(int i=0; i<ends2remove.getSize(); ++i)
		delete ends2remove[i];
}

bool BlockWrapper::isInside(hkVector4& point) const
{
	const hkReal tolerance = 0.3f;

	if( ( m_blockMin(0) < point(0)+tolerance && point(0)-tolerance < m_blockMax(0) ) &&
		( m_blockMin(1) < point(1)+tolerance && point(1)-tolerance < m_blockMax(1) ) //&&
		/*( m_blockMin(2) < point(2)+tolerance && point(2)-tolerance < m_blockMax(2) )*/ )
	{
		point(0) = hkMath::clamp(point(0), m_blockMin(0)+0.01f , m_blockMax(0)-0.01f);
		point(1) = hkMath::clamp(point(1), m_blockMin(1)+0.01f , m_blockMax(1)-0.01f);
		point(2) = hkMath::clamp(point(2), m_blockMin(2)+0.01f , m_blockMax(2)-0.01f);
		return true;
	}	
	return false;
}

void BlockWrapper::computeDimensions()
{
	if(m_bricks.isEmpty())
	{
		m_blockHeight = m_blockWidth = 0;
	} else {
		m_blockHeight = m_brickRange.maxI - m_brickRange.minI + 1; 
		m_blockWidth  = m_brickRange.maxJ - m_brickRange.minJ + 1; 
		
		m_blockMin(0) = m_brickRange.minJ*m_brickExtents(0);
		m_blockMin(1) = m_brickRange.minI*m_brickExtents(1);
		m_blockMin(2) = -m_brickExtents(2)*0.5f;
		m_blockMax(0) = (m_brickRange.maxJ + 1)*m_brickExtents(0);
		m_blockMax(1) = (m_brickRange.maxI + 1)*m_brickExtents(1);
		m_blockMax(2) = m_brickExtents(2)*0.5f;

		HK_ASSERT2(0x50f28838, (m_blockHeight*m_blockWidth)==m_bricks.getSize() ,"Extents inconsistent with number of bricks");
	}
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
