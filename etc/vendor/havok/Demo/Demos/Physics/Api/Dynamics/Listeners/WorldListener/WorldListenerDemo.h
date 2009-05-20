/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

/// This demo illustrates how to dynamically add and remove constraints between two bodies during
/// simulation. A collection of listeners are also added to the world so that we can be notified
/// of various events that are triggered within the simulation.
///
/// The scene consists of two boxes, one of which is dynamic and the other fixed. The dynamic box
/// slowly moves across the screen until it is within a specified distance from the fixed box at
/// which point a constraint is added between the two. The constraint is later removed when another
/// positional based rule is invalidated.

#ifndef HK_WORLDLISTENERAPI_H
#define HK_WORLDLISTENERAPI_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>
#include <Physics/Dynamics/Action/hkpActionListener.h>
#include <Physics/Dynamics/Constraint/hkpConstraintListener.h>
#include <Physics/Dynamics/World/Listener/hkpWorldPostSimulationListener.h>
#include <Physics/Dynamics/World/Listener/hkpWorldPostCollideListener.h>
#include <Physics/Dynamics/World/Listener/hkpWorldDeletionListener.h>


class hkpRigidBody;
class hkpBallAndSocketConstraintData;
class HoverExampleAction;
class MyWorldListener;	// Will handle the world events. Defined below the WorldListenerDemo game



class WorldListenerDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		WorldListenerDemo(hkDemoEnvironment* env);
		~WorldListenerDemo();

		Result stepDemo();

	protected:

		void createBodies();

		void addConstraint();
		void removeConstraint();
		void addAction();
		void removeAction();

	protected:
		
		hkpRigidBody*				m_fixedBody;
		hkpRigidBody*				m_dynamicBody;

		hkpConstraintInstance*				m_constraint;
		hkpBallAndSocketConstraintData*	m_constraintData;
		hkBool						m_constraintAdded;

		HoverExampleAction*			m_hoverExampleAction;
		hkBool						m_actionAdded;

		hkpActionListener*				m_worldActionListener;
		hkpEntityListener*				m_worldEntityListener;
		hkpConstraintListener*			m_worldConstraintListener;
		hkpWorldPostSimulationListener*	m_worldPostIntegrationListener;
		hkpWorldPostCollideListener*	m_worldPostCollideListener;
		hkpWorldDeletionListener*		m_worldDeletionListener;
};


class MyWorldDeletionListener : public hkReferencedObject, public hkpWorldDeletionListener
{
   	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		// Called when the hkpWorld is deleted.
		virtual void worldDeletedCallback( hkpWorld* world);
};

class MyWorldActionListener : public hkReferencedObject, public hkpActionListener
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		// Called when an action is added to the world.
		virtual void actionAddedCallback( hkpAction* action );
		
		// Called when an action is removed from the world.
		virtual void actionRemovedCallback( hkpAction* action );
	
};

class MyWorldEntityListener : public hkReferencedObject, public hkpEntityListener
{
	public:
		
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		// Called when an entity is added to the world.
		virtual void entityAddedCallback( hkpEntity* entity );
		
		// Called when an entity is removed from the world.
		virtual void entityRemovedCallback( hkpEntity* entity );
};

class MyWorldConstraintListener : public hkReferencedObject, public hkpConstraintListener
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		// Called when a constraint is added to the world.
		virtual void constraintAddedCallback( hkpConstraintInstance* constraint );

		// Called when a constraint is removed from the world.
		virtual void constraintRemovedCallback( hkpConstraintInstance* constraint );
};

class MyWorldPostSimulationListener : public hkReferencedObject, public hkpWorldPostSimulationListener
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		// Called at the end of the hkpWorld::simulate call, note this might change
		virtual void postSimulationCallback( hkpWorld* world );
};

class MyWorldPostCollideListener : public hkReferencedObject, public hkpWorldPostCollideListener
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		// Called at the end of the hkpWorld::simulate call, note this might change
		virtual void postCollideCallback( hkpWorld* world, const hkStepInfo& stepInfo );
};


#endif	// HK_WORLDLISTENERAPI_H


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
