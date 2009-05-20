/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This demo shows how to simulate stiff, destructible walls. The displayed walls are simplified representations" 
// of the real brick walls, which are kept in a Parallel Simulation.
// When the brick wall constraints break, the representation is updated to display the broken walls.

#ifndef	HK_BREAKABLECONSTRAINTSWALL_H	
#define HK_BREAKABLECONSTRAINTSWALL_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallBuilder.h>


class hkpConstraintData;
class hkpBreakableConstraintData;
class hkpConstrainedSystemFilter;
class BrickConstraintListener;
class WallFractureCollisionListener;
class WallWrapper;
class WallSimulation;
class WallFractureUtility;

class DestructibleWallsDemo : public hkDefaultPhysicsDemo {

public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	DestructibleWallsDemo(hkDemoEnvironment* env);
	~DestructibleWallsDemo();

public:

	hkDemo::Result stepDemo();

	hkReal m_frameToSimulationFrequencyRatio;

	enum CollsisionDetType {
		SIMPLE,  // Use a collision filter to allow early-outs of certain bricks collisions, faster than FULL
		PARALLEL   // The walls are simulated in a parallel, simpler, world. The main world holds the result of the sim.
	};

	struct Options
	{    
		HK_DECLARE_REFLECTION();
		Options():	
			m_useParallelSimulation(true),
			m_WallsWidth(8),
			m_WallsHeight(16),
			m_bottomConstraintStrength(300),
			m_topConstraintStrength(50),
			m_attachToGround(true),
			m_showDebugDisplay(false),
			m_wireFrameMode(false),
			m_cannonBallRadius(0.5f),
			m_cannonBallMass(50.0f)
		{
		}
		hkBool m_useParallelSimulation;
		int m_WallsWidth;
		int m_WallsHeight;
		hkReal m_bottomConstraintStrength; 
		hkReal m_topConstraintStrength; 
		hkBool m_attachToGround; 
		hkBool m_showDebugDisplay;
		hkBool m_wireFrameMode;
		hkReal m_cannonBallRadius;
		hkReal m_cannonBallMass;
	};

	DEMO_OPTIONS_DECLARE(Options);

private:
	// given a position in space finds its (vertical) projection on the ground
	void posOnGround(hkpWorld* world, const hkVector4& pos, hkVector4& newPos) const;

	// creates the ground box 
	hkpRigidBody* createGround();
	
	// Creates a wall and encapsulates it in a Wrapper, constraints are stored in the passed array
	WallWrapper* createWallWrapper( const BrickwallBuilderDescriptor& bwDescriptor );
	
	// create a single wall. Note: the world must be locked when you call it
	void createAddBrickWall( hkpWorld* world, const BrickwallBuilderDescriptor& wbDescriptor );
	
	// creates the ball
	hkpRigidBody* createBall(const hkVector4& position, const hkVector4& velocity);

	void drawBrick( const hkVector4& position, const hkVector4& halfsize) const;

private:
	// the wall fracture manager
	WallFractureUtility* m_fractureUtility;
	// type of collisions for this demo	
	CollsisionDetType m_collisionDetectionType;

	BrickConstraintListener* m_constraintListener;

	hkReal m_shootingDirX;
	hkReal m_shootingDirY;
	int m_gunCounter;

	hkVector4 m_centerOfScene;

	hkVector4 m_brickHalfExtents;
};


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
