/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>


#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallWrapper.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BrickFilter.h>
#include <Common/Base/Algorithm/Sort/hkSort.h>
#include <Common/Base/Algorithm/UnionFind/hkUnionFind.h>
#include <Common/Base/Math/Vector/hkVector4Util.h>
#include <Common/Internal/ConvexHull/hkPlaneEquationUtil.h>
#include <Common/Base/Types/Geometry/hkGeometry.h>
#include <Common/Internal/ConvexHull/hkGeometryUtility.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>


WallWrapper::WallWrapper(BlockWrapper* block, const hkVector4& brickExt):
m_fracturable(false),
m_wallID(0),
m_brickExtents(brickExt),
m_blocks(&block,1,1),
m_lastFetchedBlock( m_blocks[0] ),
m_centerOfMass( hkVector4::getZero() ),
m_isAttachedtoGround( false ),
m_theGround(HK_NULL)
{
	// check if the block should be split
	consolidateWall();
	HK_ASSERT2(0x4c1b9287, m_bricks.getSize()!=0 ,"this wall has no bricks!");
}

WallWrapper::WallWrapper(hkArray<BlockWrapper*>& blocks, const hkVector4& brickExt):
m_fracturable(false),
m_wallID(0),
m_brickExtents(brickExt),
m_centerOfMass( hkVector4::getZero() ),
m_isAttachedtoGround( false ),
m_theGround(HK_NULL)
{
	m_blocks.insertAt(0, &blocks[0], blocks.getSize());
	m_lastFetchedBlock = m_blocks[0];
	// check if the block should be split
	consolidateWall();
	HK_ASSERT2(0x4c1b9287, m_bricks.getSize()!=0 ,"this wall has no bricks!");
}


WallWrapper::WallWrapper(BlockWrapper* block, const hkArray<hkpConstraintInstance*>& constraints, const hkVector4& brickExt):
m_fracturable(false),
m_wallID(0),
m_brickExtents( brickExt ),
m_blocks( &block,1,1 ),
m_constraints( constraints.getSize() ),
m_lastFetchedBlock( m_blocks[0] ),
m_centerOfMass( hkVector4::getZero() ),
m_isAttachedtoGround( false ),
m_theGround(HK_NULL)
{
	// check if the block should be split
	consolidateWall();
	if(!m_constraints.isEmpty())
		m_constraints.copy(&m_constraints[0], &constraints[0], constraints.getSize());
	HK_ASSERT2(0x4c1b9287, m_bricks.getSize()!=0 ,"this wall has no bricks!");
}
	
WallWrapper::WallWrapper(const hkArray<hkpRigidBody*>& bricks, const hkArray<hkpConstraintInstance*>& constraints,  const hkVector4& brickExt):
m_fracturable(false),
m_wallID(0),
m_brickExtents(brickExt),
m_constraints(constraints.getSize()),
m_centerOfMass( hkVector4::getZero() ),
m_isAttachedtoGround( false ),
m_theGround(HK_NULL)
{
	// make a block from the bricks
	BrickFilter::extractPositionFromInfo(*(bricks[bricks.getSize()-1]), m_wallHeight, m_wallWidth);
	BlockWrapper* block = new BlockWrapper( bricks, BrickRange(0, 0, m_wallHeight, m_wallWidth) , m_brickExtents);
	m_blocks.pushBack(block);
	m_lastFetchedBlock = m_blocks[0];
	// and copy all data back
	consolidateWall();
	if(!m_constraints.isEmpty())
		m_constraints.copy(&m_constraints[0], &constraints[0], constraints.getSize());
	HK_ASSERT2(0x4c1b9287, m_bricks.getSize()!=0 ,"this wall has no bricks!");
}


WallWrapper::~WallWrapper()
{
	// destroy all blocks
	for(int i=0;i<m_blocks.getSize();i++)
		delete m_blocks[i];
}

void WallWrapper::releaseWorldData()
{
	for(int i=0; i<m_bricks.getSize(); ++i)
		m_bricks[i]->removeReference();
	for(int i=0; i<m_constraints.getSize(); ++i)
		m_constraints[i]->removeReference();
}

void WallWrapper::setID(int newID)
{
	m_wallID = newID;

	for(int k=0; k<m_bricks.getSize(); ++k)	
	{
		if( BrickFilter::getWallID(*(m_bricks[k])) != m_wallID )
		{
			hkUint32 filterInfo = m_bricks[k]->getCollisionFilterInfo();
			BrickFilter::setWallID( filterInfo, m_wallID );
			m_bricks[k]->setCollisionFilterInfo( filterInfo );
		}   
	}
}

