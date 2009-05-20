/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEFAULTPHYSICSDEMO_H
#define HK_DEFAULTPHYSICSDEMO_H


#include <Demos/demos.h>

#include <Physics/Collide/Filter/Group/hkpGroupFilter.h>
#include <Physics/Collide/Filter/Group/hkpGroupFilterSetup.h>

#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics/Collide/Shape/Convex/Triangle/hkpTriangleShape.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>

#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>
#include <Physics/Utilities/Actions/MouseSpring/hkpMouseSpringAction.h>

#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>

class hkLoader;
struct hkTimerData;

class hkDefaultPhysicsDemo : public hkDefaultDemo 
{
	public:

		enum DemoFlags
		{
			DEMO_FLAGS_NONE = 0,
			DEMO_FLAGS_NO_SERIALIZE = 1<<0
		};

	public:

			/// The constructor
		hkDefaultPhysicsDemo(hkDemoEnvironment* env, DemoFlags = DEMO_FLAGS_NONE);

			/// The destructor
		virtual ~hkDefaultPhysicsDemo();

		void runPhysicsMultithreaded(int numThreads);


		virtual Result stepDemo();

		void startNewSimulateStep();
		virtual void waitForStepCompletion();


			/// Call m_world->stepAsynchronously using the m_jobThreadPool
		virtual hkpStepResult stepAsynchronously(hkpWorld* world, hkReal frameDeltaTime, hkReal physicsDeltaTime);
	
			/// To draw our clock overlay if we want it
		virtual void postRenderDisplayWorld(class hkgViewport* view);

		void updateDisplay(hkpWorld* w);

		void enableDisplayingToiInformation(hkBool enable);

		virtual void setupContexts(hkArray<hkProcessContext*>& contexts);

		virtual hkpPhysicsContext* getPhysicsViewerContext() { return m_physicsViewersContext; }


		virtual void addOrRemoveThreads();
		virtual void addTimersToVdb( const hkArray<hkTimerData>& threadStreams, const hkArray<hkTimerData>& spuStreams );

			// Given a system and converted scene data objects, register the 
			// display objects in the scene with rigid bodies of the same name.
		int addPrecreatedDisplayObjectsByName( const hkArray<class hkpRigidBody*>& rbs, const class hkxScene* scene, hkArray<hkgDisplayObject*>* createdDisplayObjects = HK_NULL, bool castShadows = true);
		hkgDisplayObject* findMeshDisplay( const char* meshName, const hkxScene*  );

		void loadAndAddRigidBodies( hkLoader* loader, const char* filename );


		virtual void setAutoInstancingEnabled( bool on ); // do you want the local shape viewer to use instancing?
		virtual void setAutoDisplayCachingEnabled( bool on ); // do you want the local shape viewer to cache all hkgGeometries and reuse for each shape?


		/// called right before the demo gets deleted
		virtual void preDeleteDemo();

	protected:

			/// Called by mouseDown, should return true if it picks an object
		virtual hkBool objectPicked( const hkgDisplayObject* displayObject, const hkVector4& worldPosition, int geomIndex  );

			/// Called by mouseUp
		virtual void objectReleased();

			/// Called by mouseDrag
		virtual void objectDragged( const hkVector4& newWorldSpacePoint );

			//
			//	Mouse spring helpers
			//
			/// The mouse spring (modified by the objectXXX) functions
		hkpMouseSpringAction* m_mouseSpring;
			/// The max acceleration the mouse spring will apply to objects
		hkReal m_mouseSpringMaxRelativeForce;
		
			/// The context.
		hkpPhysicsContext* m_physicsViewersContext;

			/// Toggles displaying of PSI clock and (to be implemented:) some TOI information
		hkBool   m_displayToiInformation;

		// Internal
	public:


		void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls ) const ;

	public:

			/// Our world
		hkpWorld* m_world;

		int      m_physicsStepCounter;

		hkBool	 m_forcedFPSChange;
		int		 m_oldFPSLock;

		bool	m_simulationStarted;

		hkBool m_oldForceMultithreadedSimulation;

		hkEnum<DemoFlags, hkUint32> m_flags;


			/// This allows you to link custom code to the mouse spring action 
		hkArray<hkpMouseSpringAction::mouseSpringAppliedCallback> m_mouseSpringAppliedCallbacks;
};

#endif // HK_DEFAULTPHYSICSDEMO_H

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
