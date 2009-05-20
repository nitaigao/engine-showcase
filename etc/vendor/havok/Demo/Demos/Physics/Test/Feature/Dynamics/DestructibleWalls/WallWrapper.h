/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef WALL_WRAPPER
#define WALL_WRAPPER

#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BlockWrapper.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallBuilder.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BrickFilter.h>


class WallWrapper {

public :
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, WallWrapper);
	
	WallWrapper(BlockWrapper* block,  const hkVector4& brickExt);
	WallWrapper(BlockWrapper* block, const hkArray<hkpConstraintInstance*>& constraints,  const hkVector4& brickExt);
	WallWrapper(hkArray<BlockWrapper*>& blocks,  const hkVector4& brickExt);
	WallWrapper(const hkArray<hkpRigidBody*>& bricks, const hkArray<hkpConstraintInstance*>& constraints,  const hkVector4& brickExt);
	~WallWrapper();

	// Sets the wall ID
	void setID(int newID);
	// returns the wall ID
	inline int getID() const;

	// Provide access to the bricks
	inline hkpRigidBody * const * getBricks() const;
	
	// provides const access to a brick
	inline const hkpRigidBody& getBrick(int brickInd) const;
	
	// provides non const access to a brick
	//hkpRigidBody& GetBrick(int brickInd);
	hkpRigidBody& getBrick(const BrickPos& pos);
	// Returns the total amount of bricks 
	inline int getNumBricks() const;

	// returns true if the position is inside the wall. If the position is outside but within a tolerance the point is considered 
	// inside and it's clamped to the wall boundary
	inline bool isInside(hkVector4& point) const;

	// true if the brick with the specified position is in this wall, puts in brickInd the index of the brick
	// in this wall wrapper local bricks array
	hkBool hasBrick(const BrickPos& brickPos) const;

	// Access to constraints
	inline hkArray<hkpConstraintInstance*> const * getConstraints() const;

	// Returns true if the wall is composed multiple blocks
	inline hkBool hasSubBlocks() const;

	// Extents, in number of bricks, of the wall
	inline int getHeight() const;
	inline int getWidth() const;

	// Extents of a brick
	inline void getBrickExtents(hkReal& extentX, hkReal& extentY) const;
	inline const hkReal& getBrickExtentsX() const;
	inline const hkReal& getBrickExtentsY() const;
	// returns half extents of the wall
	inline const hkVector4& getWallHalfExtents() const;

	// returns the number of blocks the wall is composed of
	inline int getNumBlocks() const;
	// Returns a pointer to the array of sub-blocks
	inline const BlockWrapper* getBlock(int i) const;

	// returns the position (i,j) of the blockInd-th block in the world local grid
	inline BrickPos getBlockPos(int blockInd) const;

	// fracture the wall and returns the new bodies and constraints
	hkBool fractureWall( hkArray<WallWrapper*>& newWallWrappersOut, hkArray<hkpRigidBody*>& newWallBodiesOut, hkArray<hkpConstraintInstance*>& wgConstraintsOut, const hkpRigidBody& originalWallBody );

	// releases the references to the bricks and constraints, should be called after the 
	// wall elements have been added to the world of interest
	void releaseWorldData();

	// stores the wallID as a wall to be checked
	void notifyBrokenConstraint(int wallID);

	// sets the position and orientation of all bricks as if the wall was with the low-left corner in the origin
	void resetBricksPositionOrientationAndVelocities();
	// sets bricks linear and angular velocities to zero
	void resetBricksVelocities();

	inline void getAllBricksPositions(hkArray<hkVector4>& positions) const;

	// Access to min i and j values among all bricks   
	inline const BrickPos& getMinIJ() const;
	inline void getMinIJ(BrickPos& minIJ) const;
	inline int getMinI() const;
	inline int getMinJ() const;

	inline const hkVector4& getCenterOfMass() const;

	// returns true if the passed id is a valid one
	inline static hkBool isValidID(int id);

	// returns true if the wall is made of just one brick
	inline hkBool isOneBrick() const;

	inline hkBool isAttachedToGround() const;
	inline void attachToGround(hkpRigidBody* ground);
	inline hkpRigidBody* getGround() const;

	// applies the impulse to the brick containing the point (if any). 
	// NOTE: because a tolerance is used to consider the point inside the brick, the point is modified to be make it actually inside 
	hkBool applyPointImpulseToBrick(const hkVector4& impulse, hkVector4& point);

	inline void toggleFracturableState( hkBool fracturable );
	inline hkBool isFracturable() const;

	hkBool findDetachedWalls(hkArray<WallWrapper*>& newWalls, hkArray<BrickPos>& newWallsPositions);

	// Generates the single body for this wrapper. 
	// NB should be called only immediately after the Wrapper creation. Anyway only is the wrapped wall is 1 convex block.
	// For broken pieces generation use BuildBodies
	hkpConstraintInstance* generateBodyForThisWrapper(hkpRigidBody** newWall, const BrickwallBuilderDescriptor& wbDescriptor ) const;

	mutable hkArray<hkVector4> debugEdges;
	hkArray<hkVector4> m_fracturePlanes;
	hkVector4 m_contactPoint;
	hkVector4 m_impulse;
	hkBool m_fracturable;
	hkArray<hkpRigidBody*> m_fracturedBodies;

