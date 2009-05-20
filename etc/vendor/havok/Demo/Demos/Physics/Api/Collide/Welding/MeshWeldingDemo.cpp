/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/Welding/MeshWeldingDemo.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>

#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Physics/Collide/Shape/Compound/Collection/SimpleMesh/hkpSimpleMeshShape.h>
#include <Physics/Dynamics/Action/hkpUnaryAction.h>
#include <Common/Visualize/hkDebugDisplay.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Physics/Utilities/Serialize/hkpHavokSnapshot.h>

#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Common/Base/Container/LocalArray/hkLocalArray.h>
#include <Physics/Collide/Util/Welding/hkpMeshWeldingUtility.h>


//
// Declare some local classes.
//

class PushAction : public hkpUnaryAction
{
	public:

		PushAction(hkpRigidBody* body) : hkpUnaryAction(body) {}

		hkpAction* clone(const hkArray<hkpEntity*>&, const hkArray<hkpPhantom*>&) const
		{
			HK_ASSERT(0,0);
			return HK_NULL;
		}

		void applyAction(const hkStepInfo& stepInfo)
		{

			hkpRigidBody* body = static_cast<hkpRigidBody*>(m_entity);

			// Apply a push in a circle around y-axis.
			hkVector4 force;
			force.setCross(hkVector4(0,1,0), body->getPosition());
			force.mul4(10.f);

			body->applyForce(stepInfo.m_deltaTime, force);
		}
};

enum WeldingVariantType
{
	NONE,
	OLD_STYLE,
	TWO_SIDED,
	TWO_SIDED_NO_TOIS,
	ONE_SIDED
};

enum SimulationVariantType
{
	DISCRETE,
	CONTINUOUS
};

struct WeldingVariant
{
	const char*	 m_name;
	WeldingVariantType m_weldingType;
	SimulationVariantType m_simulationType;
	const char* m_details;
};

static const WeldingVariant g_variants[] =
{
	{ "No welding - Discrete", NONE,  DISCRETE, "No welding - Discrete" },
	{ "No welding - Continuous", NONE,  CONTINUOUS, "No welding - Continuous" },
	{ "Deprecated Welding - Discrete", OLD_STYLE,  DISCRETE, "Deprecated Welding - Discrete" },
	{ "Deprecated Welding - Continuous", OLD_STYLE,  CONTINUOUS, "Deprecated Welding - Continuous" },
	{ "Two sided Welding - Discrete", TWO_SIDED,  DISCRETE, "Two sided Welding - Discrete" },
	{ "Two sided Welding - Continuous", TWO_SIDED,  CONTINUOUS, "Two sided Welding - Continuous" },
	{ "Two sided Welding - Continuous, Tois not welded", TWO_SIDED_NO_TOIS,  CONTINUOUS, "Two sided Welding - Continuous, Tois not welded" },
	{ "One sided Welding - Discrete", ONE_SIDED,  DISCRETE, "One sided Welding - Discrete" },
	{ "One sided Welding - Continuous", ONE_SIDED,  CONTINUOUS, "One sided Welding - Continuous" },
};

