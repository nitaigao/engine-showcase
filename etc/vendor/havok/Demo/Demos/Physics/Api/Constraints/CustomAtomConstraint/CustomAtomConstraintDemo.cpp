/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>

#include <Demos/Physics/Api/Constraints/CustomAtomConstraint/CustomAtomConstraintDemo.h>
#include <Physics/Dynamics/hkpDynamics.h>
#include <Physics/Dynamics/Constraint/hkpConstraintData.h>
#include <Physics/ConstraintSolver/Constraint/Atom/hkpConstraintAtom.h>
#include <Physics/ConstraintSolver/Solve/hkpSolverResults.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpConstraintViewer.h>
#include <Common/Base/Math/Vector/hkVector4Util.h>

class hkCustomConstraintData : public hkpConstraintData
{
public:
	hkCustomConstraintData();
	~hkCustomConstraintData(){}

	enum
	{

		SOLVER_2DANG_RESULT_1 = 0,
		SOLVER_2DANG_RESULT_2,

		SOLVER_BALLSOCKET_RESULT_1,
		SOLVER_BALLSOCKET_RESULT_2,
		SOLVER_BALLSOCKET_RESULT_3,

		SOLVER_RESULT_MAX
	};

	//Tests to ensure the state of the atom is valid.
	//Should not always return true
	hkBool isValid() const {return true;}

	//Returns the type of the constraint
	int getType() const {return hkpConstraintData::CONSTRAINT_TYPE_CUSTOM;}

	//This structure should be the same for every ConstraintData
	struct Runtime
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkCustomConstraintData::Runtime );

		class hkpSolverResults m_solverResults[SOLVER_RESULT_MAX];
	};

	//Return the constraintRuntime as a Runtime struct
	static inline Runtime* HK_CALL getRuntime( hkpConstraintRuntime* runtime ) { return reinterpret_cast<Runtime*>(runtime); }

	//Will fill infoOut with the runtimeInfo if wantRuntime is set
	void getRuntimeInfo( hkBool wantRuntime, hkpConstraintData::RuntimeInfo& infoOut ) const 
	{
		if ( wantRuntime )
		{
			infoOut.m_numSolverResults = SOLVER_RESULT_MAX;
			infoOut.m_sizeOfExternalRuntime = sizeof( Runtime);
		}
		else
		{
			infoOut.m_numSolverResults = 0;
			infoOut.m_sizeOfExternalRuntime = 0;
		}
	}

	//Fill infoOut with a pointer to the atoms and the size of the atoms
	void getConstraintInfo( hkpConstraintData::ConstraintInfo& infoOut ) const 
	{
		getConstraintInfoUtil( m_atoms.getAtoms(), m_atoms.getSizeOfAllAtoms(), infoOut );
	}


	//This is the atoms structure containing all the constraintAtoms. Refer to the
	//constraintData documentation for a description of its contents.
	struct Atoms
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkCustomConstraintData::Atoms );
		HK_DECLARE_REFLECTION();

		struct hkpSetLocalTransformsConstraintAtom	m_transforms;

		struct hkp2dAngConstraintAtom				m_ang1;
		struct hkpBallSocketConstraintAtom			m_socket;

		Atoms(){}

		// get a pointer to the first atom
		const hkpConstraintAtom* getAtoms() const { return &m_transforms; }

		// get the size of all atoms (we can't use sizeof(*this) because of align16 padding)
		int getSizeOfAllAtoms() const               { return hkGetByteOffsetInt(getAtoms(), &m_socket+1); }

		Atoms(hkFinishLoadedObjectFlag f) : m_transforms(f), m_ang1(f),m_socket(f) {}
	};

	//Create an Atoms structure and align it on a 16 byte boundary
	HK_ALIGN16( struct Atoms m_atoms );

	//Used to change the attributes of the hkSetLocalTranstormsConstraintAtom to position the effect
	//of the constraint on the rigid bodies. 
	//Use when positioning the rigid bodies and constraint in the world.
	void setInWorldSpace( const hkTransform& bodyATransform, const hkTransform& bodyBTransform,
		const hkVector4& pivot, const hkVector4& axis ) 
	{
		hkVector4 perpToAxle1;
		hkVector4 perpToAxle2;
		hkVector4Util::calculatePerpendicularVector( axis, perpToAxle1 ); perpToAxle1.normalize3();
		perpToAxle2.setCross(axis, perpToAxle1);

		m_atoms.m_transforms.m_transformA.getColumn(0).setRotatedInverseDir(bodyATransform.getRotation(), axis);
		m_atoms.m_transforms.m_transformA.getColumn(1).setRotatedInverseDir(bodyATransform.getRotation(), perpToAxle1);
		m_atoms.m_transforms.m_transformA.getColumn(2).setRotatedInverseDir(bodyATransform.getRotation(), perpToAxle2);
		m_atoms.m_transforms.m_transformA.getTranslation().setTransformedInversePos(bodyATransform, pivot);

		m_atoms.m_transforms.m_transformB.getColumn(0).setRotatedInverseDir(bodyBTransform.getRotation(), axis);
		m_atoms.m_transforms.m_transformB.getColumn(1).setRotatedInverseDir(bodyBTransform.getRotation(), perpToAxle1);
		m_atoms.m_transforms.m_transformB.getColumn(2).setRotatedInverseDir(bodyBTransform.getRotation(), perpToAxle2);
		m_atoms.m_transforms.m_transformB.getTranslation().setTransformedInversePos(bodyBTransform, pivot);

		HK_ASSERT2(0x3a0a5292, isValid(), "Members of constraint inconsistent after World Space changed.");
	}

	//Similar to setInWorldSpace() but positions the constraint in the bodies local space, rather than the world
	void setInBodySpace( const hkVector4& pivotA,const hkVector4& pivotB,
		const hkVector4& axisA,const hkVector4& axisB)
	{
		m_atoms.m_transforms.m_transformA.getTranslation() = pivotA;
		m_atoms.m_transforms.m_transformB.getTranslation() = pivotB;

		hkVector4* baseA = &m_atoms.m_transforms.m_transformA.getColumn(0);
		baseA[0] = axisA; baseA[0].normalize3();
		hkVector4Util::calculatePerpendicularVector( baseA[0], baseA[1] ); baseA[1].normalize3();
		baseA[2].setCross( baseA[0], baseA[1] );

		hkVector4* baseB = &m_atoms.m_transforms.m_transformB.getColumn(0);
		baseB[0] = axisB; baseB[0].normalize3();
		hkVector4Util::calculatePerpendicularVector( baseB[0], baseB[1] ); baseB[1].normalize3();
		baseB[2].setCross( baseB[0], baseB[1] );

		HK_ASSERT2(0x3a0a5292, isValid(), "Members of constraint inconsistent after Body Space changed.");
	}
};


