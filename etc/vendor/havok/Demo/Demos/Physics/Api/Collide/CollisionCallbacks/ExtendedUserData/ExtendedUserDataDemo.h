/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This demo demonstrates the use of Collision Events (for a specific Entity).
// When an entity collides with another object a collision callback is fired.
// By deriving a custom class from the standard hkpCollisionListener class we can
// implement a variety of functions utilizing the collision information.

// In this demo these callbacks have been implemented by the MyExtendedUserDataListener class which,
// upon the addition of a new contact point to the manifold, displays a RED star at that point. It also displays
// a WHITE line to represent the 'normal' whenever the contact is processed by the solver.
// Note that contact points are (generally) 'persistent', and so only get 'added' once (the
// first time the collision occurs), and are not removed until they move outside collision
// tolerance.


#ifndef HK_EXTENDED_USER_DATA_DEMO_H
#define HK_EXTENDED_USER_DATA_DEMO_H


#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Dynamics/Collide/hkpCollisionListener.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>

class MyExtendedUserDataListener;	// Will handle the collision events. Defined below the ExtendedUserDataDemo


class ExtendedUserDataDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO);

		ExtendedUserDataDemo( hkDemoEnvironment* env);

		Result stepDemo();

		class MyExtendedUserDataListener* m_listener;

};



// This specific implementation is not referenced by any other class, but adds itself
// as a listener to the entity. It automatically deletes itself, if the entity gets 
// deleted.	

class MyExtendedUserDataListener : public hkReferencedObject, public hkpCollisionListener, public hkpEntityListener
{

   	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		/*
		 * Construction and Destruction
		 */

		MyExtendedUserDataListener( hkpRigidBody* rigidBody);
        
		~MyExtendedUserDataListener();


		/*
		 * Members from base class hkpCollisionListener which must be implemented:
		 */

		// Called after a contact point was added 
		virtual void contactPointAddedCallback(	hkpContactPointAddedEvent& event );

		virtual void contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event);

		// Called before a contact point gets removed. We do not implement this for this demo.
		virtual void contactPointRemovedCallback( hkpContactPointRemovedEvent& event );

		// Called just before the collisionResult is passed to the constraint system (solved).
		virtual void contactProcessCallback( hkpContactProcessEvent& event );

		/*
		 * Members from base class hkpEntityListener which must be implemented:
		 */


		// Called when the entity is deleted. Important to use this event to remove ourselves as a listener.
		void entityDeletedCallback( hkpEntity* entity );

		// Called when the entity is removed from the hkpWorld
		void entityRemovedCallback( hkpEntity* entity );

	protected:
		// a small structure, which gets attached to each contact point
		int m_uniqueIdCounter;

	public:

		struct ContactPointInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO, ContactPointInfo );

			enum Type
			{
				INVALID,
				PSI, 
				TOI
			};
			Type m_type;
			int m_key;
			hkBool m_added;
			hkBool m_confirmed;
			int m_processed;
			hkBool m_removed;

			ContactPointInfo()
			{
				m_type = INVALID;
				m_added = false;
				m_confirmed = false;
				m_processed = 0;
				m_removed = false;
			}
		};

		hkArray<ContactPointInfo> m_contactInfos;

};

#endif // HK_COLLISION_EVENTS_H


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
