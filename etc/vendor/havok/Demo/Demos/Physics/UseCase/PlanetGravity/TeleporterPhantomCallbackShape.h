/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_TELEPORTER_PHANTOM_CALLBACK_SHAPE_H
#define HK_TELEPORTER_PHANTOM_CALLBACK_SHAPE_H

#include <Physics/Collide/Shape/Misc/PhantomCallback/hkpPhantomCallbackShape.h>

class hkTransform;
class hkpCollidable;

// Teleport rigid bodies when they enter the phantom's shape
class TeleporterPhantomCallbackShape : public hkpPhantomCallbackShape
{
	public:
		// Set the destination transform (position+orientation) on construction
		TeleporterPhantomCallbackShape( const hkTransform& destTransform ) : m_destTransform( destTransform ) {}

		// Performs the teleport when rigid body enters the shape of the phantom
		virtual void phantomEnterEvent( const hkpCollidable* phantomColl, const hkpCollidable* otherColl, const hkpCollisionInput& env );

		// This has to have an implementation or else this is a pure virtual class
		virtual void phantomLeaveEvent( const hkpCollidable* phantomColl, const hkpCollidable* otherColl ) {}

	private:
		const hkTransform& m_destTransform;
};

#endif // HK_TELEPORTER_PHANTOM_CALLBACK_SHAPE_H

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
