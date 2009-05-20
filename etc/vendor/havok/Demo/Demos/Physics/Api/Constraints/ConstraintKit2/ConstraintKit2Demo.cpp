/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Constraints/ConstraintKit2/ConstraintKit2Demo.h>
#include <Demos/Physics/Api/Constraints/ConstraintKit2/hkFixedConstraint.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>

#include <Physics/Dynamics/Constraint/ConstraintKit/hkpGenericConstraintData.h>

// We will need these collision agents
#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>


ConstraintKit2Demo::ConstraintKit2Demo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE)
{
	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 5.0f, 15.0f);
		hkVector4 to  (0.0f, 2.0f, 0.0f);
		hkVector4 up  (0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM); 
		info.setBroadPhaseWorldSize( 100.0f );
		m_world = new hkpWorld( info );
		m_world->lock();
		m_world->m_wantDeactivation = false;

		setupGraphics();
	}

	//
	// Set up a collision filter so we can disable collision between the objects we constrain together
	//
	{
		hkpGroupFilter* groupFilter = new hkpGroupFilter;
		hkpGroupFilterSetup::setupGroupFilter( groupFilter );

		m_world->setCollisionFilter( groupFilter );
		groupFilter->removeReference();
	}

	//
	// Register the box-box collision agent
	//
	{
		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );
	}

	//
	// Create a fixed 'floor' box
	//
	const hkVector4 floorSize(10.0f, 0.5f, 10.0f);
	{
		m_world->addEntity( GameUtils::createBox( floorSize, 0.0f, hkVector4(0,-1,0) ) )->removeReference();
	}

	// We create a 'diagonal line' of boxes; each new box after the first will have
	// its transform fixed in its initial position (actually expressed as a relative 
	// offset to the previous box).

	const int numBoxes = 5;
	{
		hkpRigidBody* lastBox = 0;

		for(int boxCount = 0; boxCount < numBoxes; boxCount++)
		{
			hkVector4 pos( (hkReal)boxCount - ((hkReal)numBoxes-1) / 2.0f, (hkReal)boxCount, 0.0f);
			hkpRigidBody* box = GameUtils::createBox(hkVector4(1,1,1),10.0f,	pos );

			// Set filter info to be in group 1, so all boxes are in same group and don't collide
			box->setCollisionFilterInfo( hkpGroupFilter::calcFilterInfo(0,1) );
			m_world->addEntity(box);
			box->removeReference();

			if( lastBox )
			{
				// If there is a previous box, then we create a constraint between that box, and
				// the box just added. Here is the code that initially constructs the hkFixedConstraint:
				hkFixedConstraintData* fixedConstraint = new hkFixedConstraintData(box, lastBox);

				// Now we set the transform of the attached box in reference box space (the product
				// of inv(refToWorld) * attToWorld)
				hkTransform attToRef;
				{
					hkTransform refToWorld = lastBox->getTransform();
					hkTransform attToWorld = box->getTransform();
					attToRef.setMulInverseMul(refToWorld, attToWorld);
				}

				// This modifies the internal parameters of the constraint as follows:
				fixedConstraint->setTransformInRef(attToRef);

				m_world->createAndAddConstraintInstance(box, lastBox, fixedConstraint)->removeReference();
				fixedConstraint->removeReference();
			}

			lastBox = box;
		}
	}

	m_world->unlock();
}

////////////////////////////////////////////////////////////////////////////////

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo uses a fixed constraint (constrains all degrees of freedom) built "\
"using the constraint kit.\n" \
"This is intended to be another example of how to use the hkGenericConstraint/ConstraintKit.\n" \
"N.B. Because Havok constraints are not completely rigid this sort of constraint " \
"should not be used for bodies which must be absolutely 'fixed' (use a FIXED body, or a body with a compound shape).\n" \
"Also, since this is a constraint it will cost more CPU than a compound body so be wary about using this " \
"in conjunction with a hkBreakableConstraint to try to simulate 'fracture'.";

HK_DECLARE_DEMO(ConstraintKit2Demo, HK_DEMO_TYPE_PRIME, "Using the ConstraintKit to build a 'fixed' constraint", helpString);

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
