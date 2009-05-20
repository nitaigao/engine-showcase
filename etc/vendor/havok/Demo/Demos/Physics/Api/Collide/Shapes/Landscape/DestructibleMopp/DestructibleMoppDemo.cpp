/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/Shapes/Landscape/DestructibleMopp/DestructibleMoppDemo.h>


#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>

#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>
#include <Physics/Collide/Util/hkpTriangleUtil.h>

#include <Physics/Dynamics/Entity/hkpEntityListener.h>

#include <Physics/Collide/Shape/Compound/Tree/Mopp/Modifiers/hkpRemoveTerminalsMoppModifier.h>

#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>

// We need to display some collision based info
#include <Common/Visualize/hkDebugDisplay.h>
#include <Physics/Utilities/VisualDebugger/Viewer/hkpShapeDisplayBuilder.h>
#include <Graphics/Common/Window/hkgWindow.h>


// there are two ways for removal demonstrated.
//  - remove keys directly from the mopp-code.
//  - update collision info of leaf shapes to a 'destroyed' value. A preset group filter disables then 
//    collisions with objects having that collision info.

enum RemovalType
{
	REMOVE_KEYS_FROM_MOPP,
	UPDATE_COLLISION_FILTER_INFO
};

struct DestructibleMoppVariant
{
	const char*	m_name;
	RemovalType m_removalType;
	const char* m_details;
};

static const char helpString1[] = "Remove keys from MOPP.";
static const char helpString2[] = "Update collision info of the leaf shapes.";

static const DestructibleMoppVariant g_variants[] =
{
	{ helpString1, REMOVE_KEYS_FROM_MOPP, helpString1 },
	{ helpString2, UPDATE_COLLISION_FILTER_INFO, helpString2 },
};

	// Collision info used by all not damaged triangles; the initial value.
#define SOLID_TRIANGLE_FILTER_INFO 2
	// Collision info used by all destroyed triangles. The filter is setup do disable collisino with this 'group' at start.
#define DESTROYED_TRIANGLE_FILTER_INFO 3

	// We only store two materials in the mesh -- solid (or not destroyed) and destroyed materials. 
	// When a triangle is destroyed its material index is simply updated.
#define DESTROYED_TRIANGLE_MATERIAL_INDEX 0 
#define SOLID_TRIANGLE_MATERIAL_INDEX 1 

// a collision listener that serves to destroy part of the mopp on impact with another object
class DestroyAllMopps : public hkReferencedObject, public hkpCollisionListener, public hkpEntityListener
{
   	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		/*
		 * Construction and Destruction
		 */

		DestroyAllMopps( hkpRigidBody* rigidBody, hkpWorld* world, hkpShape* ignoreShape, hkgDisplayHandler* displayHandler, RemovalType removalType );
        
		~DestroyAllMopps();


		/*
		 * Members from base class hkpCollisionListener which must be implemented:
		 */

		// Called after a contact point was added 
		void contactPointAddedCallback(	hkpContactPointAddedEvent& event );

		// The hkpCollisionListener interface implementation
		void contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event);

		// Called before a contact point gets removed. We do not implement this for this demo.
		void contactPointRemovedCallback( hkpContactPointRemovedEvent& event );

		// Called just before the collisionResult is passed to the constraint system (solved).
		void contactProcessCallback( hkpContactProcessEvent& event );

		/*
		 * Members from base class hkpEntityListener which must be implemented:
		 */


		// Called when the entity is deleted. Important to use this event to remove ourselves as a listener.
		void entityDeletedCallback( hkpEntity* entity );

		// Called when the entity is removed from the hkpWorld
		void entityRemovedCallback( hkpEntity* entity );

		hkpWorld* m_world;
		hkpShape* m_ignoreShape;
		hkgDisplayHandler* m_displayHandler;
		RemovalType m_removalType;
};


