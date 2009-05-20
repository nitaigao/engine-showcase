/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

/// This demo illustrates how to use the constraint toolkit to build a user-defined constraint.
/// In this case we build a "rope" constraint by using a single linear constraint, modified during simulation
/// to always be in the direction from the pivot on one body to the pivot on the other. We 
/// illustrate this by fixing one of the bodies. Thus the constraint has the effect of restricting 
/// the motion of the other body so that it is free to move only within a "sphere" of radius equal to the
/// length of the "rope", ie. the linear limit specified. Inside the sphere, the constraint has no effect
/// (the rope is "slack"), it only prevents the body moving outside the sphere.

#ifndef HK_CONSTRAINTKITAPI_H
#define HK_CONSTRAINTKITAPI_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

#include <Physics/Dynamics/Constraint/ConstraintKit/hkpGenericConstraintData.h>


class hkpGenericConstraintData;
class hkpRigidBody;


class ConstraintKitDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		ConstraintKitDemo(hkDemoEnvironment* env);
		~ConstraintKitDemo();

		Result stepDemo(); 

	private:

		hkpConstraintInstance*							m_constraint;
		hkpGenericConstraintData*					m_constraintData;

		hkpGenericConstraintData::hkpParameterIndex	m_axisIndex;
		hkpGenericConstraintData::hkpParameterIndex	m_pivotAIndex;
		hkpGenericConstraintData::hkpParameterIndex	m_pivotBIndex;
		

};

#endif	// HK_CONSTRAINTKITAPI_H

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
