/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_LAUNCH_PAD_LISTENER_H
#define HK_LAUNCH_PAD_LISTENER_H

#include <Physics/Dynamics/Collide/hkpCollisionListener.h>

class LaunchPadListener : public hkReferencedObject, public hkpCollisionListener
{
	public:

		// Constructor for "basic" launchpad (launches along contact normal)
		LaunchPadListener( hkReal forceMagnitude = 1.0f ) :
		  m_forceMagnitude( forceMagnitude ), m_targetPosition( hkVector4::getZero() ) {}

		// Constructor for launchpad which launches toward a target
		LaunchPadListener( const hkVector4& targetPosition ) :
		  m_forceMagnitude( 0.0f ), m_targetPosition( targetPosition ) {}

		virtual void contactPointAddedCallback( hkpContactPointAddedEvent& event );

		// Empty definitions
		virtual void contactPointRemovedCallback( hkpContactPointRemovedEvent& event ) {}
		virtual void contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event ) {}
		virtual void contactProcessCallback( hkpContactProcessEvent& event ) {}

	private:
		hkReal m_forceMagnitude;
		const hkVector4& m_targetPosition;
};

#endif  // HK_LAUNCH_PAD_LISTENER_H

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
