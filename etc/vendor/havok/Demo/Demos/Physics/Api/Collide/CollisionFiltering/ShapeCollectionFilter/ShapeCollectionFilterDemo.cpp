/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>



// Need some shapes
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>

#include <Physics/Collide/Shape/Misc/Transform/hkpTransformShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>

#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhase.h>
#include <Physics/Collide/Filter/Group/hkpGroupFilterSetup.h>

// Need to get broadPhase from world
#include <Demos/Physics/Api/Collide/CollisionFiltering/ShapeCollectionFilter/ShapeCollectionFilterDemo.h>

// We need to display the results of the raycast to confirm correctness.
#include <Common/Visualize/hkDebugDisplay.h>
#include <Graphics/Common/Window/hkgWindow.h>


ShapeCollectionFilterDemo::ShapeCollectionFilterDemo(hkDemoEnvironment* env)
	:	hkDefaultPhysicsDemo(env), 
		m_time(0.0f)
{

	//	
	// Setup the camera.
	//
	{
		hkVector4 from(-10.0f, 30.0f, 10.0f);
		hkVector4 to  (5.0f, 0.0f, 0.0f);
		hkVector4 up  (0.0f, 1.0f, 0.0f);
		setupDefaultCameras(env, from, to, up);
	}



	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 100.0f );
		info.m_gravity.setZero4();
		m_world = new hkpWorld(info);
		m_world->lock();

		setupGraphics();

		// the world uses reverse triangles (a strip that isn't actually stripped)
		// so we must make sure the graphics culling is off
		setGraphicsState(HKG_ENABLED_CULLFACE, false);
	}

	//
	// Register all agents
	//
	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

	//
	//	Create the filter: every layer collides only with itself except layer 0, which collides with everything
	//
	{
			hkpGroupFilter* filter = new hkpGroupFilter();
			filter->disableCollisionsUsingBitfield( 0xfffffffe, 0xfffffffe );
			for (int i = 0; i < 32; i++ )
			{
				filter->enableCollisionsBetween( i, i );
			}
			m_world->setCollisionFilter( filter );
			filter->removeReference();
	}


	//
	//	Create a grid of 4x4 boxes, merged into one single list shape
	//
	{
		hkVector4 halfExtents(.5f, .5f, .5f );
		hkpConvexShape* boxShape = new hkpBoxShape( halfExtents , 0 );

		hkpShape* shapes[16];
		{
			for ( int j = 0 ; j < 4; j++)
			{
				for (int i = 0; i< 4; i++ )
				{
					hkVector4 t(i*3.0f, 0.0f, j*2.0f+2.0f);
					shapes[4*j + i] = new hkpConvexTranslateShape( boxShape, t );
				}
			}
		}

		hkpListShape* listShape = new hkpListShape(shapes, 16);
		{
			for (int i = 0; i < 16; i++)
			{
				shapes[i]->removeReference();
				shapes[i] = HK_NULL;
			}
		}

		//
		//	Set the collision filter info
		//
		{
			for ( int j = 0 ; j < 4; j++)
			{
				for ( int i = 0; i < 4; i++)
				{
					listShape->setCollisionFilterInfo(4*j + i, hkpGroupFilter::calcFilterInfo(i+1) );
				}
			}
		}

		//
		// create the rigid body
		//
		{
			hkpRigidBodyCinfo ci;
			ci.m_motionType = hkpMotion::MOTION_FIXED;
			ci.m_shape = listShape;

			hkpRigidBody* body = new hkpRigidBody( ci );
			m_world->addEntity(body);
			body->removeReference();
		}

		listShape->removeReference();
		boxShape->removeReference();
	}

	//
	//	Create a mopp shape, which looks like __--__--__--__--
	//
	hkpShape* moppShape;
	{
		//
		//	Set our vertices and indices
		//  Note: we reuse the index triples in an overlapping way to
		//  get a triangle strip
		//
		{
			for (short i = 0; i < NUM_VERTICES; i++ )
			{
				m_indices[i] = i;
				m_vertices[i].x = ( (i+2)>>2 )  * 1.5f - 2.0f;
				m_vertices[i].y = ( (i>>2) & 1) * 1.0f - 0.5f;
				m_vertices[i].z = ( (i>>0) & 1) * (-9.5f);
				m_vertices[i].w = 0.f;
			}
		}
		//
		//	Set our materialIndices per triangle
		//
		{
			for (short i = 0; i < NUM_TRIANGLES; i++ )
			{
				m_materialIndices[i] = char(i>>3);
			}
		}

		//
		// Our material lib
		//
		{
			for ( int i = 0; i < NUM_MATERIALS; i++)
			{
				m_materials[i].m_filterInfo = hkpGroupFilter::calcFilterInfo(i+1);
			}
		}

		hkpExtendedMeshShape* mesh = new hkpExtendedMeshShape();
		mesh->setRadius(0.05f);
		{
			hkpExtendedMeshShape::TrianglesSubpart part;

			part.m_vertexBase = &m_vertices[0].x;
			part.m_vertexStriding = sizeof(MyVector4);
			part.m_numVertices = NUM_VERTICES;

			part.m_indexBase = m_indices;
			part.m_indexStriding = sizeof( hkUint16);
			part.m_numTriangleShapes = NUM_TRIANGLES;
			part.m_stridingType = hkpExtendedMeshShape::INDICES_INT16;

			part.m_materialBase = m_materials;
			part.m_materialStriding = hkSizeOf( hkpMeshMaterial );
			part.m_numMaterials = 4;

			part.m_materialIndexBase = m_materialIndices;
			part.m_materialIndexStriding = sizeof( m_materialIndices[0] );

			mesh->addTrianglesSubpart( part );
		}

		hkpMoppCompilerInput mci;
		mci.m_enableChunkSubdivision = true;
		hkpMoppCode* code = hkpMoppUtility::buildCode( mesh ,mci);

		hkpMoppBvTreeShape* mopp = new hkpMoppBvTreeShape( mesh, code );

		moppShape = mopp;

		mesh->removeReference();
		code->removeReference();
	}

	//
	//	Create the landscape body
	//
	{
		hkpRigidBodyCinfo ci;
		ci.m_motionType = hkpMotion::MOTION_FIXED;
		ci.m_shape = moppShape;
		hkpRigidBody* body = new hkpRigidBody( ci );
		m_world->addEntity(body);
		body->removeReference();
	}
	moppShape->removeReference();


	//
	//	Create a series of moving rigid bodies.
	//  Each body will only collide with the childShape of the specific color
	//
	{
		hkVector4 halfExtents( .2f, .2f, .2f );
		hkpConvexShape* boxShape = new hkpBoxShape( halfExtents , 0 );

		//
		//
		//
		int colors[4] = { hkColor::RED, hkColor::GREEN, hkColor::BLUE, hkColor::YELLOW };

		hkpRigidBodyCinfo ci;
		ci.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		hkReal d = 10.0f;
		ci.m_inertiaTensor.setDiagonal( d, d, d );
		ci.m_shape = boxShape;
		ci.m_mass = d;

		for (int i = -1; i <= 1 ; i+=2 )
		{
			for (int j = 0; j < 4; j++ )
			{
				//
				//	Setup the start coordinates
				//

				ci.m_position.set(-4.f, 0.f, i*(1+j) * 2.0f);
				ci.m_linearVelocity.set( 5.0f, 0.f, 0.f );
				ci.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo(j+1);

				hkpRigidBody* body = new hkpRigidBody( ci );
				m_world->addEntity(body);
				HK_SET_OBJECT_COLOR((hkUlong)body->getCollidable(), colors[j]);
				
				body->removeReference();
			}
		}

		boxShape->removeReference();
	}

	//
	// create 4 different hkListShapes (consisting of 4x hkConvexTranslate+hkpBoxShape) with a different filter id each
	//
	{
		for ( int j = -1; j <= 2; j+= 2 )
		{
			for (int i=0; i<4; i++)
			{
				hkVector4 position(15.0f, 0.0f, j*(i+1)*2.0f);
				hkVector4 velocity(-5.0f, 0.0f, 0.f );
				createMoveableListShape(position, velocity, i);
			}
		}
	}

	m_world->unlock();
}


