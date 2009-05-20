/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>


#include <Physics/Dynamics/World/hkpPhysicsSystem.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>
#include <Demos/Physics/Test/Feature/Dynamics/Stability/SolverNanTest/SolverNanTest.h>


SolverNanTest::SolverNanTest(hkDemoEnvironment* env)
:	RagdollOnStairsDemo(env)
{
	m_frameCounter = 0;
}

SolverNanTest::~SolverNanTest()
{
}

hkDemo::Result SolverNanTest::stepDemo()
{
	m_world->lock();
	{
		m_frameCounter++;
		static hkReal zero = 0.0f;
		emptyFunction();
		hkReal inf = (1.0f / zero) * 10.0f;
		hkReal nan = inf / inf;
		hkVector4 nan4; nan4.setAll( nan );

		if ( (m_frameCounter& 0xf) == 0 )
		{
			hkpRigidBody* body = m_ragdolls[(m_frameCounter*34543)%m_ragdolls.getSize()]->getRigidBodies()[0];
			body->getRigidMotion()->m_linearVelocity = nan4;
		}

		if ( (m_frameCounter& 0x3f) == 2 )
		{
			hkpRigidBody* body = m_ragdolls[(m_frameCounter*34543)%m_ragdolls.getSize()]->getRigidBodies()[0];
			// search constraint
			hkSmallArray<struct hkConstraintInternal>& masters = body->getConstraintMastersRw();
			for (int i =0; i < masters.getSize(); i++ )
			{
				hkpConstraintAtom* atom = masters[i].getAtoms();
				if ( atom->m_type == hkpConstraintAtom::TYPE_CONTACT)
				{
					hkpSimpleContactConstraintAtom* scca = (hkpSimpleContactConstraintAtom*)atom;
					if ( scca->m_numContactPoints )
					{
						scca->getContactPoints()[0].setNormal(nan4);
					}
					break;
				}
			}
		}
	}

	m_world->unlock();
	hkError::getInstance().setEnabled(0x75d662fb, false);
	Result result = RagdollOnStairsDemo::stepDemo();
	hkError::getInstance().setEnabled(0x75d662fb, true);

	return result;
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = "Setting random nans in the contact data";

HK_DECLARE_DEMO(SolverNanTest, HK_DEMO_TYPE_PHYSICS, helpString, helpString );

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
