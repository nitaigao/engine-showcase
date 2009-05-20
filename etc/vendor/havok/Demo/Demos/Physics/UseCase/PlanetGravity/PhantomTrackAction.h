/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_PHANTOM_TRACK_ACTION_H
#define HK_PHANTOM_TRACK_ACTION_H

#include <Physics/Dynamics/Action/hkpUnaryAction.h>

class SimpleGravityPhantom;
class hkpAabbPhantom;

// Just in case you want the phantom to be updated with a body
// It will also then update the center for any overlapping bodies with gravity actions associated 
// with the body. For now it will just center the Aabb around the body's position but could be extended
// for an offset transform
class PhantomTrackAction: public hkpUnaryAction
{
	public:
		// Constructor takes the rigid body it should track and the phantom it should update
		PhantomTrackAction( hkpRigidBody* trackedBody, hkpAabbPhantom* phantomToUpdate );
	private:
		// Move the phantom m_phantom so it tracks trackedBody
		virtual void applyAction( const hkStepInfo& stepInfo );

		virtual hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const 
		{ 
			return HK_NULL; 
		}

		// Phantom to track the body.
		 hkpAabbPhantom* m_phantom;
};

#endif // HK_PHANTOM_TRACK_ACTION_H

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
