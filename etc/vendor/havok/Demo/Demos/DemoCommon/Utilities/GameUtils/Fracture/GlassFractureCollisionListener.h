/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef GLASS_FRACTURE_LISTENER_H
#define GLASS_FRACTURE_LISTENER_H


#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Fracture/FractureCollisionListener.h>


	/// This is a simple collision listener which you can attach to any body
	/// which you want to fracture using a glass-like fracture pattern
	/// Nearly all the constants in this class are hard coded, so we expect
	/// you to make your own copy of this class in your game and to modify
	/// the parameters based on your specific game play needs.
class GlassFractureCollisionListener : public FractureCollisionListener
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

			/// The constructor. It takes a listener which will be attached to all children
			/// and a velocity threshold, which will be used to decide when the object will brake
		GlassFractureCollisionListener( hkArray<hkpRigidBody*>* outputArrayForFracturedBodies, hkReal fractureVelocity );

			// hkpCollisionListener interface implementation
		virtual void contactPointAddedCallback(	hkpContactPointAddedEvent& event){}

			// hkpCollisionListener interface implementation
		virtual void contactPointRemovedCallback( hkpContactPointRemovedEvent& event ){}

			/// The callback initiating the fracture
		virtual void contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event);

			/// A helper function handling all the parameters and
			/// data flow for the final fracture code
		static void HK_CALL createFracturedRigidBodyPieces(  hkpWorld*             world, 
												hkpRigidBody*         body, 
												const hkContactPoint& contactPointW, 
												int                  pseudoRandomGeneratorSeed, 
												hkArray<hkpRigidBody*>& fracturedBodies);

	public:
			/// The random number seed
		int m_randomSeed;
};

#endif // GLASS_FRACTURE_LISTENER_H

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
