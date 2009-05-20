/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_SpecialEffectsPhysicsDemo_H
#define HK_SpecialEffectsPhysicsDemo_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

	/// This demo shows how to run the graphics and the physics in separate threads.
	/// The difference to the PhysicsInSeparateThreadDemo is that
	/// we use two physical simulations: One physics which is integrated into
	/// the game engine and handles game play related physics and
	/// a second physics running in a separate thread, which is used to 
	/// simulate special effects. Objects in the game play physics are
	/// simply replicated as key-frame objects in the special effects physics.
	/// As a result we get an optional special effects physics. This is
	/// very useful, as we do not have to care about saving the state of that
	/// physics, and we can drop this special effects physics on slower CPUs
	/// altogether without affecting game-play.
class SpecialEffectsPhysicsDemo : public hkDefaultPhysicsDemo
{
	public:
		
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		SpecialEffectsPhysicsDemo(hkDemoEnvironment* environment);

		~SpecialEffectsPhysicsDemo();

		Result stepDemo(); 

	public:
		
		//
		// Functions to copy/clone game-play objects from the game-play simulation to the 
		// visual effects simulation.
		//

		struct TankBodies;

			// Clone all bodies of the tank
		void cloneTankBodies( class TankModel* tank, hkpWorld* world, TankBodies& proxyTank );

			// Key-frame all bodies of the tank into the special effects world
		void keyframeProxyTank( class TankModel* tank, hkpWorld* world, TankBodies& proxyTank );


		//
		// World setup helper functions
		//

			// Create some brick walls
		static void HK_CALL createWalls( hkpWorld* world, const hkVector4& pos, int size );

			// Create the ground floor
		hkpShape* createGround();


	protected:

		//
		// Explicit display handling functions
		//

			// used to disable the default graphics engine
		virtual void setupContexts(hkArray<hkProcessContext*>& contexts);

			// used to call the graphics engine by hand
		class hkpShapeDisplayViewer* m_shapeDisplayViewer;

	public:

			/// The rigid bodies which make the tank
		struct TankBodies
		{
			hkpRigidBody* m_skiL;
			hkpRigidBody* m_skiR;
			hkpRigidBody* m_body;
			hkpRigidBody* m_tower;
			hkpRigidBody* m_gun;
		};

			// 
			// objects in the game-play world
			//
		class TankModel* m_tank;
		hkReferencedObject* m_track;

			//	special effects world
		hkpWorld* m_specialEffectsWorld;
		hkJobQueue* m_specialEffectsJobQueue;

			// The cloned tank model in the special effects world
		TankBodies m_proxyTank;
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