hkBool WallWrapper::fractureWall( hkArray<WallWrapper*>& newWallWrappersOut, hkArray<hkpRigidBody*>& newWallBodiesOut, hkArray<hkpConstraintInstance*>& wgConstraintsOut, const hkpRigidBody& originalWallBody ) 
{
	// check if the wall was broken
	if( findDetachedWalls2() )
	{
		// create the rigid bodies
		buildBodies(m_newWalls, m_newWallsPositions, newWallBodiesOut, originalWallBody, wgConstraintsOut);

		// Set the new wall wrappers
		newWallWrappersOut.insertAt(newWallWrappersOut.getSize(), m_newWalls.begin(), m_newWalls.getSize());

		m_blocks.clear();
		m_bricks.clear();

		return true;
	}
	return false;
}

hkpConstraintInstance* WallWrapper::generateBodyForThisWrapper(hkpRigidBody** newWall, const BrickwallBuilderDescriptor& bwDescriptor ) const
{
	HK_ASSERT2(0x4c1b9287, m_blocks.getSize()==1 ,"the wall is not a single block...cannot generate the body");	

	const hkpShape* shape=HK_NULL;
	hkReal wallMass=0;
	// build the shape and compute the mass
	makeShape(this, &shape, wallMass);
	// build wall's body
	hkpRigidBodyCinfo info;
	info.m_position = bwDescriptor.m_transform.getTranslation();  //bwDescriptor.m_position;
	info.m_position(1) += (getWallHalfExtents())(1);
	info.m_rotation.set( bwDescriptor.m_transform.getRotation() );   //bwDescriptor.m_orientation;

	makeWall(shape, wallMass, getID(), newWall, info);
	hkpConstraintInstance* wgConstraint = HK_NULL;
	if( isAttachedToGround() )
	{
		wgConstraint = BrickWallBuilder::buildConstraintInstance( (*newWall), bwDescriptor.m_theGround );
	}

	shape->removeReference();

	return wgConstraint;
}

void WallWrapper::buildBodies( const hkArray<WallWrapper*>& newWallWrappers, const hkArray<BrickPos>& brokenPiecesPositions, hkArray<hkpRigidBody*>& newWallBodies, const hkpRigidBody& originalWallBody, hkArray<hkpConstraintInstance*>& wgConstraints ) const
{
	hkArray<const hkpShape*> shapes;
	hkArray<hkReal> wallMasses;

	// build the shapes for the walls and compute the masses
	makeShapes(newWallWrappers, shapes, wallMasses);

	// compute original body half extents
	hkAabb aabb;
	originalWallBody.getCollidable()->getShape()->getAabb( hkTransform::getIdentity() , 0, aabb);
	hkVector4 originalBodyHalfExtents;
	originalBodyHalfExtents.setSub4(aabb.m_max, aabb.m_min);
	originalBodyHalfExtents.mul4( 0.5f ); 

	hkpRigidBodyCinfo info;
	int newWallBodiesBaseInd = newWallBodies.getSize();
	newWallBodies.expandBy(shapes.getSize());

	// build the bodies from the collected shapes and masses
	for(int k=0; k<shapes.getSize(); ++k)
	{
		// compute the position of the center of the wall (considering the lower-left corner in the origin)
		hkVector4 localPos( brokenPiecesPositions[k].j *   getBrickExtentsX() , brokenPiecesPositions[k].i * getBrickExtentsY() , .0f); 
		localPos.add4( newWallWrappers[k]->getWallHalfExtents() );
		localPos.sub4( originalBodyHalfExtents );
		// apply rotation
		info.m_position.setRotatedDir( originalWallBody.getRotation(), localPos);
		// and translate it in the original wall position
		info.m_position.add4( originalWallBody.getPosition() );
		// set body rotation
		info.m_rotation = originalWallBody.getRotation();

		// build the wall body
		makeWall(shapes[k], wallMasses[k], newWallWrappers[k]->getID() ,&(newWallBodies[k + newWallBodiesBaseInd]), info);	
		if(isAttachedToGround() && newWallWrappers[k]->getMinI()==0 )
		{
			newWallWrappers[k]->attachToGround( getGround() );
			wgConstraints.pushBack( BrickWallBuilder::buildConstraintInstance(newWallBodies[k + newWallBodiesBaseInd], getGround()) );
		}
		shapes[k]->removeReference();		
	}
}