private:
	struct LastFindBlock
	{
		BlockWrapper* m_block;
		int m_firstBrickInd;
		LastFindBlock(BlockWrapper* block, int firstBrickInd)
		{
			m_block = block;
			m_firstBrickInd = firstBrickInd;
		}
		LastFindBlock()
		{
			m_block = HK_NULL;
			m_firstBrickInd = -1;
		}
	};

	hkBool fractureBrick( const hkVector4& impulse, hkVector4& point );
	hkBool fractureBrick2( const hkVector4& impulse, hkVector4& point );
	hkBool makeShapeFromPlanes(hkArray<hkVector4>& planes, hkpShape** shapeOut);

	// extracts and return all detached components of the wall
	hkBool findDetachedWalls2();
	// Builds the shapes corresponding to the wall wrappers
	inline void makeShapes(const hkArray<WallWrapper*>& newWallsWrappers, hkArray<const hkpShape*>& shapesOut, hkArray<hkReal>& wallMassesOut) const;
	// Builds the shape corresponding to the wall wrapper
	void makeShape( const WallWrapper* wallWrapper, const hkpShape** finalShapeOut, hkReal& wallMassOut ) const;
	// Builds the rigid bodies for each wall represented by the wall wrappers in the list
	void buildBodies(const hkArray<WallWrapper*>& newWallWrappers, const hkArray<BrickPos>& brokenPiecesPositions, hkArray<hkpRigidBody*>& newWallBodies, const hkpRigidBody& originalWallBody, hkArray<hkpConstraintInstance*>& wgConstraints) const;
	// Builds the rigid body wall from the shape and mass
	inline void makeWall(const hkpShape* const shape, const hkReal& wallMass, int wallID ,hkpRigidBody** newWall, hkpRigidBodyCinfo& info) const;
	// find groups inside the wall, used by the fracture procedure
	void findGroups(hkUnionFind& unionFinder) const;
	// retrieves the index in the bricks array of the brick (i,j)
	int findBrickInd(int lastBlockInd, int i, int j, LastFindBlock& foundBlock) const;
	// stores the original positions of all the bricks
	void storeAllBricksPositions();
	// As -IsInside-, but returns the block which contains the point, the point is modified to lay on the brick surface
	bool isInsideGetBlock(hkVector4& point, BlockWrapper** block) const;
	// To be called after all modifications are done.
	// Copies all blocks bricks in one place and updates dimensions;
	void consolidateWall();

private:
	// ID of the wall, must be unique inside a simulation
	int m_wallID;
	// wall extents as number of bricks
	int m_wallWidth;
	int m_wallHeight;
	// brick extents
	const hkVector4 m_brickExtents;
	hkVector4 m_wallHalfExtents;
	// the blocks of the wall
	hkArray<BlockWrapper*> m_blocks;
	// the bricks that compose the wall, it's the union of the bricks of all the blocks
	hkArray<hkpRigidBody*> m_bricks;
	// the constraints of the wall at creation time
	hkArray<hkpConstraintInstance*> m_constraints;
	// array of the positions of all bricks at construction time
	hkArray<hkVector4> m_bricksPositions;
	// min i and j values among all the bricks indexes
	BrickPos m_minIJ;
	// the last block we inspected for generic data, we keep it for locality purposes
	mutable BlockWrapper* m_lastFetchedBlock;
	// the last blocks we inspected during the FindGroup procedure, respectively for the left and bottom brick 
	mutable LastFindBlock m_lastLeftBlock;
	mutable LastFindBlock m_lastBottomBlock;
	// wall center of mass
	hkVector4 m_centerOfMass;
	// true if the wrapped wall should be attached to ground
	hkBool m_isAttachedtoGround;
	// the ground
	hkpRigidBody* m_theGround;
	// if the wall was broken the new walls are stored until this wall is removed
	hkArray<WallWrapper*> m_newWalls;
	// if the wall was broken the positions of new walls are stored until this wall is removed
	hkArray<BrickPos> m_newWallsPositions;
};

#include<Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallWrapper.inl>

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
