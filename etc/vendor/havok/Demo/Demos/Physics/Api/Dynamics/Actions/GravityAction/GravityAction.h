/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// This action applies a "gravity" force to a satellite body, attracting it to a fixed point.

#ifndef HK_GRAVITY_ACTION_H
#define HK_GRAVITY_ACTION_H

#include <Physics/Dynamics/Action/hkpUnaryAction.h>


class GravityAction: public hkpUnaryAction
{
	public:
		// Constructor takes fixed point, mobile satellite body and a scaling factor
		// for the force to be applied.
		inline GravityAction( hkpRigidBody* satellite, const hkVector4 &gravityCenter, const hkReal gravityConstant );
			
	private:
		// applyAction Called by the simulation every timestep.
		inline virtual void applyAction( const hkStepInfo& stepInfo );

		virtual hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const { return HK_NULL; }

		// Point toward which, the gravity force acts.
		hkVector4 m_gravityCenter;

		// A constant scaling factor.
		hkReal m_gravityConstant;
};

#include <Demos/Physics/Api/Dynamics/Actions/GravityAction/GravityAction.inl>

#endif // HK_GRAVITY_ACTION_H

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