void WallWrapper::makeShape( const WallWrapper* wallWrapper, const hkpShape** finalShapeOut, hkReal& wallMassOut ) const
{
	int width, height;
	hkReal brickMass = m_bricks[0]->getMass();

	// list of sub-shapes for the wall
	hkArray<const hkpShape*> shapes; 
	hkVector4 boxHalfExt;
	boxHalfExt.setMul4(0.5f, m_brickExtents);

	if(wallWrapper->hasSubBlocks()) 
	{
		//hkVector4 wallHalfExt = 
		const hkpBoxShape* boxshape;
		int numBlocks = wallWrapper->getNumBlocks();
		// for each block
		for(int k=0; k<numBlocks; ++k)
		{
			const BlockWrapper* block = wallWrapper->getBlock( k );
			width = block->getWidth();
			height = block->getHeight();
			// compute this block position
			BrickPos bpos = wallWrapper->getBlockPos( k );
			hkVector4 wpos(m_brickExtents(0)*( static_cast<hkReal>(bpos.j) ), m_brickExtents(1)*( static_cast<hkReal>(bpos.i) ), 0);
			wpos.add4( hkVector4( m_brickExtents(0)*width*0.5f, m_brickExtents(1)*height*0.5f, m_brickExtents(2)*0.5f ) );
			wpos.sub4( wallWrapper->getWallHalfExtents() );
			// make a shape from each sub block
			boxshape = StiffWallBuilder::createBoxShape( width, height, boxHalfExt );
			shapes.pushBack(new hkpConvexTranslateShape(boxshape , wpos) );
			boxshape->removeReference();
		}
		// make a wall as a shape collection of the sub-block walls
		(*finalShapeOut) = new hkpListShape( shapes.begin(), shapes.getSize() );
		// this wall mass is proportional to the number of bricks => wall size
		wallMassOut = (brickMass*wallWrapper->getNumBricks());
		// release resources
		for(int k=0; k< shapes.getSize() ; ++k)
			shapes[k]->removeReference();
		shapes.clear();
	}
	else {
		// single block wall 
		width = wallWrapper->getWidth();
		height = wallWrapper->getHeight();
		// make just one wall
		(*finalShapeOut) = StiffWallBuilder::createBoxShape( width, height, boxHalfExt );
		wallMassOut = (brickMass*wallWrapper->getNumBricks());
	}
}

hkBool WallWrapper::findDetachedWalls(hkArray<WallWrapper*>& newWalls, hkArray<BrickPos>& newWallsPositions)
{
	HK_ASSERT2(0x4c1b9287, m_newWalls.getSize()==0 ,"this wall has been already broken!");	
	HK_ASSERT2(0x4c1b9287, m_newWallsPositions.getSize()==0 ,"this wall has been already broken!");	

	hkBool res = false;

	int numBricks = m_bricks.getSize();
	// the buffer used by the union-find algorithm
	hkLocalBuffer<int> structure(numBricks);
	hkUnionFind unionFinder(structure, numBricks);

	// a group identifies a set of bricks which are still constrained toghether
	findGroups(unionFinder);

	// groupsElNums[i] will hold the number of elements of group i
	hkArray<int> groupsElNums;

	// check if there are detached blocks i.e. there is more than one group
	if( !unionFinder.isOneGroup() ) 
	{
		res = true;
		// create groups
		unionFinder.assignGroups(groupsElNums);
		// one block array for each group
		hkObjectArray< hkArray<BlockWrapper*> > blockGroups( groupsElNums.getSize() );
		newWallsPositions.setSize( groupsElNums.getSize() );
		for(int i=0; i<groupsElNums.getSize(); ++i)
			newWallsPositions[i].set(m_wallHeight, m_wallWidth);

		int offset=0;
		for(int i=0; i < m_blocks.getSize(); ++i)
		{
			int numbricks = m_blocks[i]->getBricksNum();
			// ask the blocks to find and extract detached sub blocks divided by groups;
			// each group is a connected component and will therefore be a wall
			if(m_blocks[i]->extractDetachedBlocks(blockGroups, unionFinder, groupsElNums.getSize(), offset))
			{
				delete m_blocks[i];
				m_blocks[i] = HK_NULL;
			} // if DetachedBlocks
			offset += numbricks;
		} // for each block

		// make a wall from each group of detached blocks and add it to the list
		for(int k=0; k<blockGroups.getSize(); ++k)
		{
			if( !blockGroups[k].isEmpty() )	
			{
				// for each wall to be constructed, the ranges of the new blocks are relative to the min i,j position 
				// among the ranges of the same group (i.e. same new wall)
				for(int b=0; b<blockGroups[k].getSize(); ++b)
				{
					if( (blockGroups[k])[b]->getBrickRange().minI < newWallsPositions[k].i )
						newWallsPositions[k].i = (blockGroups[k])[b]->getBrickRange().minI;
					if( (blockGroups[k])[b]->getBrickRange().minJ < newWallsPositions[k].j )
						newWallsPositions[k].j = (blockGroups[k])[b]->getBrickRange().minJ;
				}

				// for each wall to be constructed, the ranges of the new blocks are relative to the min i,j position 
				// among the ranges of the same group (i.e. same new wall)
				for(int b=0; b<blockGroups[k].getSize(); ++b)
					(blockGroups[k])[b]->shiftRange( newWallsPositions[k] );
				WallWrapper* ww = new WallWrapper( blockGroups[k], m_brickExtents );
				//ww->SetPosition( newWallsPositions[k] );
				newWalls.pushBack( ww );
			}
		}

		blockGroups.clear();
	}

	if(res)
	{
		m_blocks.clear();
		m_bricks.clear();
		// the first in the new walls list will take its place
		newWalls[0]->setID( getID() );	
	}

	return res;
}


