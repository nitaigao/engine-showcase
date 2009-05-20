/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Fracture/SetBodyVelocityAsCriticalOperation.h>

//////////////////////////////////////////////////////////////////////////
//
//  Generic
//
//////////////////////////////////////////////////////////////////////////

SetBodyVelocityAsCriticalOperation::SetBodyVelocityAsCriticalOperation(hkpRigidBody* body, const hkVector4& linearVelocity, const hkVector4& angularVelocity, hkBool reintegrateAndRecollideBodyImmediately)
	: m_body(body),
	  m_reintegrateAndRecollideImmediately(reintegrateAndRecollideBodyImmediately),
	  m_linearVelocity(linearVelocity),
	  m_angularVelocity(angularVelocity)
{
	hkReferencedObject::lockAll();
	m_body->addReference();
	hkReferencedObject::unlockAll();
}

SetBodyVelocityAsCriticalOperation::~SetBodyVelocityAsCriticalOperation()
{
	hkReferencedObject::lockAll();
	m_body->removeReference();
	hkReferencedObject::unlockAll();
}

void SetBodyVelocityAsCriticalOperation::worldOperationUserCallback(hkUlong userData)
{
	m_body->setLinearVelocity(m_linearVelocity);
	m_body->setAngularVelocity(m_angularVelocity);

	if (m_reintegrateAndRecollideImmediately)
	{
		hkpWorld* world = m_body->getWorld();
		if (world)
		{
			world->reintegrateAndRecollideEntities( reinterpret_cast<hkpEntity**>(&m_body), 1);
		}
	}
}


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