hkDemo::Result ShapeCollectionFilterDemo::stepDemo()
{
	return hkDefaultPhysicsDemo::stepDemo();
}


void ShapeCollectionFilterDemo::createMoveableListShape(hkVector4& position, hkVector4& velocity, int colorIdx)
{
	int colors[4] = { hkColor::RED, hkColor::GREEN, hkColor::BLUE, hkColor::YELLOW };

	//
	//	Create an array of 4 boxes, merged into one single list shape
	//
	{
		hkVector4 halfExtents(.2f, .2f, .2f );
		hkpConvexShape* boxShape = new hkpBoxShape( halfExtents , 0 );

		hkpShape* shapes[4];
		{
			for (int i = 0; i< 4; i++ )
			{
				hkVector4 t(i*0.9f, 0.0f, 0.0f);
				shapes[i] = new hkpConvexTranslateShape( boxShape, t );
			}
		}

		hkpListShape* listShape = new hkpListShape(shapes, 4);
		{
			for (int i = 0; i < 4; i++)
			{
				shapes[i]->removeReference();
				shapes[i] = HK_NULL;
			}
		}

		//
		//	Set the collision filter info
		//
		{
			for ( int i = 0; i < 4; i++)
			{
				listShape->setCollisionFilterInfo(i, hkpGroupFilter::calcFilterInfo(colorIdx+1) );
			}
		}

		//
		// create the rigid body
		//
		{
			hkpRigidBodyCinfo ci;
			ci.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
			ci.m_shape = listShape;
			ci.m_position = position;
			ci.m_linearVelocity = velocity;

			hkpRigidBody* body = new hkpRigidBody( ci );
			m_world->addEntity(body);
			HK_SET_OBJECT_COLOR((hkUlong)body->getCollidable(), colors[colorIdx]);
			body->removeReference();
		}

		listShape->removeReference();
		boxShape->removeReference();
	}
}



////////////////////////////////////////////////////////////////////
#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"This demo illustrates the use of shape collection filtering.  " \
"On one side moving boxes are fired at a list shape. " \
"The fixed boxes inside this list shape have specific collision " \
"filter info set so that each collide with a specific box color. " \
"Opposite to these single moving boxes some moving list shapes are also " \
"fired against the same fixed list shape grid. " \
"On the other side, the boxes are fired against a hkpMeshShape, " \
"where they only hit selected triangles. " \
"This selection of triangles is achieved by adding material " \
"information to the mesh, which includes collision filter data. ";

HK_DECLARE_DEMO(ShapeCollectionFilterDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, "Filtering raycasts", helpString);

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