hkBool WallWrapper::findDetachedWalls2()
{
	HK_ASSERT2(0x4c1b9287, m_newWalls.getSize()==0 ,"this wall has been already broken!");	
	HK_ASSERT2(0x4c1b9287, m_newWallsPositions.getSize()==0 ,"this wall has been already broken!");	

	hkBool res = false;

	int numBricks = m_bricks.getSize();
	// the buffer used by the union-find algorithm
	hkLocalBuffer<int> structure(numBricks);
	hkUnionFind unionFinder(structure, numBricks);
	
	// a group identifies a set of bricks which are still constrained toghether
	findGroups(unionFinder);

	// groupsElNums[i] will hold the number of elements of group i
	hkArray<int> groupsElNums;

	// check if there are detached blocks i.e. there is more than one group
	if( !unionFinder.isOneGroup() ) 
	{
		res = true;
		// create groups
		unionFinder.assignGroups(groupsElNums);

		// one block array for each group
		hkObjectArray< hkArray<BlockWrapper*> > blockGroups( groupsElNums.getSize() );
		m_newWallsPositions.setSize( groupsElNums.getSize() );
		for(int i=0; i<groupsElNums.getSize(); ++i)
			m_newWallsPositions[i].set(m_wallHeight, m_wallWidth);

		int offset=0;
		for(int i=0; i < m_blocks.getSize(); ++i)
		{
			int numbricks = m_blocks[i]->getBricksNum();
			// ask the blocks to find and extract detached sub blocks divided by groups;
			// each group is a connected component and will therefore be a wall
			if(m_blocks[i]->extractDetachedBlocks(blockGroups, unionFinder, groupsElNums.getSize(), offset))
			{
				delete m_blocks[i];
				m_blocks[i] = HK_NULL;
			} // if DetachedBlocks
			offset += numbricks;
		} // for each block

		// make a wall from each group of detached blocks and add it to the list
		for(int k=0; k<blockGroups.getSize(); ++k)
		{
			if( !blockGroups[k].isEmpty() )	
			{
				// for each wall to be constructed, the ranges of the new blocks are relative to the min i,j position 
				// among the ranges of the same group (i.e. same new wall)
				for(int b=0; b<blockGroups[k].getSize(); ++b)
				{
					if( (blockGroups[k])[b]->getBrickRange().minI < m_newWallsPositions[k].i )
						m_newWallsPositions[k].i = (blockGroups[k])[b]->getBrickRange().minI;
					if( (blockGroups[k])[b]->getBrickRange().minJ < m_newWallsPositions[k].j )
						m_newWallsPositions[k].j = (blockGroups[k])[b]->getBrickRange().minJ;
				}
				
				// for each wall to be constructed, the ranges of the new blocks are relative to the min i,j position 
				// among the ranges of the same group (i.e. same new wall)
				for(int b=0; b<blockGroups[k].getSize(); ++b)
					(blockGroups[k])[b]->shiftRange( m_newWallsPositions[k] );
				WallWrapper* ww = new WallWrapper( blockGroups[k], m_brickExtents );
				//ww->SetPosition( newWallsPositions[k] );
				m_newWalls.pushBack( ww );
			}
		}

		blockGroups.clear();
	}
	
	return res;
}

