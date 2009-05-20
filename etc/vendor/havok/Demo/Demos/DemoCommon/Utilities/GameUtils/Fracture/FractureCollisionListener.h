/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef FRACTURE_LISTENER_H
#define FRACTURE_LISTENER_H


#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Dynamics/Collide/hkpCollisionListener.h>


	/// This is a simple collision listener which you can attach to any body
	/// which you want to fracture using a glass-like fracture pattern
	/// Nearly all the constants in this class are hard coded, so we expect
	/// you to make your own copy of this class in your game and to modify
	/// the parameters based on your specific game play needs.
class FractureCollisionListener : public hkReferencedObject, public hkpCollisionListener
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

			// FractureCollisionListeners constructor. Specifies an output array for bodies resulting from fracture. The array may be null.
		FractureCollisionListener(hkArray<hkpRigidBody*>* outputArrayForFracturedBodies) : m_fracturedBodies(outputArrayForFracturedBodies) {}

			// hkpCollisionListener interface implementation
		virtual void contactPointAddedCallback(	hkpContactPointAddedEvent& event) = 0;

			// hkpCollisionListener interface implementation
		virtual void contactPointRemovedCallback( hkpContactPointRemovedEvent& event ) = 0;

			/// The callback initiating the fracture
		virtual void contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event) = 0;

	public:
			/// The minimum velocity of the two colliding objects before the object is broken
		hkReal m_fractureVelocity;

			/// Pointer to the array storing references to the newly created bodies.
			/// It can be HK_NULL.
		hkArray<hkpRigidBody*>* m_fracturedBodies;
};

#endif // FRACTURE_LISTENER_H

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
