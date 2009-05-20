/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef WALL_SIMULATION
#define WALL_SIMULATION

#include <Common/Base/Container/PointerMap/hkPointerMap.h>
#include <Physics/Dynamics/Constraint/Contact/hkpSimpleContactConstraintData.h>
#include <Physics/ConstraintSolver/SimpleConstraints/hkpSimpleConstraintUtil.h>

#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallWrapper.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallBuilder.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/BrickFilter.h>

struct BrickPos;

struct BodyVelocities
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, BodyVelocities);

	hkVector4 linear;
	hkVector4 angular;

	BodyVelocities()
		:linear( hkVector4::getZero() ),
		angular( hkVector4::getZero() )
	{
	}
};

struct CollisionBodiesVelocities
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, CollisionBodiesVelocities);
	
	hkVector4 bodyALinear;
	hkVector4 bodyBLinear;
	hkVector4 bodyAAngular;
	hkVector4 bodyBAngular;

	CollisionBodiesVelocities( const hkVector4& linearA, const hkVector4& linearB, const hkVector4& angularA, const hkVector4& angularB)
		:bodyALinear( linearA ),
		bodyBLinear( linearB ),
		bodyAAngular( angularA ),
		bodyBAngular( angularB )
	{
	}
};

struct WallCollisionEventData
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, WallCollisionEventData);

	hkVector4 impulseApplied;
	hkVector4 contactPointPosition;

	WallCollisionEventData(const hkVector4& impulse, const hkVector4& position)
		:impulseApplied(impulse),
		contactPointPosition(position)
	{
	}
};



struct WallCollisions
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, WallCollisions);
	
	int wallID;
	hkArray<WallCollisionEventData*> eventsData;
	hkVector4 preCollisionLinearVelocity;
	hkVector4 preCollisionAngularVelocity;

	WallCollisions(int wallid, WallCollisionEventData* eventdata, const hkVector4& preCollisionLinear, const hkVector4& preCollisionAngular)
		:wallID( wallid ),
		eventsData( 1, eventdata ),
		preCollisionLinearVelocity( preCollisionLinear ),
		preCollisionAngularVelocity( preCollisionAngular )
	{
	}

	void clear()
	{
		for(int i=0; i<eventsData.getSize(); ++i)
			delete eventsData[i];
	}
};


class BrickConstraintListener;
class WallFractureCollisionListener;

// Class used to simulate walls. The actual simulation is run in the world provided.
// You can add and remove elements from the simulation
class WallSimulation 
{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, WallSimulation);
	
	WallSimulation(hkpWorld* mainWorld, hkpWorld* parallelWorld, hkReal timestep);
	~WallSimulation();

	void autoSetup();
	//inline void SetCollisionListener(hkpCollisionListener* collisionListener);

	// Construction methods
	// reserve n wall IDs
	inline int reserveWallID();
	// reserve n wall IDs
	void reserveWallIDs(int numIds, hkArray<int>& IDs);
	
	// creates and adds a wall to the simulation according to the info in the descriptor returning a pointer to the newly created wall wrapper.
	// note that the returned wrapper should be used only just after creation as the simulation may delete it during wall fracture
	WallWrapper* addWallToSimulation( const BrickwallBuilderDescriptor& bwDescriptor );
	// adds a non-wall object to the world
	inline void addObjectToParallel( hkpRigidBody* obj );

	// returns the wall wrapper, from the parallel world, with the specified ID. 
	inline WallWrapper* getWallFromParallel(int wallID);
	// returns the wall rigid body from the main world, with the specified ID. 
	inline hkpRigidBody* getWallFromMain(int wallID);
	// puts all the bodies currently in the simulation in the array 
	inline void getAllWallsBodies(hkArray<hkpRigidBody*>& bodies);

	// returns true if the wall with is in the simulation and has the specified ID
	inline hkBool isWallInSimulation(int wallID, const hkpRigidBody* wall) const;
	
	// Update walls status in both the main and parallel simulation, checking for broken walls
	void updateWalls();
	// to be called when a constraint in wallID is broken
	inline void notifyBrokenConstraintInWall(int wallID);
	// to be called when a collision event involving walls of the simulation occurs
	void notifyCollisionEvent(int wallID1, int wallID2, const hkVector4& appliedImpulse, const hkVector4& contactPoint, const CollisionBodiesVelocities& preCollisionVelocities);

	// Utility function to transform point from world space to a wall object space
	inline void transformPointFromWorldToWallLocal(const hkpRigidBody& wall, const hkVector4& worldPoint, hkVector4& localPoint);

	// retrieves the positions of all the bricks in the simulation
	void getAllBricksPositions(hkArray<hkVector4>& positions) const;

	void getAllDebugEdges(hkArray<hkVector4>& edges) const;

	void getAllDebugfracturePlanes(hkArray<hkVector4>& planes) const;
	void getAllDebugImpulsesAndContactPoints(hkArray<hkVector4>& impulsesArray, hkArray<hkVector4>& pointsArray) const;
	// NB: not used by now, must be more general....
	inline void attachWallTo(int wallID, hkpRigidBody* otherBody);
	
	inline WallFractureCollisionListener* getCollisionListener();

	hkArray<hkVector4> debugImpulses;
	mutable hkArray<hkVector4> debugEdges;