void WallWrapper::findGroups(hkUnionFind& unionFinder) const
{
	debugEdges.clear();

	int ii,jj;
	int start=0;
	for(int blockInd=0; blockInd<m_blocks.getSize(); ++blockInd)
	{
		BlockWrapper* block = m_blocks[blockInd];
		int numBricks = block->getBricksNum();
		for(int brickInd=start; brickInd< start+numBricks; ++brickInd)
		{
			hkpRigidBody* brick = m_bricks[brickInd];
			if( BrickFilter::hasBottomConstraint( *brick ) )
			{
				if( (block->getHeight()>1) && (( (brickInd-start) % block->getHeight()) != 0)/*brickInd-1 >= start*/ ) 
				{
					// search in this block
					unionFinder.addEdge(brickInd, brickInd-1); // connect to the element just below

					debugEdges.pushBack( m_bricks[brickInd]->getPosition() );
					debugEdges.pushBack( m_bricks[brickInd-1]->getPosition() );
				}
				else
				{	// search in another block, preceding this one
					BrickFilter::extractPositionFromInfo(*brick, ii, jj);
					int ind = findBrickInd(m_blocks.getSize()/*blockInd*/, ii-1, jj, m_lastBottomBlock);
					HK_ASSERT2(0x27d78c22, ind >= 0, "incorrect index found");

					unionFinder.addEdge(brickInd, ind); // connect to the element below, but in an other block

					debugEdges.pushBack( m_bricks[brickInd]->getPosition() );
					debugEdges.pushBack( m_bricks[ind]->getPosition() );
				}
			}
			if( BrickFilter::hasLeftConstraint( *brick ) )
			{
				if( brickInd - start >= block->getHeight() ) 
				{
					// search in this block
					unionFinder.addEdge(brickInd, brickInd - block->getHeight()); // connect to the element to the left

					debugEdges.pushBack( m_bricks[brickInd]->getPosition() );
					debugEdges.pushBack( m_bricks[brickInd - block->getHeight()]->getPosition() );
				}
				else
				{	// search in another block, preceding this one
					BrickFilter::extractPositionFromInfo(*brick, ii, jj);  
					int ind = findBrickInd(m_blocks.getSize()/*blockInd*/, ii, jj-1, m_lastLeftBlock);
					HK_ASSERT2(0x27d78c22, ind >= 0, "incorrect index found");

					unionFinder.addEdge(brickInd, ind); // connect to the element to the left, but in an other block

					debugEdges.pushBack( m_bricks[brickInd]->getPosition() );
					debugEdges.pushBack( m_bricks[ind]->getPosition() );
				}
			}
		}
		start += numBricks;
	}
}

int WallWrapper::findBrickInd(int lastBlockInd, int i, int j, LastFindBlock& foundBlock) const
{

	i-= m_minIJ.i; 
	j-= m_minIJ.j; 

	if( (foundBlock.m_block!=HK_NULL) && (foundBlock.m_block->hasBrick( i, j )) )
	{
		return ( foundBlock.m_firstBrickInd + foundBlock.m_block->getHeight() * (j-foundBlock.m_block->getPositionJ()) + i-foundBlock.m_block->getPositionI() );
	}

	int ind = 0;

	for(int b=0; b<lastBlockInd; ++b)
	{
		foundBlock.m_block = m_blocks[b];
		foundBlock.m_firstBrickInd = ind;
		if( foundBlock.m_block->hasBrick( i, j ) )
			return ( ind + foundBlock.m_block->getHeight() * (j-foundBlock.m_block->getPositionJ()) + i-foundBlock.m_block->getPositionI() );
		else
			ind += m_blocks[b]->getBricksNum();
	}
	return -1;
}

hkpRigidBody& WallWrapper::getBrick(const BrickPos& pos) 
{
	int i = 0;
	hkpRigidBody* res = HK_NULL;

	// check if the last block we searched has the brick
	hkBool found = m_lastFetchedBlock->hasBrick(pos);
	if(found)
	{
		res = m_lastFetchedBlock->getBrick(pos);
	}
	while( !found  && (i < m_blocks.getSize()))
	{
		found = m_blocks[i]->hasBrick(pos);
		if(found) 
		{
			// we found the block, compute brick ind in local array
			res = m_blocks[i]->getBrick(pos);
			m_lastFetchedBlock = m_blocks[i];
		}
			
		++i; // go on
	}

	return (*res);
}


bool WallWrapper::isInsideGetBlock(hkVector4& point, BlockWrapper** block) const
{
	// check the last checked block
	hkBool res = m_lastFetchedBlock->isInside( point );;
	int i = 0;
	// check each block
	while( !res && (i < m_blocks.getSize()) )
	{
		res = m_blocks[i]->isInside(point);
		m_lastFetchedBlock = m_blocks[i];
		++i;
	}

	if(res)
		*block = m_lastFetchedBlock;
	
	return res;
}


hkBool WallWrapper::hasBrick(const BrickPos& brickPos) const
{	
	// check if it's in the bounding range
	if( !( 0 <= brickPos.i  && brickPos.i <= m_wallHeight-1 ) || 
		!( 0 <= brickPos.j  && brickPos.j <= m_wallWidth-1  ) )
		return false;

	hkBool found = m_lastFetchedBlock->hasBrick(brickPos);
	// check each block
	int i = 0;
	while( !found && (i < m_blocks.getSize()))
	{
		found = m_blocks[i]->hasBrick(brickPos);
		if(found)
			m_lastFetchedBlock = m_blocks[i];
		++i;
	}

	return found;
}

static HK_FORCE_INLINE hkBool _compareBrickRange( const BlockWrapper* l, const BlockWrapper* r)
{
	const BrickRange& lhs = l->getBrickRange();
	const BrickRange& rhs = r->getBrickRange();

	return 
	( ( lhs.minJ < rhs.minJ ) || 
		( ( lhs.minJ == rhs.minJ) && (lhs.minI < rhs.minI) ) );
}

