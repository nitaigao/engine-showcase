/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Constraints/ConstraintKit2/hkFixedConstraint.h>

///[TUTORIAL 3]

///[CODE createfixedconstraintcode]<
hkFixedConstraintData::hkFixedConstraintData(hkpRigidBody* att, hkpRigidBody* ref)
: hkpGenericConstraintData()
{
	hkpConstraintConstructionKit kit;
	kit.begin(this);

	// identity transform in the attached body (even though it doesn't have to be)
	kit.setPivotA( hkVector4::getZero() );
	hkRotation rot; rot.set( hkQuaternion::getIdentity() );
	kit.setAngularBasisA( rot );

	// this defaults to identity; can be set later using setTransformInRef
	m_transformInRef.setTranslation( hkVector4::getZero() );
	m_transformInRef.setRotation( hkQuaternion::getIdentity() );

	// remember the indices of the translation and rotation components
	// (used in _updateTransform to update the constraint parameters)
	m_refPivotIndex = kit.setPivotB( m_transformInRef.getTranslation() );
	m_refAngularBasisIndex = kit.setAngularBasisB( m_transformInRef.getRotation() );
	setTransformInRef( m_transformInRef );

	// set the constraint to constrain all linear and angular degrees of freedom
	kit.constrainAllLinearDof();
	kit.constrainAllAngularDof();

	kit.end();
}
///[CODE createfixedconstraintcode]>

hkFixedConstraintData::~hkFixedConstraintData()
{
}

void hkFixedConstraintData::setTransformInRef(const hkTransform& transform)
{
	m_transformInRef = transform;
	_updateTransform();
}

const hkTransform& hkFixedConstraintData::getTransformInRef() const
{
	return m_transformInRef;
}

///[CODE updatefixedconstraintcode]<
void hkFixedConstraintData::_updateTransform()
{
	// set the constraint parameters from the transform member
	setParameters(m_refPivotIndex, 1, &m_transformInRef.getTranslation());
	setParameters(m_refAngularBasisIndex, 3, &m_transformInRef.getRotation().getColumn(0));		
}
///[CODE updatefixedconstraintcode]>

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