MeshWeldingDemo::MeshWeldingDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	const WeldingVariant& variant =  g_variants[m_variantId];

	//
	// Create the world.
	//
	{

		hkpWorldCinfo info;
		info.m_toiCollisionResponseRotateNormal = 0;
		info.m_gravity.setZero4();

		if ( variant.m_weldingType == TWO_SIDED )
		{
			info.m_enableToiWeldRejection = true;
		}

		m_world = new hkpWorld( info );
		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());
	}

	//
	// Setup graphics
	//

	{
		// Disable culling
		setGraphicsState(HKG_ENABLED_CULLFACE, false);

		// Enable wire frame display mode
		setGraphicsState(HKG_ENABLED_WIREFRAME, true);

		//
		// Setup default camera
		//
		{

			hkVector4 from(0.0f, 30.0f, 10.0f);
			hkVector4 to(0,0,0);
			hkVector4 up(0.0f, 1.0f, 0.0f);

			setupDefaultCameras( env, from, to, up, 0.1f, 500.0f);
		}
		
		setupGraphics();
	}

	//
	// Create self-propelled, moving block.
	//
	{
		hkVector4 posBlock(-7.8f, 1.0f, 0.0f);
		float mass = 1;
		float friction = 0.f;
		float restitution = 0;
		m_movingBody = createDynamicBox(posBlock, mass, friction, restitution, m_world);

		hkpCollidableQualityType qt = (variant.m_simulationType == CONTINUOUS) ? HK_COLLIDABLE_QUALITY_CRITICAL : HK_COLLIDABLE_QUALITY_DEBRIS;
		m_movingBody->setQualityType( qt );

		m_world->addEntity( m_movingBody );
		{
			PushAction* pushAction = new PushAction( m_movingBody );
			m_world->addAction(pushAction);
			pushAction->removeReference();
		}
	}

	//
	// Load simple mesh from xml file, create welding information, add welding utility and
	// insert it into world.
	//
	{
		hkVector4 posSimpleMesh(0, -1, 0);
		loadAndInitSimpleMesh(posSimpleMesh, m_world, m_ringData);
	}



	m_world->unlock();

	m_bodySpeed = 0;
}



MeshWeldingDemo::~MeshWeldingDemo( )
{
	m_world->markForWrite();
	m_world->removeReference();
	m_world = HK_NULL;

	// release the memory used by the xml reader
	m_ringData->removeReference();
	m_movingBody->removeReference();


}

hkDemo::Result MeshWeldingDemo::stepDemo()
{
	hkDefaultPhysicsDemo::stepDemo();

	m_world->lock();

	// Display the speed of the body, and stop updating the value when it leaves the mesh
	if (m_movingBody->getNumConstraints() == 1)
		m_bodySpeed = m_movingBody->getLinearVelocity().length3();
	if (m_movingBody->isFixed())
		m_bodySpeed = 0;

	m_world->unlock();

	char buf[100];
	hkString::sprintf( buf, "Rigid body speed = %f m/s", m_bodySpeed );
	m_env->m_textDisplay->outputText(buf, 20, 60 );

	return DEMO_OK;
}



hkpRigidBody* MeshWeldingDemo::createDynamicBox(const hkVector4& position, float mass, float friction, float restitution, hkpWorld* world) const
{
	hkpBoxShape* boxShape;
	{
		hkVector4 boxHalfExtent; boxHalfExtent.setAll(1);
		float convexRadius = 0;
		boxShape = new hkpBoxShape(boxHalfExtent, convexRadius);
	}
	hkpListShape* list = new hkpListShape( (const hkpShape*const*)&boxShape, 1 );

	hkpRigidBodyCinfo boxInfo;
	{
		boxInfo.m_shape = list;
		boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		hkpInertiaTensorComputer::setShapeVolumeMassProperties(boxInfo.m_shape, boxInfo.m_mass, boxInfo);
		boxInfo.m_qualityType = HK_COLLIDABLE_QUALITY_CRITICAL;
		boxInfo.m_linearDamping = 0.0f;
		boxInfo.m_position = position;
		boxInfo.m_mass = mass;
		boxInfo.m_friction = friction;
		boxInfo.m_restitution = restitution;
	}

	hkpRigidBody* box = new hkpRigidBody(boxInfo);

	boxShape->removeReference();
	list->removeReference();

	return box;
}


