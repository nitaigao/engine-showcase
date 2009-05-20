/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SET_BODY_VELOCITY_AS_CRITICAL_OPERATION_H
#define HK_SET_BODY_VELOCITY_AS_CRITICAL_OPERATION_H

#include <Common/Base/hkBase.h>

class hkpRigidBody;
#include <Physics/Dynamics/World/Util/hkpWorldOperationQueue.h>

struct hkGeometry;
	
	/// This class allows you to treat hkpRigidBody::setVelocity as a critical operation.
	/// Normally, if you call hkpRigidBody::setLinearVelocity, the velocity is changed immediately,
	/// without checking for hkpWorld::isLocked().
	/// This class allows you to treat hkpRigidBody::setLinearVelocity as a critical operation, that
	/// means it effect will be delayed until the world is not locked.
	/// This is very useful when you want to force set the velocities of a rigid bodies in a (toi) contact point
	/// confirmed callback, and ignore the velocity changes introduced by solving the toi event.
class SetBodyVelocityAsCriticalOperation : public hkWorldOperation::UserCallback
{
	private:
		SetBodyVelocityAsCriticalOperation() {}

	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

			/// Construct a new instance of this class. To use it you simply have to call:
			/// hkpWorld::queueCallback( new SetBodyVelocityAsCriticalOperation( body, linVel, angVel, reintegrateFlag) )->removeReference()
			/// If reintegrateAndRecollideBodyImmediately is set to true, the engine will recollide the updated body immediately.
			/// This is important if you expect the velocity change to have an effect immediatly and not just at the next PSI.
		SetBodyVelocityAsCriticalOperation(hkpRigidBody* body, const hkVector4& linearVelocity, const hkVector4& angularVelocity, hkBool reintegrateAndRecollideBodyImmediately);

			/// Destructor, typically called by this->removeReference()
		~SetBodyVelocityAsCriticalOperation();

	protected:

			/// hkWorldOperation::UserCallback interface implementations
		void worldOperationUserCallback(hkUlong userData);

	protected:
		hkpRigidBody* m_body;
		hkBool m_reintegrateAndRecollideImmediately;
		hkVector4 m_linearVelocity;
		hkVector4 m_angularVelocity;
};

#endif // HK_SET_BODY_VELOCITY_AS_CRITICAL_OPERATION_H


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