void WallWrapper::consolidateWall()
{
	m_bricks.clear();

	m_wallHeight = -1;
	m_wallWidth = -1;

	// sort all the blocks left-to-right & bottom-to-top
	hkSort( &m_blocks[0], m_blocks.getSize(), _compareBrickRange ); 

	BrickPos wallMinCoords = m_blocks[0]->getPosition();

	for(int i=0; i<m_blocks.getSize(); ++i) 
	{
		BrickRange br( m_blocks[i]->getBrickRange() );
		// collect all the bricks from all the walls
		m_blocks[i]->getBricks(m_bricks);
		// and find the min and max brick coordinates
		if(wallMinCoords.i > br.minI)
			wallMinCoords.i = br.minI;
		if(wallMinCoords.j > br.minJ)
			wallMinCoords.j = br.minJ;
		if(m_wallHeight < br.maxI)
			m_wallHeight = br.maxI; 
		if(m_wallWidth < br.maxJ)
			m_wallWidth = br.maxJ; 
	}
	// and compute extents
	m_wallWidth  -= ( wallMinCoords.j - 1); // i.e. WW = WW - PosJ + 1;
	m_wallHeight -= ( wallMinCoords.i - 1); // i.e. WH = WH - PosI + 1;

	m_wallHalfExtents(0) = 0.5f * m_brickExtents(0) * m_wallWidth;
	m_wallHalfExtents(1) = 0.5f * m_brickExtents(1) * m_wallHeight;
	m_wallHalfExtents(2) = 0.5f * m_brickExtents(2);

	// save all positions
	storeAllBricksPositions();

	// and compute min i and j
	
	int i, j;
	m_minIJ.i = 100000;
	m_minIJ.j = 100000;
	
	for(int k=0; k<m_bricks.getSize(); ++k)	
	{
		BrickFilter::extractPositionFromInfo( *(m_bricks[k]), i, j);
		if(i<m_minIJ.i) m_minIJ.i=i;
		if(j<m_minIJ.j) m_minIJ.j=j;
		m_centerOfMass.add4( m_bricks[k]->getPosition() );
	}
	m_centerOfMass.mul4( 1.0f/m_bricks.getSize() );

	m_impulse.setZero4();
	m_contactPoint.setZero4();
}
	
void WallWrapper::resetBricksPositionOrientationAndVelocities()
{
	const hkQuaternion identityQuat = hkQuaternion::getIdentity();
	const hkVector4 zeroVect = hkVector4::getZero();

	for(int k=0; k<m_bricks.getSize(); ++k)
	{
		// restore original position and rotation of each brick 
		m_bricks[k]->setPositionAndRotation( m_bricksPositions[k], identityQuat );

		// set zero linear and angular velocity 
		m_bricks[k]->setLinearVelocity( zeroVect );
		m_bricks[k]->setAngularVelocity( zeroVect );
	}
}

void WallWrapper::resetBricksVelocities()
{
	const hkVector4 zeroVect = hkVector4::getZero();
	for(int k=0; k<m_bricks.getSize(); ++k)
	{
		// set both linear and angular to zero
		m_bricks[k]->setLinearVelocity( zeroVect );
		m_bricks[k]->setAngularVelocity( zeroVect );
	}
}

void WallWrapper::storeAllBricksPositions()
{
	m_bricksPositions.reserveExactly( m_bricks.getSize() );
	for(int i=0; i<m_bricks.getSize(); ++i)
		m_bricksPositions.pushBack( m_bricks[i]->getPosition() );
}



hkBool WallWrapper::makeShapeFromPlanes(hkArray<hkVector4>& planes, hkpShape** shapeOut)
{
	hkArray<hkVector4> vertices;
	// compute vertices for convex shape
	hkPlaneEquationUtil::getConvexVerticesFromPlaneEquations(planes, vertices);

	if(!vertices.isEmpty())
	{
		hkStridedVertices vertsIn;
		vertsIn.m_vertices = reinterpret_cast<hkReal*>(&vertices[0]);
		vertsIn.m_numVertices = vertices.getSize();
		vertsIn.m_striding = sizeof(hkVector4);

		hkGeometry* geometry = new hkGeometry();

		hkInplaceArrayAligned16<hkVector4,32> transformedPlanes;
		// create geometry
		hkGeometryUtility::createConvexGeometry(vertsIn, *geometry, transformedPlanes);

		// create shape
		(*shapeOut) = new hkpConvexVerticesShape(vertsIn, transformedPlanes, 0.0f);

		delete geometry;
		return true;
	}
	return false;
}


