/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef BLOCK_RANGE_TYPE
#define BLOCK_RANGE_TYPE

#include <Common/Base/Container/LocalArray/hkLocalBuffer.h>
class hkpRigidBody;

class hkpRigidBody;

// simple struct to keep position (i,j) of a brick in a wall or block
struct BrickPos
{
	int i;
	int j;

	BrickPos(int posi, int posj)
	{
		i=posi;
		j=posj;
	}

	BrickPos(const BrickPos& pos)
	{
		i=pos.i;
		j=pos.j;
	}

	BrickPos()
	{
		i=j=-1;
	}

	void add(const BrickPos& rhs)
	{
		i += rhs.i;
		j += rhs.j;
	}
	void sub(const BrickPos& rhs)
	{
		i -= rhs.i;
		j -= rhs.j;
	}
	void set(int posi, int posj)
	{
		i=posi;
		j=posj;
	}

	hkBool operator== (const BrickPos& rhs)
	{
		return ((i == rhs.i) && (j == rhs.j));
	}

	hkBool operator!= (const BrickPos& rhs) const
	{
		return !((i == rhs.i) && (j == rhs.j));
	}
};



// helper struct
template<typename T>
struct Range
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, Range);

	T minI;
	T minJ;
	T maxI;
	T maxJ;

	Range(const Range<T>& rhs)
	{
		minI=rhs.minI;
		minJ=rhs.minJ;
		maxI=rhs.maxI;
		maxJ=rhs.maxJ;
	}

	Range(const T& min_i, const T& min_j, const T& max_i, const T& max_j)
	{
		minI=min_i;
		minJ=min_j;
		maxI=max_i;
		maxJ=max_j;
	}

	Range()
	{
		minI=10000;
		minJ=10000;
		maxI=-1;
		maxJ=-1;
	}

	const Range<T>& operator= (const Range<T>& rhs)
	{
		minI=rhs.minI;
		minJ=rhs.minJ;
		maxI=rhs.maxI;
		maxJ=rhs.maxJ;
		return (*this);
	}

	void sub(const BrickPos& diff)
	{
		minI -= diff.i;
		minJ -= diff.j;
		maxI -= diff.i;
		maxJ -= diff.j;
	}
};

template<typename T>
hkBool operator==(const Range<T>& lhs, const Range<T>& rhs)
{
	return ( lhs.minI == rhs.minI &&
			 lhs.minJ == rhs.minJ &&
			 lhs.maxI == rhs.maxI &&
			 lhs.maxJ == rhs.maxJ );
}

template<typename T> 
void findMinMaxInRanges( const hkArray< Range<T> >& ranges, T& minI, T& minJ, T& maxI, T& maxJ)
{
	for(int i=0; i<ranges.getSize(); ++i) 
	{
		if(minI > ranges[i].minI)
			minI = ranges[i].minI;
		if(minJ > ranges[i].minJ)
			minJ = ranges[i].minJ;
		if(maxI < ranges[i].maxI)
			maxI = ranges[i].maxI; 
		if(maxJ < ranges[i].maxJ)
			maxJ = ranges[i].maxJ; 
	}
}

template<typename T> 
void findMinInRanges( const hkArray< Range<T>* >& ranges, T& minI, T& minJ)
{
	for(int i=0; i<ranges.getSize(); ++i) 
	{
		if(minI > ranges[i]->minI)
			minI = ranges[i]->minI;
		if(minJ > ranges[i]->minJ)
			minJ = ranges[i]->minJ;
	}
}

template<typename T> 
void findMaxInRanges( const hkArray< Range<T> >& ranges, T& maxI, T& maxJ)
{
	for(int i=0; i<ranges.getSize(); ++i) 
	{
		if(maxI < ranges[i].maxI)
			maxI = ranges[i].maxI; 
		if(maxJ < ranges[i].maxJ)
			maxJ = ranges[i].maxJ; 
	}
}

typedef Range<int> BrickRange;

class rangeless
{
public:

	hkBool operator() ( const BrickRange& lhs, const BrickRange& rhs)
	{
		return ( ( lhs.minJ < rhs.minJ ) || 
				 ( ( lhs.minJ == rhs.minJ) && (lhs.minI < rhs.minI) ) );
	}
	hkBool operator() ( const BrickRange* lhs, const BrickRange* rhs)
	{
		return operator()(*lhs, *rhs);
	}
};

class BlockRangeFinder : public hkReferencedObject
{

public:

	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	BlockRangeFinder(const BrickRange& blockRange);

	hkBool findRanges(int baseInd, int lastInd, hkArray<BrickRange*>& endRanges, int baseOffset) const;
private:
	virtual hkBool isRangeElement(int brickInd) const = 0;
protected:
	BrickRange m_blockRange;
	int m_blockHeight;
	int m_blockWidth;
};

class BlockRangeFinderHoles : public BlockRangeFinder
{

public:
	BlockRangeFinderHoles(const BrickRange& blockRange, hkArray<hkpRigidBody*>* bricks);

private:
	inline virtual hkBool isRangeElement(int brickInd) const;

private:
	hkArray<hkpRigidBody*>* m_bricks;
};

class BlockRangeFinderGroups : public BlockRangeFinder
{

public:
	BlockRangeFinderGroups(const BrickRange& blockRange, int searchedGroup, hkFixedArray<int>* groups);
	inline void setGroup(int searchedGroup);

private:
	// returns true if the element at index brickInd doesn't belong to the searched group
	inline virtual hkBool isRangeElement(int brickInd) const;

private:
	hkFixedArray<int>* m_groups;
	int m_searchedGroup;
};

#include<Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BlockRangeFinder.inl>

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
