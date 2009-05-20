/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This demonstrates how you can find the closest points between a given pair of objects.
// In addition to this we also indicate whether or not a specially optimized agent exists for
// this pair of shapes. If one does then we connect the closest points for this pair with
// a GREEN line. Otherwise the pair are connected by a RED line to indicate that either GJK or
// the more optimized GSK was used to compute the points.
// Another operating mode of this demo is getting the very same information from the normal
// processCollision call. This call produces a manifold of contact points instead of single
// contact points.

#ifndef HK_ClosestPointsDemo_H
#define HK_ClosestPointsDemo_H


#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Physics/Dynamics/Collide/hkpCollisionListener.h>


class hkSimpleClosestPointsDetails;
class hkpRigidBodyCinfo;


class ClosestPointsDemo : public hkDefaultPhysicsDemo, public hkpCollisionListener
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO);

		 ClosestPointsDemo( hkDemoEnvironment* env, bool setupWorldFlag = true );
		~ClosestPointsDemo();

		virtual Result stepDemo();

	protected:

		virtual void contactPointAddedCallback	   ( hkpContactPointAddedEvent&	   event);
		virtual void contactPointConfirmedCallback ( hkpContactPointConfirmedEvent& event);
		virtual void contactPointRemovedCallback   ( hkpContactPointRemovedEvent&   event);
		virtual void contactProcessCallback		   ( hkpContactProcessEvent&		   event);

		void setupWorld(hkDemoEnvironment* env, bool spuSupportedShapesOnly);

		void drawContactPoint(const hkContactPoint& contactPoint, hkBool isGjk);

		static hkpRigidBody* createTriangle( const hkpRigidBodyCinfo& templt, const hkVector4& pos, hkReal mass);

	protected:

		hkArray<hkpRigidBody*> m_bodies;
};


#endif // HK_ClosestPointsDemo_H

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
