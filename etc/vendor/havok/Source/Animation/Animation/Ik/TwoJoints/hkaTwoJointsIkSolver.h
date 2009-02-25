/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef INC_HKA_TWO_JOINTS_IK_SOLVER_H
#define INC_HKA_TWO_JOINTS_IK_SOLVER_H

#include <Common/Base/hkBase.h>

class hkaPose;

/// This IK solver operates on the two specified joints in order for a third bone/joint to reach
/// a desired position. The first joint is considered to be a ball-socket, while the second joint is considered to be
/// a hinge joint (and therefore an axis must be provided).
/// The joints/bones do not need to be immediate parents of each other (but they must be in the same chain).
class hkaTwoJointsIkSolver : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME );

		/// This setup data is passed to every call of solve()
		struct Setup
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaTwoJointsIkSolver::Setup );

				/// The id of the first joint (ball-socket : thigh, shoulder)
			hkInt16 m_firstJointIdx;

				/// The id of the second joint (hinge : knee, elbow)
			hkInt16 m_secondJointIdx;

				/// The id of the end bone (usually heel, hand, toe)
			hkInt16 m_endBoneIdx;

				/// The hinge axis for the second joint, in local space. Positive rotations (using right hand rule) around this
				/// axis should extend the limb.
			hkVector4 m_hingeAxisLS;

				/// Limit the hinge angle (to avoid knee/elbow artifacts). Default is -1 (180 deg). 
			hkReal m_cosineMaxHingeAngle;

				/// Limit the hinge angle (to avoid knee/elbow artifacts). Default is 1 (0 deg). 
			hkReal m_cosineMinHingeAngle;

				/// Gain of the Ik applied to the first joint (from 0 to 1). You can use this to transition smoothly from/to ik-fixed poses
			hkReal m_firstJointIkGain;

				/// Gain of the Ik applied to the second joint (from 0 to 1). You can use this to transition smoothly from/to ik-fixed poses
			hkReal m_secondJointIkGain;

				/// The target position for the end bone, in model space
			hkVector4 m_endTargetMS;

				// Sets defaults
			Setup() : m_firstJointIdx (-1), m_secondJointIdx(-1), m_endBoneIdx(-1), m_hingeAxisLS(hkVector4::getZero()), m_cosineMaxHingeAngle (-1.0f),
				m_cosineMinHingeAngle(1.0f), m_firstJointIkGain(1.0f), m_secondJointIkGain(1.0f), m_endTargetMS(hkVector4::getZero())
				
			{
			}

		};

			/// Modifies the given pose in order to solve the two two-joint system described in the setup parameter.
		static hkBool HK_CALL solve ( const Setup& setup, hkaPose& poseInOut );

};


#endif // INC_HKA_TWO_JOINTS_IK_SOLVER_H


/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20080925)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2008
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