private:

	WallWrapper* createWallWrapper( const BrickwallBuilderDescriptor& wbDescriptor );
	void posOnGround(hkpWorld* world, const hkVector4& pos, hkVector4& newPosOut) const ;
	// Adds the wall wrapper to the list of wall wrappers	
	void storeWrapperAndBody(WallWrapper* wall, hkpRigidBody* mwWall);
	// adds wall bricks and constraints to the parallel world
	void addWallBricksToParallelWorld(WallWrapper* wall);
	// adds the rigid body to the main world
	inline void addWallBodyToMainWorld(hkpRigidBody* wallBody);
	// removes the bricks from the parallel world	
	void removeWrapperAndBricks(int wallID);
	void removeWrapper(int wallID);

	// replace the wall identified by wallID with newWall
	void replaceWallWrapperAndBody(int wallID, WallWrapper* newWallWrapper, hkpRigidBody* newWallBody);

	// runs the simulation
	inline void simulate();
	// Apply impulses from collision events data
	void applyImpulses();
	
	// If the wall identified by wallID broke in the last simulation collects the new walls in
	// brokenPieces and returns true, otherwise returns false and an empty array
	hkBool collectWallBrokenPieces(int wallID, hkArray<hkpRigidBody*>& wallsAddedOut, hkpRigidBody** wallRemovedOut, hkArray<hkpConstraintInstance*>& wgConstraintsOut);
	// Compute and updates linear and angular velocities of the wall bodies from the wrappers bricks velocities 
	void computeVelocitiesForNewBodies( const hkArray<WallWrapper*>& wrappers, const hkVector4& oldLinearVelocity, const hkVector4& oldAngularVelocity, hkArray<hkpRigidBody*>& newBodies, int offset, const hkpRigidBody& oldBody );
	// chooses a sublist of the wrapper's bricks as a sample. the chosen bricks indices are put in sampleBricks
	inline void findSample( const WallWrapper* const wrapper, hkArray<int>& sampleBricks );
	// clears collision events data
	void clearCollisionEventData();
	// Store last velocities at time of impact for a wall in the simulation
	inline void setWallLastImpactVelocities(int wallID, const hkVector4& linear, const hkVector4& angular);
	// stores a collision event
	void addCollisionEvent(int wallID, const hkVector4& impulse, const hkVector4& contactPoint, const hkVector4& linearVelocity, const hkVector4& angularVelocity);

private:
	// simulation timestep
	hkReal m_timeStep;

	// next id to be assigned to a wall in the simulation
	int m_nextWallID;
	
	// the main world 
	hkpWorld* m_mainWorld;
	// the world in which the parallel simulation will run
	hkpWorld* m_parallelWorld;

	// the collision listener
	WallFractureCollisionListener* m_collisionListener;
	BrickConstraintListener* m_constraintListener;

	// the rigid bodies of the walls in the simulations
	hkPointerMap<int, hkpRigidBody*> m_wallsInSimulationBodies;
	// the wrappers of the walls in the simulation
	hkPointerMap<int, WallWrapper*> m_wallsInSimulationWrappers;
	// object in the simulation that aren't walls and therefore are treated separately
	hkArray< hkpRigidBody* > m_nonWallObjects;
	// the list of Ids of the walls to which an impulse was applied during the last simulation step
	hkArray<int> m_wallsToBeChecked;
	// collision events data
	hkPointerMap< int, WallCollisions* > m_wallsCollisions;
	// the velocities at the last time of impact for each wall
	hkPointerMap< int, BodyVelocities* > m_wallsLastImpactVelocities;

	
	// debug stuff
	hkArray<hkVector4> m_debugFracturePlanes;
	hkArray<hkVector4> m_debugcPoints;
	hkArray<hkVector4> m_debugImpulses;
};

#include<Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallSimulation.inl>

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
