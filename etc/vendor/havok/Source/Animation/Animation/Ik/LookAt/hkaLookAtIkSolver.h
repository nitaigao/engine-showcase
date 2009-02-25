/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef INC_HK_LOOK_AT_IK_SOLVER_H
#define INC_HK_LOOK_AT_IK_SOLVER_H

/// This IK solver operates on a single bone (specified as a transform in model space). It rotates it so the forward axis (m_fwdLS) points towards the
/// specified target (targetMS). The movement is clamped around a cone specified by m_limitAxisMS and m_limitAngle. Finally, the rotation applied by the IK
/// can be weighted by a gain value (gain), in the range [0..1]. The solver returns false if the target is over limits.
class hkaLookAtIkSolver : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME );

		/// Setup information passed to hkaLookAtIkSolver::solve()
		struct Setup
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaLookAtIkSolver::Setup );

			    /// Forward vector in the local space of the bone; must be a unit vector
			hkVector4 m_fwdLS;

				/// "Eye" position in the local space of the bone.
				///	Defaults to (0,0,0). If the offset is not (0,0,0) (the eye is not located
				/// at the joint's position), the required correction is calculated.
			hkVector4 m_eyePositionLS;

				/// Axis of the limiting cone, specified in model space; must be a unit vector
			hkVector4 m_limitAxisMS;

				/// Angle of the limiting cone; must be in range [ 0, pi ]
			hkReal m_limitAngle;
				
				/// Constructor
			Setup()
			{
				m_eyePositionLS.setZero4();
			}

		};

		/// Solves the specified lookAt constraint. Check the documentation for hkaLookAtIkSolver for details.
		static hkBool HK_CALL solve ( const Setup& setup, const hkVector4& targetMS, hkReal gain, hkQsTransform& boneModelSpaceInOut );

};


#endif // INC_HK_LOOK_AT_IK_SOLVER_H

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