hkBool WallWrapper::fractureBrick( const hkVector4& impulse, hkVector4& point )
{
	BlockWrapper* foundBlock;
	hkBool res=false;
	if( isInsideGetBlock( point, &foundBlock ) )
	{
		// set brick planes
		hkInplaceArrayAligned16<hkVector4,32> planes;
		planes.setSize(8);
		planes[0].set(-1, 0, 0,-m_brickExtents(0)*0.5f);
		planes[1].set( 1, 0, 0,-m_brickExtents(0)*0.5f);
		planes[2].set( 0,-1, 0,-m_brickExtents(1)*0.5f);
		planes[3].set( 0, 1, 0,-m_brickExtents(1)*0.5f);
		planes[4].set( 0, 0,-1,-m_brickExtents(2)*0.5f);
		planes[5].set( 0, 0, 1,-m_brickExtents(2)*0.5f);

		hkInplaceArrayAligned16<hkVector4, 2> fracturePlanes;
		{
			fracturePlanes.setSize(2);
			hkVector4 plane1;
			// normal to the first pale ( the plane is identified by the impulse direction )
			hkVector4Util::calculatePerpendicularVector(impulse, plane1);
			plane1.fastNormalize3();

			// find the distance of the plane (passing through the contact point) from the center of the shape aabb
			hkAabb aabb;
			m_bricks[0]->getCollidable()->getShape()->getAabb(hkTransform::getIdentity(), 0.0f, aabb);
			hkVector4 cm;
			cm.setSub4(aabb.m_max, aabb.m_min);
			cm.mul4(0.5f);
			cm(2)= 0.0f;
			cm.sub4(point);
			
			plane1(3) = cm.dot3(plane1);
			hkReal side = 1.0f;
			if(plane1(3) > 0)
			{
				side = -1.0f;
				plane1.mul4(side);
			}
			// set the first plane
			fracturePlanes[0] = plane1;

			// apply deviation
			hkVector4 plane2;
			hkQuaternion deviation;
			hkVector4 normal;
			normal.setCross(plane1, impulse);
			normal.fastNormalize3();
			deviation.setAxisAngle(normal, HK_REAL_PI/6.0f);
			plane2.setRotatedDir(deviation, plane1);
			plane2(3) = plane1(3) + side*0.1f;
			// add second plane
			fracturePlanes[1] = plane2;
		}

		planes[6] = fracturePlanes[0];
		planes[7] = fracturePlanes[1];
		
		// create the convex piece
		hkpShape* shape = HK_NULL;
		res = makeShapeFromPlanes(planes, &shape);
		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeShapeVolumeMassProperties(shape, m_bricks[0]->getMass(), massProperties);
		{
			// create rigid body
			hkpRigidBodyCinfo info; 
			info.m_shape = shape;
			info.m_mass = massProperties.m_mass;
			info.m_centerOfMass = massProperties.m_centerOfMass;
			info.m_inertiaTensor = massProperties.m_inertiaTensor;
			m_fracturedBodies.pushBack( new hkpRigidBody(info) );
			shape->removeReference();
		}

		// now the non-convex part
		// flip fracture plane 1
		planes[6].mul4(-1.0f);
		// and make shape 1
		res = makeShapeFromPlanes(planes, &shape);
		hkpInertiaTensorComputer::computeShapeVolumeMassProperties(shape, m_bricks[0]->getMass(), massProperties);
		{
			// create rigid body
			hkpRigidBodyCinfo info; 
			info.m_shape = shape;
			info.m_mass = massProperties.m_mass;
			info.m_centerOfMass = massProperties.m_centerOfMass;
			info.m_inertiaTensor = massProperties.m_inertiaTensor;
			m_fracturedBodies.pushBack( new hkpRigidBody(info) );
			shape->removeReference();
		}

		// flip and reinsert fracture plane 2 to take plane 1 place
		planes.popBack(1);
		planes[6].setMul4(-1.0f, fracturePlanes[1]);
		// and make shape 2
		res = makeShapeFromPlanes(planes, &shape);
		hkpInertiaTensorComputer::computeShapeVolumeMassProperties(shape, m_bricks[0]->getMass(), massProperties);
		{
			// create rigid body
			hkpRigidBodyCinfo info; 
			info.m_shape = shape;
			info.m_mass = massProperties.m_mass;
			info.m_centerOfMass = massProperties.m_centerOfMass;
			info.m_inertiaTensor = massProperties.m_inertiaTensor;
			m_fracturedBodies.pushBack( new hkpRigidBody(info) );
			shape->removeReference();
		}

		// disable fracture for successive impulses in this step
		toggleFracturableState(false);
		// save visual debug info
		m_fracturePlanes.insertAt(0, &(planes[0]), planes.getSize()-2);
		m_fracturePlanes.pushBack(fracturePlanes[0]);
		m_fracturePlanes.pushBack(fracturePlanes[1]);
		m_impulse = impulse;
		m_contactPoint = point;
	}
	return res;
}



