/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_FIXED_CONSTRAINT_H
#define HK_FIXED_CONSTRAINT_H

	// The construction kit used to build the generic constraint
#include <Physics/Dynamics/Constraint/ConstraintKit/hkpConstraintConstructionKit.h>

/// The attached body of the hkFixedConstraintData will be positioned at
/// m_transformInRef relative to the reference body.
class hkFixedConstraintData : public hkpGenericConstraintData
{
public:
		// Construct the constraint - m_transformInRef initializes to
		// the identity transform (the transform in attached object
		// space is always identity)
	hkFixedConstraintData(hkpRigidBody* att, hkpRigidBody* ref);

	virtual ~hkFixedConstraintData();

		// Set the constraint transform of the attached body in reference
		// object space
	void setTransformInRef(const hkTransform& transform);
		
		// Get the constraint transform of the attached body in reference
		// object space
	const hkTransform& getTransformInRef() const;

protected:
	hkTransform m_transformInRef;

	void _updateTransform();

	hkpParameterIndex m_refPivotIndex;
	hkpParameterIndex m_refAngularBasisIndex;
};

#endif // HK_FIXED_CONSTRAINT_H

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