// create and add as a listener to the mopp rigidbody
DestroyAllMopps::DestroyAllMopps( hkpRigidBody* rigidBody, hkpWorld* world, hkpShape* ignoreShape,
								  hkgDisplayHandler* displayHandler, RemovalType removalType )
:	m_removalType( removalType )
{
	rigidBody->addCollisionListener( this );
	rigidBody->addEntityListener( this );
	m_world = world;
	m_ignoreShape = ignoreShape;
	m_ignoreShape->addReference();
	m_displayHandler = displayHandler;
}


DestroyAllMopps::~DestroyAllMopps()
{
	m_ignoreShape->removeReference();
}

static void setChildShapesMaterialIndex( hkpExtendedMeshShape* meshShape, hkpShapeKey key, int newMaterialIndex )
{
	const hkUint32 triangleIndex = key & ( ~0U >> meshShape->m_numBitsForSubpartIndex );
	const hkUint32 subPartIndex = key >> ( 32 - meshShape->m_numBitsForSubpartIndex );
	// Grab a handle to the sub-part
	hkpExtendedMeshShape::TrianglesSubpart& part = meshShape->getTrianglesSubpartAt(subPartIndex);

	if (part.m_materialIndexBase)
	{
		HK_ASSERT2(0x26d359f1, newMaterialIndex < part.m_numMaterials, "The material index is out of range." );
		HK_ASSERT2(0xad453fa2, part.m_materialIndexStridingType == hkpExtendedMeshShape::MATERIAL_INDICES_INT8 || part.m_materialIndexStridingType == hkpExtendedMeshShape::MATERIAL_INDICES_INT16, "Invalid hkpMeshShape::SubPart::m_materialIndexStridingType.");
		if (part.m_materialIndexStridingType == hkpExtendedMeshShape::MATERIAL_INDICES_INT8)
		{	
			hkUint8* materialId = static_cast<hkUint8 *>(hkAddByteOffset( const_cast<void*>(part.m_materialIndexBase), triangleIndex * part.m_materialIndexStriding ));
			HK_ASSERT2(0xad6788d5, hkUint8(newMaterialIndex) == newMaterialIndex, "newMaterialIndex parameter value is out of hkUint8 range");
			*materialId = hkUint8(newMaterialIndex);
		}
		else
		{	
			hkUint16* materialId = static_cast<hkUint16*>(hkAddByteOffset( const_cast<void*>(part.m_materialIndexBase), triangleIndex * part.m_materialIndexStriding ));
			HK_ASSERT2(0xad6788d6, hkUint16(newMaterialIndex) == newMaterialIndex, "newMaterialIndex parameter value is out of hkUint16 range");
			*materialId = hkUint16(newMaterialIndex);
		}
	}
	else
	{
		HK_ASSERT2(0xad789d9d, 0, "This hkpExtendedMeshShape does not have per-triangle material information");
	}
}