hkBool WallWrapper::fractureBrick2( const hkVector4& impulse, hkVector4& point )
{
	hkBool res=false;
	// set brick planes
	hkInplaceArrayAligned16<hkVector4,32> planes;
	planes.setSize(7);
	planes[0].set(-1, 0, 0,-m_brickExtents(0)*0.5f);
	planes[1].set( 1, 0, 0,-m_brickExtents(0)*0.5f);
	planes[2].set( 0,-1, 0,-m_brickExtents(1)*0.5f);
	planes[3].set( 0, 1, 0,-m_brickExtents(1)*0.5f);
	planes[4].set( 0, 0,-1,-m_brickExtents(2)*0.5f);
	planes[5].set( 0, 0, 1,-m_brickExtents(2)*0.5f);

	hkInplaceArrayAligned16<hkVector4, 2> fracturePlanes;
	// create 2 planes facing opposite directions
	{
		fracturePlanes.setSize(2);
		hkVector4 plane;
		// normal to the first pale ( the plane is identified by the impulse direction )
		hkVector4Util::calculatePerpendicularVector(impulse, plane);
		plane.fastNormalize3();

		// find the distance of the plane (passing through the contact point) from the center of the shape
		hkAabb aabb;
		m_bricks[0]->getCollidable()->getShape()->getAabb(hkTransform::getIdentity(), 0.0f, aabb);
		hkVector4 cm;
		cm.setSub4(aabb.m_max, aabb.m_min);
		cm.mul4(0.5f);
		cm(2)= 0.0f;
		cm.sub4(point);
		//cm.sub4(impulse);
		plane(3) = cm.dot3(plane);
		if(plane(3) > 0)
			plane.mul4(-1.0f);
		// set the first plane
		fracturePlanes[0] = plane;
		// set the second as reversed
		fracturePlanes[1].setMul4(-1.0f, plane);  
	}

	for(int i=0; i<fracturePlanes.getSize(); ++i)
	{
		planes[6] = fracturePlanes[i];
		// compute vertices for convex shape
		hkArray<hkVector4> vertices;
		hkPlaneEquationUtil::getConvexVerticesFromPlaneEquations(planes, vertices);

		if(!vertices.isEmpty())
		{
			hkStridedVertices vertsIn;
			vertsIn.m_vertices = reinterpret_cast<hkReal*>(&vertices[0]);
			vertsIn.m_numVertices = vertices.getSize();
			vertsIn.m_striding = sizeof(hkVector4);

			hkGeometry* geometry = new hkGeometry();

			hkInplaceArrayAligned16<hkVector4,32> transformedPlanes;
			// create geometry
			hkGeometryUtility::createConvexGeometry(vertsIn, *geometry, transformedPlanes);

			// compute mass properties 
			hkReal mass = m_bricks[0]->getMass();
			hkpMassProperties massProperties;
			hkpInertiaTensorComputer::computeGeometryVolumeMassPropertiesChecked(geometry, mass, massProperties);
			{
				// create shape
				hkpConvexVerticesShape* cvs = new hkpConvexVerticesShape(vertsIn, transformedPlanes, 0.0f);
				// create rigid body
				hkpRigidBodyCinfo info; 
				info.m_shape = cvs;
				info.m_mass = massProperties.m_mass;
				info.m_centerOfMass = massProperties.m_centerOfMass;
				info.m_inertiaTensor = massProperties.m_inertiaTensor;
				m_fracturedBodies.pushBack( new hkpRigidBody(info) );
				cvs->removeReference();
			}
			delete geometry;
			res = true;
		}
	}

	// disable fracture
	toggleFracturableState(false);
	m_fracturePlanes.insertAt(0, &(planes[0]), planes.getSize()-1);
	m_fracturePlanes.pushBack(fracturePlanes[0]);
	m_fracturePlanes.pushBack(fracturePlanes[1]);
	m_impulse = impulse;
	m_contactPoint = point;
	return res;
}

hkBool WallWrapper::applyPointImpulseToBrick(const hkVector4& impulse, hkVector4& point)
{
	// THE FOLLOWING CODE MUST BE COMPLETED AND MOVED! TODO: here we'll store the impulses, later ( in FractureWall ) we'll fracture the body
	BlockWrapper* foundBlock;
	if( isFracturable() && isInsideGetBlock( point, &foundBlock ) )
	{
		return fractureBrick( impulse, point );
		// return fractureBrick2( impulse, point );
	} /// end TMP fracture procedure ----------------------------------------------
	else 
		if( isInsideGetBlock( point, &foundBlock ) )
	{
		// find the brick corresponding to the point
		BrickPos brickPos( static_cast<int>( point(1) / m_brickExtents(1)), static_cast<int>( point(0) / m_brickExtents(0) ) );

		// translate to the correct position 
		point(0) += getMinJ()* m_brickExtents(0);
		point(1) += getMinI()* m_brickExtents(1);

		foundBlock->getBrick(brickPos)->applyPointImpulse( impulse, point ); 
		
		return true;
	}

	return false;
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
