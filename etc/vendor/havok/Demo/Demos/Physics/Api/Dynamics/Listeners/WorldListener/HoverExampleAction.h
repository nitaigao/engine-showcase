/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// After starting the demo you'll notice a bobbing box slowly drifting across the screen.
// The motion of this box is controlled by the HoverExampleClass which applies a force to a body
// opposite to a specified 'gravity' force and will thus cancel out the current gravity in the world for this body.
// The force is modulated over time, sometimes being stronger than gravity and other times less strong.

// Since we are dealing with an action, the core work is all performed within the 'apply' method of the action
// and is given below.

#ifndef HK_HOVEREXAMPLECTION_H
#define HK_HOVEREXAMPLECTION_H

#include <Physics/Dynamics/Action/hkpUnaryAction.h>
#include <Common/Visualize/hkDebugDisplay.h>


class HoverExampleAction: public hkpUnaryAction
{
	public:

		inline HoverExampleAction( hkpRigidBody* body, const hkVector4& gravity , hkReal desiredHeight );

		inline void init( const hkVector4& gravity , hkReal desiredHeight );
			
		// no need to clone as we don't use clone func in this demo
		virtual hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const { return HK_NULL; }

	private:

		inline virtual void applyAction( const hkStepInfo& stepInfo );
		
		hkVector4	m_gravity;
		hkReal	m_desiredHeight;
};

#include <Demos/Physics/Api/Dynamics/Listeners/WorldListener/HoverExampleAction.inl>

#endif // HK_HOVEREXAMPLECTION_H

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