//Initialize the attributes in the Atoms
hkCustomConstraintData::hkCustomConstraintData()
{
	//Initialize the transforms to something sensible
	//They will be set to their actual values when setInWorldSpace is called
	m_atoms.m_transforms.m_transformA.setIdentity();
	m_atoms.m_transforms.m_transformB.setIdentity();

	//Allow the rigid bodies to only rotate about the first axis (the axis passed to setInWorldSpace)
	m_atoms.m_ang1.m_freeRotationAxis = 0;

	//hkBallSocketConstraintAtom requires no parameters to work
}


CustomAtomConstraintDemo::CustomAtomConstraintDemo(hkDemoEnvironment* env)
:hkDefaultPhysicsDemo(env)
{

	//
	// Setup the camera
	//
	{
		hkVector4 from(0.0f, 0.0f, 10.0f);
		hkVector4 to(0.0f, 0.0f, 0.0f);
		hkVector4 up(0.0f, 1.0f, 0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register the single agent required (a hkBoxBoxAgent)
		hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

		setupGraphics();
	}

	//
	// Create vectors to be used for setup
	//
	hkVector4 halfSize(0.5f, 0.5f, 0.5f);
	hkVector4 size;
	size.setMul4(2.0f, halfSize);
	hkVector4 position(size(0), -size(1), 0.0f);


	//
	// Create Box Shape
	//
	hkpBoxShape* boxShape = new hkpBoxShape( halfSize , 0 );

	//
	// Create fixed rigid body
	//
	hkpRigidBody* rb0;
	{
		hkpRigidBodyCinfo info;
		info.m_position.set(0.0f, 0.0f, 0.0f);
		info.m_shape = boxShape;
		info.m_motionType = hkpMotion::MOTION_FIXED;

		rb0 = new hkpRigidBody(info);
		m_world->addEntity(rb0);
		rb0->removeReference();	
	}

	//
	// Create movable rigid body
	//
	hkpRigidBody* rb1;
	{

		hkpRigidBodyCinfo info;
		info.m_position = position;
		info.m_shape = boxShape;

		// Compute the box inertia tensor
		hkpMassProperties massProperties;
		info.m_mass = 10.0f;
		hkpInertiaTensorComputer::setShapeVolumeMassProperties(info.m_shape, info.m_mass, info);
		info.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

		rb1 = new hkpRigidBody(info);
		m_world->addEntity(rb1);
		rb1->removeReference();	
	}

	// Remove reference from box shape since rigid bodies now have ownership
	boxShape->removeReference();


	//Test the custom constraint:
	hkCustomConstraintData* cc = new hkCustomConstraintData();
	hkVector4 axis(0.0f, 0.0f, 1.0f);
	hkVector4 pivot;
	pivot.setAdd4(rb1->getCenterOfMassInWorld(),rb0->getCenterOfMassInWorld());
	pivot.mul4(0.5f);
	cc->setInWorldSpace(rb0->getTransform(), rb1->getTransform(), pivot, axis);		
	hkpConstraintInstance* cons = new hkpConstraintInstance(rb0,rb1,cc);
	m_world->addConstraint(cons);

	cons->removeReference();
	cc->removeReference();

	m_world->unlock();
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demonstrates how to build a basic hinge constraint using atoms";

HK_DECLARE_DEMO(CustomAtomConstraintDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, "Custom Atom constraint", helpString);

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
