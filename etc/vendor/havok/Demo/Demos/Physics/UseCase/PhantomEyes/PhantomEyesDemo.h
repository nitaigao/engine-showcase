/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_PHANTOMDEMO_H
#define HK_PHANTOMDEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

// For base classes
#include <Physics/Dynamics/Action/hkpUnaryAction.h>
#include <Physics/Collide/Shape/Misc/PhantomCallback/hkpPhantomCallbackShape.h>

/// A demo which shows use of phantoms as "eyes" for an AI controlled character
/// The 'bot' bimbles around the room and turns away from the walls to avoid collisions.
/// View in wireframe to see things clearer.

class PhantomEyesDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);


		PhantomEyesDemo(hkDemoEnvironment* env);

	protected:

		void createStaticBox( hkpWorld* world, float centerX, float centerY, float centerZ, float radiusX, float radiusY, float radiusZ );

};


/// An example of a phantom implementation, which just counts the penetrating shapes
class MyPhantomCallbackShape: public hkpPhantomCallbackShape
{
	public:

		MyPhantomCallbackShape();

		long m_penetratingShapesCount;


		virtual void phantomEnterEvent( const hkpCollidable* phantomColl, const hkpCollidable* otherColl,	const hkpCollisionInput& env );

		/// This callback is called when the phantom shape stops intersecting with another shape.
		virtual void phantomLeaveEvent( const hkpCollidable* phantomColl, const hkpCollidable* otherColl );
};


class MyPhantomAction: public hkpUnaryAction
{
	public:

		MyPhantomAction( hkpRigidBody* body, MyPhantomCallbackShape* leftEye, MyPhantomCallbackShape* rightEye );
			/// the hkpAction interface implementation
		virtual void applyAction( const hkStepInfo& stepInfo );
	
			/// no need to clone as we don't use clone func in this demo
		virtual hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const { return HK_NULL; }

	protected:

		MyPhantomCallbackShape* m_leftEye;
		MyPhantomCallbackShape* m_rightEye;
		int m_leftActive;
		int m_rightActive;
};

#endif // HK_PHANTOMDEMO_H

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
