/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// A demo which shows a single ball and socket constraint

#ifndef HK_WINDCHIMES_H
#define HK_WINDCHIMES_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Dynamics/Collide/hkpCollisionListener.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>

#include <Common/Base/Types/Color/hkColor.h>


class WindChimesDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		WindChimesDemo(hkDemoEnvironment* env);
		~WindChimesDemo();

		class hkLoader* m_loader;


};

class WindChimesCollisionListener : public hkReferencedObject, public hkpCollisionListener, public hkpEntityListener
{

   	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		/*
		 * Construction and Destruction
		 */

		WindChimesCollisionListener( hkpRigidBody* rigidBody,  int noteIndex);

		~WindChimesCollisionListener();

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

		// A small structure, which gets attached to each contact point
		struct  ContactPointInfo
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO,  ContactPointInfo);
			int m_uniqueId;
		};
		int m_uniqueIdCounter;

		int m_noteIndex;

};



#endif	// HK_WINDCHIMES_H

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