// remove a piece of the mopp if the preconditions are satisfied
void DestroyAllMopps::contactPointAddedCallback( hkpContactPointAddedEvent& event )
{
	hkpShapeKey keyToRemove = event.m_bodyA->getShapeKey();

	// which one is the mopp?  the one that gives us a shape key will be.
	const hkpCdBody& fixedCdBody  = ( keyToRemove != HK_INVALID_SHAPE_KEY ) ? *event.m_bodyA : *event.m_bodyB;
	const hkpCdBody& movingCdBody = ( keyToRemove == HK_INVALID_SHAPE_KEY ) ? *event.m_bodyA : *event.m_bodyB;

	const hkpMoppBvTreeShape* moppShape = static_cast< const hkpMoppBvTreeShape* >( fixedCdBody.getParent()->getShape() );
	hkpEntity*                moppEntity = static_cast<hkpEntity*>( fixedCdBody.getRootCollidable()->getOwner());
	const hkpTriangleShape*   triShape  = static_cast<const hkpTriangleShape* >(fixedCdBody.getShape());
	keyToRemove = fixedCdBody.getShapeKey();
	
	// ignore the small box, who is there to test the aabb cache disabling
	if( movingCdBody.getShape() == m_ignoreShape )
	{
		return;
	}
	hkTransform T = fixedCdBody.getTransform();
	T.getTranslation()(1) += 0.01f;
	
	// lets only remove every other triangle, so only those with their normals pointing up.
	hkVector4 n;
	hkpTriangleUtil::calcNormal( n, triShape->getVertex(0), triShape->getVertex(1), triShape->getVertex(2) );
	if( n(1) > 0 )
	{
		// black out the triangle.  Graphics lib makes it very difficult to remove a triangle, so lets just colour it removed.
		// In this case we simply create a new rigid body, add it to the scene and set its color to black.

		hkpShapeDisplayBuilder::addObjectToDebugDisplay( triShape, T, keyToRemove );
		HK_SET_OBJECT_COLOR((hkUlong)keyToRemove, hkColor::rgbFromChars(0, 0, 0));

		// make sure that the new triangle isn't pickable, otherwise it'll crash
		// we need to run through the whole list of mappings, in case there are more than one display object per key
		{
			hkgDisplayObject* newTri = m_displayHandler->findDisplayObject( keyToRemove );
			HKG_DISPLAY_OBJECT_STATUS status = newTri->getStatusFlags();
			status |= HKG_DISPLAY_OBJECT_NOT_PICKABLE;
			newTri->setStatusFlags(status);
		}

		// remove the shape from the mopp containing it.
		// there are two techniques demonstrated here.  
		switch ( m_removalType )
		{
		case REMOVE_KEYS_FROM_MOPP:
			{
				// the simpler and more efficient method of removing a shape is to remove it's key directly from the mopp code.
				hkInplaceArray<hkpShapeKey, 1> toRemove; 
				toRemove.pushBackUnchecked( keyToRemove );
				hkpRemoveTerminalsMoppModifier modifier( moppShape->getMoppCode(), moppShape->getShapeCollection(), toRemove );
				modifier.applyRemoveTerminals( const_cast<hkpMoppCode*>(moppShape->getMoppCode()) );
				break;
			}
		case UPDATE_COLLISION_FILTER_INFO:
			{
				hkpExtendedMeshShape* meshShape = const_cast<hkpExtendedMeshShape*>(static_cast<const hkpExtendedMeshShape*>(moppShape->getShapeCollection()));
				setChildShapesMaterialIndex(meshShape, keyToRemove, DESTROYED_TRIANGLE_MATERIAL_INDEX);
				break;
			}
		}

		//
		// we need to reset aabb caching otherwise stationary objects ( aabb hasn't changed ) sitting on destroyed bits 
		// will hang on to the now-invalid agent.
		//
		{
			moppEntity->getWorld()->updateCollisionFilterOnEntity( moppEntity,
				HK_UPDATE_FILTER_ON_ENTITY_DISABLE_ENTITY_ENTITY_COLLISIONS_ONLY,
				HK_UPDATE_COLLECTION_FILTER_PROCESS_SHAPE_COLLECTIONS );
		}

		//
		//	We need to wake up all objects touching this removed element
		//
		{
			hkAabb aabb;
			triShape->getAabb( fixedCdBody.getTransform(), 0.0f, aabb );
			moppEntity->getWorld()->activateRegion( aabb );
		}
	}
}

void DestroyAllMopps::contactProcessCallback( hkpContactProcessEvent& event )
{
	// Do nothing
}

void DestroyAllMopps::contactPointRemovedCallback( hkpContactPointRemovedEvent& event )
{
	// Do nothing
}

void DestroyAllMopps::contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event)
{

}


void DestroyAllMopps::entityDeletedCallback( hkpEntity* entity )
{
	// Remove the collision event listener
	entity->removeCollisionListener( this ); 

	// Now that we're removed from our entity, we can remove ourselves, too.
	delete this;
}