hkpRigidBody* MeshWeldingDemo::loadAndInitSimpleMesh(const hkVector4& position, hkpWorld* world, hkPackfileReader::AllocatedData*& allocatedData)
{
	hkpRigidBody* mesh = loadRigidBodyFromXmlFile("Resources/Physics/Landscapes/weldtestmesh.xml" , allocatedData);

	mesh->setPosition(position);

	//HK_SET_OBJECT_COLOR((hkUlong)mesh->getCollidable(), hkColor::rgbFromChars(255, 255, 255, 128)); // RGBA

	HK_ASSERT(0xaf639541, mesh->getCollidable()->getShape()->getType() == HK_SHAPE_MOPP);
	const hkpMoppBvTreeShape* moppShape = static_cast<const hkpMoppBvTreeShape*>(mesh->getCollidable()->getShape());

	hkpSimpleMeshShape* meshShape = (hkpSimpleMeshShape*)moppShape->getShapeCollection();

	HK_ASSERT(0xaf639541, moppShape->getShapeCollection()->getType() == HK_SHAPE_TRIANGLE_COLLECTION);


	// Replace the mesh shape with an extendedMeshShape
	//
	hkpExtendedMeshShape* extShape = new hkpExtendedMeshShape();

	hkpExtendedMeshShape::TrianglesSubpart part;
	part.m_numTriangleShapes = meshShape->m_triangles.getSize();
	part.m_vertexBase = &meshShape->m_vertices[0](0);
	part.m_vertexStriding = sizeof(hkVector4);
	part.m_numVertices = meshShape->m_vertices.getSize();
	part.m_extrusion.setZero4(); 
	part.m_indexBase = meshShape->m_triangles.begin();
	part.m_indexStriding = sizeof(meshShape->m_triangles[0]);
	part.m_stridingType = hkpExtendedMeshShape::INDICES_INT32;

	extShape->addTrianglesSubpart(part);
	
	hkpMoppCompilerInput mci;
	mci.m_enableChunkSubdivision = true;
	hkpMoppCode* newCode = hkpMoppUtility::buildCode( extShape,mci);
	hkpMoppBvTreeShape* newMopp = new hkpMoppBvTreeShape( extShape, newCode );

	mesh->setShape(newMopp);

	extShape->removeReference();
	newCode->removeReference();
	newMopp->removeReference();

	//
	// Setup welding for this object.
	//


	{
		extShape->setRadius(0);

		const WeldingVariant& variant =  g_variants[m_variantId];
		switch (variant.m_weldingType )
		{
			case ONE_SIDED:
			{
				hkpMeshWeldingUtility::ShapeInfo info;
				info.m_transform = hkTransform::getIdentity();
				info.m_shape = newMopp;

				hkLocalArray< hkpMeshWeldingUtility::ShapeInfo > shapes( 1 );
				shapes.pushBack( info );

				extShape->m_weldingType = hkpWeldingUtility::WELDING_TYPE_CLOCKWISE;
				hkpMeshWeldingUtility::computeWeldingInfo( hkTransform::getIdentity(), extShape, shapes, true, hkpMeshWeldingUtility::WINDING_IGNORE_CONSISTENCY );

				break;
			}
			case TWO_SIDED:
			case TWO_SIDED_NO_TOIS:
			{
				extShape->computeWeldingInfo( newMopp, hkpWeldingUtility::WELDING_TYPE_TWO_SIDED );
				break;
			}
			case NONE:
			{
				extShape->m_disableWelding = true;
				break;
			}
			case OLD_STYLE:
			{
				break;
			}
		}
	}

	world->addEntity(mesh);

	return mesh;
}



//
// This method loads a rigid body from an xml file. The allocated memory used for loading has to be
// kept using a reference. Note that this method expects the xml file to contain exactly one physics
// system and at least one rigid body. Although it will only return the first rigid body (as we don't
// need more for this demo), this could easily be extended to handle more.
//
hkpRigidBody* MeshWeldingDemo::loadRigidBodyFromXmlFile(const hkString& path, hkPackfileReader::AllocatedData*& allocatedData) const
{
	hkIstream infile(path.cString());
	HK_ASSERT(0xaf639672, infile.isOk());

	hkpPhysicsData* physicsData = hkpHavokSnapshot::load(infile.getStreamReader(), &allocatedData);
	hkArray<hkpPhysicsSystem*> physicsSystems;
	physicsSystems = physicsData->getPhysicsSystems();
	HK_ASSERT(0xaf836275, physicsSystems.getSize() == 1);

	hkArray<hkpRigidBody*> rigidBodies;
	rigidBodies = physicsSystems[0]->getRigidBodies();
	HK_ASSERT(0xaf836271, rigidBodies.getSize() >= 1);

	return rigidBodies[0];
}


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


static const char helpString[] = \
"A stress test for welding triangle edge collisions.  The variants show the various algorithms and their results.";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( MeshWeldingDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, WeldingVariant, g_variants, helpString ); 

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
