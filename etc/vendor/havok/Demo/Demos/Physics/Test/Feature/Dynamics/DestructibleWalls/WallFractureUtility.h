/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef WALL_FRACTURE_UTILITY
#define WALL_FRACTURE_UTILITY

#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallSimulation.h>

// The only class needed to add destructible walls. Acts as a Facade wrapping the WallSimulation.
class WallFractureUtility
{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, WallFractureUtility);

	// The world must be provided in construction toghether with the simulation timestep. The latter should be the same used
	// in the main simulation
	WallFractureUtility(hkpWorld* mainWorld, hkReal timeStep=-1.0f);
	~WallFractureUtility();

	// adds a wall
	inline WallWrapper* addWallToSimulation(  const BrickwallBuilderDescriptor& bwDescriptor  );	

	inline void Update();

	inline WallSimulation* getSimulation();

private:

	WallSimulation* m_wallSimulation;
};

#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/WallFractureUtility.inl>

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