void DestroyAllMopps::entityRemovedCallback( hkpEntity* entity )
{
	// Do nothing (we'll remove the collision event listener only when the entity is deleted)
}



DestructibleMoppDemo::DestructibleMoppDemo(hkDemoEnvironment* env)
	:	hkDefaultPhysicsDemo(env)
{
	const DestructibleMoppVariant& variant =  g_variants[m_variantId];

	// Disable face culling
	setGraphicsState(HKG_ENABLED_CULLFACE, false);

	//
	// Setup the camera
	//
	{
		hkVector4 from(10.0f, 10.0f, -30.0f);
		hkVector4 to  ( 0.0f,  0.0f,   0.0f);
		hkVector4 up  ( 0.0f,  1.0f,   0.0f);
		setupDefaultCameras( env, from, to, up );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.m_gravity.set(0.0f, -9.5f, 0.0f);
		info.setBroadPhaseWorldSize(150.0f);
		info.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	//
	// set up collision filter so nothing collides with the
	// blacked out triangles we create for graphical effect.
	//
	{
		hkpGroupFilter* filter = new hkpGroupFilter();
		filter->enableCollisionsUsingBitfield( 0xffffffe, 0xffffffe );
		m_world->setCollisionFilter( filter );
		filter->disableCollisionsBetween( DESTROYED_TRIANGLE_FILTER_INFO, 1 );
		filter->removeReference();
	}

	//
	// Create cube piles
	//
	{
		hkVector4 boxRadii( 0.5f, 0.5f, 0.5f);
		hkpBoxShape* cubeShape = new hkpBoxShape(boxRadii, 0 );

		hkpRigidBodyCinfo boxInfo;

		boxInfo.m_mass = 1.0f;
		boxInfo.m_shape = cubeShape;
		hkpInertiaTensorComputer::setShapeVolumeMassProperties(boxInfo.m_shape, boxInfo.m_mass, boxInfo);
		boxInfo.m_rotation.setIdentity();
		boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;

		for (int i = 0; i < 10; i++ )
		{
			//
			// create a rigid body (using the template above) and add to the m_world
			//
			boxInfo.m_position.set(0, i * 1.4f, -2.5f);
			boxInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(1);

			hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
			m_world->addEntity( boxRigidBody );
			boxRigidBody->removeReference();
		}
		cubeShape->removeReference();
	}

	hkpShape* smallBox = HK_NULL;

	// create a box sitting on a bit that will be destroyed
	{
		hkVector4 boxRadii( 0.25f, 0.25f, 0.25f);
		smallBox = new hkpBoxShape(boxRadii, 0 );

		hkpRigidBodyCinfo boxInfo;

		boxInfo.m_mass = 1.0f;
		boxInfo.m_shape = smallBox;
		hkpInertiaTensorComputer::setShapeVolumeMassProperties(boxInfo.m_shape, boxInfo.m_mass, boxInfo);
		boxInfo.m_rotation.setIdentity();
		boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		boxInfo.m_position.set(3.0f, -0.1f, -7.5f);
		boxInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(1);

		hkpRigidBody* boxRigidBody = new hkpRigidBody(boxInfo);
		m_world->addEntity( boxRigidBody );
		boxRigidBody->removeReference();
	}

	//
	//  create the ground mopp
	//
	{
		hkpMoppBvTreeShape*	moppShape = createMoppShape();

		hkpRigidBodyCinfo boxInfo;
		boxInfo.m_shape = moppShape;

		boxInfo.m_position.set(0.0f,-1.6f,0.0f);
		boxInfo.m_motionType = hkpMotion::MOTION_FIXED;
		boxInfo.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(1);

		hkpRigidBody* baseRigidBody = new hkpRigidBody(boxInfo);

		m_world->addEntity( baseRigidBody );

		baseRigidBody->removeReference();
		moppShape->removeReference();

		new DestroyAllMopps( baseRigidBody, m_world, smallBox, m_env->m_displayHandler, variant.m_removalType );
		smallBox->removeReference();
	}

	m_world->unlock();
}

DestructibleMoppDemo::~DestructibleMoppDemo()
{
	m_world->lock();

	// remove our black triangles
	// (twice ensures that duplicated ids are deleted)
	for (int i = 0; i < 20; i++)
	{
		HK_REMOVE_GEOMETRY( i );
		HK_REMOVE_GEOMETRY( i );
	}

	m_world->unlock();

	m_world->markForWrite();
	m_world->removeReference();
	m_world = HK_NULL;
}



	/// create a mopp shape:\n
	/// The idea of this particular instance is to use a strip:\n
	///	\code
	///		v0----v1
	///			/
	///		v2----v3
	///			/
	///		v4----v5
	///	and so on.
	///	\code
	/// So we just have to create an alternating vertexArray (triples of float)
	/// and a triangle index array. As we are using strips, the index array can
	/// actually use overlapping triangles: Array = [ 0,1,2,3,4,5...]. So the first triangle
	/// get's the indices 0,1,2 the next triangle the indices 1,2,3 and so on.\n\n
	/// This example also implements a callback, which assigns a per triangle friction

hkpMoppBvTreeShape* DestructibleMoppDemo::createMoppShape()
{

	{
		for (short i = 0; i < NUM_VERTICES; i++ )
		{
			m_indices[i] = i;
			m_vertices[i*3 + 0] = (i&1) * 16.0f - 8.0f;
			m_vertices[i*3 + 1] = (i&3) * 0.3f;
			m_vertices[i*3 + 2] = i * 2.0f - NUM_VERTICES;
		}
	}
	{
		m_materials[DESTROYED_TRIANGLE_MATERIAL_INDEX].m_filterInfo = hkpGroupFilter::calcFilterInfo( DESTROYED_TRIANGLE_FILTER_INFO );
		m_materials[SOLID_TRIANGLE_MATERIAL_INDEX].m_filterInfo = hkpGroupFilter::calcFilterInfo( SOLID_TRIANGLE_FILTER_INFO );

		for (short i = 0; i < NUM_TRIANGLES; i++ )
		{
			m_materialIndizes[i] = hkUchar(SOLID_TRIANGLE_MATERIAL_INDEX);
		}
	}


	hkpExtendedMeshShape* mesh = new hkpExtendedMeshShape();
	mesh->setRadius( 0.05f );

	{
		hkpExtendedMeshShape::TrianglesSubpart part;

		part.m_vertexBase = m_vertices;
		part.m_vertexStriding = sizeof(float) * 3;
		part.m_numVertices = NUM_VERTICES;

		part.m_indexBase = m_indices;
		part.m_indexStriding = sizeof( hkUint16);
		part.m_numTriangleShapes = NUM_TRIANGLES;
		part.m_stridingType = hkpExtendedMeshShape::INDICES_INT16;

		part.m_materialIndexStridingType = hkpExtendedMeshShape::MATERIAL_INDICES_INT8;
		part.m_materialIndexBase = m_materialIndizes;
		part.m_materialIndexStriding = sizeof( hkUchar );

		part.m_materialBase = m_materials;
		part.m_materialStriding = sizeof( hkpMeshMaterial );
		part.m_numMaterials = sizeof( m_materials ) / sizeof( hkpMeshMaterial );

		mesh->addTrianglesSubpart( part );
	}

	hkpMoppCompilerInput mci;
	hkpMoppCode* code = hkpMoppUtility::buildCode( mesh ,mci);

	// there are two techniques demonstrated here for removing shapes.
	hkpMoppBvTreeShape* moppShape;
	moppShape = new hkpMoppBvTreeShape( mesh, code );

	mesh->removeReference();
	code->removeReference();

	return moppShape;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] =						\
	"Dynamic removal of children from a MOPP. Half the triangles are removed on "	\
	"collision. Removed triangles are colored black.";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( DestructibleMoppDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, DestructibleMoppVariant, g_variants, helpString);

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
