/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_RAGDOLLVSMOPP_H
#define HK_RAGDOLLVSMOPP_H


///[TUTORIAL 1]

///[introduction_text] Introduction
///[createragdoll_text] Create Ragdoll
///[addragdoll_text] Add ragdoll
///[removeragdoll_text] Remove ragdoll
///[CompleteListing] The Complete Code Listing

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkRagdoll;
class hkpWorld;
class hkpConvexVerticesShape;
class hkRagdollViewer;
class hkDrawUtil;
class KeyValueChecker;
class hkpGroupFilter;


///[introduction_text]
/// This demo shows a single 11 bone ragdoll colliding again a room made
/// from a concave mopp geometry. 
/// [IMAGE Resources/ragdollVsMoppDemo.png]
///
///
/// The ragdoll can be constructed from a number shapes types including:
/// - Box
/// - Multi-sphere
/// - Sphere
/// - Convex
/// - Line segment
/// The left and right arrow will change the ragdolls type.
/// The up arrow will throw the ragdoll from the camera position into the scene.
/// The down arrow will reinitialise the ragdoll to its standard t-pose before
/// throwing it into the screen.
/// Buttons 2 through 4 drop the ragdoll into interesting parts of the room.
///
/// For a more specific example of how to set up constraints, and what each limit means,
/// please check out the constraint api demos.


class ragdollVsMoppDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		ragdollVsMoppDemo(hkDemoEnvironment* environment);
		~ragdollVsMoppDemo();

		Result stepDemo(); 
		
		struct Parameter 
		{
			const char*					 m_name;
			hkpWorldCinfo::SimulationType m_simulationType;
			hkReal						 m_frameFrequency;
			hkReal						 m_physicsFrequency;
			hkpWorldCinfo::SolverType     m_solverType;
			hkReal                       m_inertiaFactor;
			const char*					 m_details;
		};

	private:


        static void addRagdoll( hkpWorld* world, hkpPhysicsSystem*	ragdoll, hkReal inertiaFactor);

		void removeRagdoll();
		static void HK_CALL setTransformAndVelocity( const hkTransform&, const hkVector4&, hkpPhysicsSystem* ragdoll );
		void setTransformAndVelocityFromCamera(hkpPhysicsSystem* ragdoll);

		hkpPhysicsSystem*		m_ragdoll;
		int				m_ragdollType;
		hkpGroupFilter*	m_filter;

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
