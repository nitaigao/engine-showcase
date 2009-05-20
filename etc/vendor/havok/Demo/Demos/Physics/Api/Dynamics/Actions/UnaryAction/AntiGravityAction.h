/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_ANTIGRAVITYACTION_H
#define HK_ANTIGRAVITYACTION_H

#include <Physics/Dynamics/Action/hkpUnaryAction.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>

/// This action applies a force to a body opposite to a specified "gravity" force and will
/// thus cancels out the current gravity in the world for this body. The force is modulated
/// over time, so sometimes it is stronger then gravity, sometimes is less strong.
class AntiGravityAction: public hkpUnaryAction
{
	public:

		inline AntiGravityAction( hkpRigidBody* body, const hkVector4& gravity );		

		/// no need to clone as we don't use clone func in this demo
		virtual hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const { return HK_NULL; }

	private:

		inline virtual void applyAction( const hkStepInfo& stepInfo );
		
		hkVector4	m_gravity;
		hkReal		m_modulateStrength;
};

#include <Demos/Physics/Api/Dynamics/Actions/UnaryAction/AntiGravityAction.inl>

#endif // HK_ANTIGRAVITYACTION_H

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
