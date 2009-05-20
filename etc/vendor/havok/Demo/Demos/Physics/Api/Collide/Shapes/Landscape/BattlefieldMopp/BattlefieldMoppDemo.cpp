/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/Shapes/Landscape/BattlefieldMopp/BattlefieldMoppDemo.h>
#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/FlatLand.h>

#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/Modifiers/hkpRemoveTerminalsMoppModifier.h>

#include <Physics/Dynamics/Collide/hkpCollisionListener.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>

#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhase.h>
#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhaseHandlePair.h>

#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/Geometry/VertexSet/hkgVertexSet.h>

#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>

#include <Common/Visualize/hkDebugDisplay.h>

// the minimum/maximum radius of a single bomb (and thus of the produced crater)
#define BOMB_MIN_RADIUS  3.0f
#define BOMB_MAX_RADIUS 13.0f

// the maximum height vertices are allowed to be pushed down by one single bomb
#define BOMB_MAX_CRATER_DEPTH 10.0f

// property that is added to objects to mark them as 'bombs'. Property value can be 1 (live) and 2 (already detonated).
#define BOMB_PROPERTY_KEY 100



// a collision listener used to create impact craters by removing selected triangles, shifting them down and building a
// new object from them
class BattlefieldTerrainModifier : public hkReferencedObject, public hkpCollisionListener, public hkpEntityListener
{
public:

	BattlefieldTerrainModifier(hkDemoEnvironment* env, hkpWorld* world, hkpRigidBody* rigidBody, BattlefieldMoppDemo::Explosions* explosions);
	~BattlefieldTerrainModifier();


protected:

	//
	// hkpCollisionListener member functions
	//

	// Called after a contact point was added 
	void contactPointAddedCallback(hkpContactPointAddedEvent& event);

	// The hkpCollisionListener interface implementation
	void contactPointConfirmedCallback(hkpContactPointConfirmedEvent& event) {}

	// Called before a contact point gets removed. We do not implement this for this demo.
	void contactPointRemovedCallback(hkpContactPointRemovedEvent& event) {}


	//
	// hkpEntityListener member functions
	//

	// Called when the entity is deleted. Important to use this event to remove ourselves as a listener.
	void entityDeletedCallback(hkpEntity* entity);


protected:

	hkDemoEnvironment*					m_env;
	hkpWorld*							m_world;
	BattlefieldMoppDemo::Explosions*	m_explosions;
};



BattlefieldTerrainModifier::BattlefieldTerrainModifier(hkDemoEnvironment* env, hkpWorld* world, hkpRigidBody* rigidBody, BattlefieldMoppDemo::Explosions* explosions)
{
	rigidBody->addCollisionListener(this);
	rigidBody->addEntityListener(this);

	m_env			= env;
	m_world			= world;
	m_explosions	= explosions;

	addReference();
}



BattlefieldTerrainModifier::~BattlefieldTerrainModifier()
{
	
}



void BattlefieldTerrainModifier::contactPointAddedCallback(hkpContactPointAddedEvent& event)
{

	// find the bomb object
	hkpEntity* bomb = HK_NULL;
	{
		hkpEntity* collA = hkGetRigidBody(event.m_bodyA->getRootCollidable());
		hkpEntity* collB = hkGetRigidBody(event.m_bodyB->getRootCollidable());
		if ( collA->hasProperty(BOMB_PROPERTY_KEY) )
		{
			bomb = collA;
		}
		else if ( collB->hasProperty(BOMB_PROPERTY_KEY) )
		{
			bomb = collB;
		}
	}

	// abort if no bomb was found (because of collision between terrain object and a non-bomb object) or if bomb already
	// has detonated
	if ( bomb == HK_NULL || bomb->getProperty(BOMB_PROPERTY_KEY).getInt() != 1 )
	{
		return;
	}

	// reject this contact point
	event.m_status = HK_CONTACT_POINT_REJECT;

	//
	// add bomb and position of detonation to global list
	// (note: doing stuff inside callbacks is VERY tricky; therefore we will move the rather complex actual 'detonation' code outside and
	//  only call it in a sort-of-serialized way once every frame)
	//
	{
		m_explosions->m_criticalSection.enter();

		BattlefieldMoppDemo::Explosions::Data* explosionData;
		{
			explosionData				= &m_explosions->m_explosionsData.expandOne();
			explosionData->m_bomb		= bomb;
			explosionData->m_position	= event.m_contactPoint->getPosition();
		}

		m_explosions->m_criticalSection.leave();
	}
}



void BattlefieldTerrainModifier::entityDeletedCallback(hkpEntity* entity)
{
	// Remove the collision event listener
	entity->removeCollisionListener(this); 

	// Now that we're removed from our entity, we can remove ourselves, too.
	removeReference();
}



/// This demo demonstrates how to modify an existing terrain object on-the-fly.
///
/// Technical note: bombs with a high velocity might seem to penetrate through the terrain. This is a design problem
/// of this demo (and no actual physics problem) because the 'shifted' triangles are removed immediately from simulation
/// while the replacement objects are created inside a collision listener callback (which causes a delay in inserting
/// them into the simulation). Thus a bomb with high velocity might already have moved below the replacement terrain
/// object before the latter is actually simulated.

BattlefieldMoppDemo::BattlefieldMoppDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env), m_randomGenerator(11)
{
	
	// Disable warnings:									
	hkError::getInstance().setEnabled(0x7dd65995, false); //'The system has requested a heap allocation on stack overflow.'

	
	hkpWorld::IgnoreForceMultithreadedSimulation ignoreForceMultithreaded;

	//
	// Setup the camera
	//
	{
		hkVector4 from(80.0f, 50.0f, -80.0f);
		hkVector4 to  ( 0.0f,  0.0f,   0.0f);
		hkVector4 up  ( 0.0f,  1.0f,   0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo worldInfo;
		{
			worldInfo.m_gravity.set(0.0f, -9.81f, 0.0f);
			worldInfo.setBroadPhaseWorldSize(1000.0f);
			worldInfo.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );
		}
		m_world = new hkpWorld(worldInfo);
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	//
	// Create bombs at random coordinates
	//
	{
		hkVector4 bombSize(0.2f, 0.2f, 0.2f);
		hkpBoxShape* bombShape = new hkpBoxShape(bombSize, 0);

		hkpRigidBodyCinfo bombInfo;
		{
			bombInfo.m_mass = 1.0f;
			bombInfo.m_shape = bombShape;
			hkpInertiaTensorComputer::setShapeVolumeMassProperties(bombInfo.m_shape, bombInfo.m_mass, bombInfo);
			bombInfo.m_linearDamping = 1.0f;
			bombInfo.m_rotation.setIdentity();
			bombInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		}

		hkPseudoRandomGenerator random(10);

		//
		// create the bombs
		//
		{
			for (int i = 0; i < 50; i++)
			{
				bombInfo.m_position.set(random.getRandRange(-40.0f, 40.0f), 5.0f + i * 10.0f, random.getRandRange(-40.0f, 40.0f));

				hkpRigidBody* bomb = new hkpRigidBody(bombInfo);
				m_world->addEntity(bomb);
				bomb->removeReference();

				// mark object as 'live bomb'
				bomb->addProperty(BOMB_PROPERTY_KEY, hkpPropertyValue(1));
			}
		}

		bombShape->removeReference();
	}

	//
	// create the ground mopp
	//
	{

		hkpRigidBodyCinfo terrainInfo;

		//
		// create terrain shape using the FlatLand utility class
		//
		{
			FlatLand* flatLand = new FlatLand(100); 
			m_terrain = flatLand;
			// Note: You can scale the y-axis to 0 for a completely plain terrain
			hkVector4 scaling(1.0f, 0.1f, 1.0f);
			flatLand->setScaling(scaling);
			flatLand->enableCachedBuild(true);
			terrainInfo.m_shape = flatLand->createMoppShape();
			terrainInfo.m_position.set(5.0f, -2.0f, 5.0f);
		}

		{
			terrainInfo.m_motionType = hkpMotion::MOTION_FIXED;
			terrainInfo.m_friction = 0.5f;

			hkpRigidBody* terrainBody = new hkpRigidBody(terrainInfo);
			m_world->addEntity(terrainBody);

			terrainBody->removeReference();

			// make our new terrain object destructible
			(new BattlefieldTerrainModifier(m_env, m_world, terrainBody, &this->m_explosions))->removeReference();
		}
		terrainInfo.m_shape->removeReference();
	}

	m_world->unlock();
}



BattlefieldMoppDemo::~BattlefieldMoppDemo()
{
	// Re-enable warnings:									
	hkError::getInstance().setEnabled(0x7dd65995, true); 

	m_terrain->removeReference();
}



hkDemo::Result BattlefieldMoppDemo::stepDemo()
{
	processExplosions();

	return hkDefaultPhysicsDemo::stepDemo();
}



void BattlefieldMoppDemo::processExplosions()
{
	m_world->lock();

	{
		for (int i=0; i<m_explosions.m_explosionsData.getSize(); i++)
		{
			Explosions::Data* explosion = &m_explosions.m_explosionsData[i];
			processSingleExplosion(explosion);
		}
	}
	m_explosions.m_explosionsData.clear();

	m_world->unlock();
}



void BattlefieldMoppDemo::processSingleExplosion(const Explosions::Data* explosion)
{
	hkVector4 contactPosition = explosion->m_position;
	hkpEntity* bomb = explosion->m_bomb;

	HK_ASSERT(0xaf14274d, bomb);

	// abort if bomb already has detonated
	// (this situation arises if one bomb is added several times to the list because of colliding with several
	//  mopps simultaneously and the first entry already took care of the detonation)
	if ( bomb->getProperty(BOMB_PROPERTY_KEY).getInt() != 1 )
	{
		return;
	}

	// mark bomb as 'detonated'
	bomb->editProperty(BOMB_PROPERTY_KEY, hkpPropertyValue(2));

	hkReal bombRadius = m_randomGenerator.getRandRange(BOMB_MIN_RADIUS, BOMB_MAX_RADIUS);

	//
	// collect all objects within bomb's range
	//
	hkArray<hkpBroadPhaseHandlePair> objectsWithinRange;
	hkAabb bombAabb;
	{
		{
			hkVector4 minimum(-bombRadius, -bombRadius, -bombRadius);
			hkVector4 maximum( bombRadius,  bombRadius,  bombRadius);
			minimum.add4(contactPosition);
			maximum.add4(contactPosition);
			bombAabb.m_min = minimum;
			bombAabb.m_max = maximum;
		}
		m_world->getBroadPhase()->querySingleAabb(bombAabb, objectsWithinRange);
	}

	//
	// collect all triangles of all objects within bomb's range and remove them from their
	// respective objects
	//
	hkArray<hkpTriangleShape*> collectedTrianglesWithinRange;
	{
		for (int objIdx = 0; objIdx < objectsWithinRange.getSize(); objIdx++)
		{
			hkpBroadPhaseHandlePair *pair = &objectsWithinRange[objIdx];
			hkpCollidable* collidable = static_cast<hkpCollidable*>(static_cast<hkpTypedBroadPhaseHandle*>(pair->m_b)->getOwner());
			hkpRigidBody* rigidBody = hkGetRigidBody(collidable);

			// do not process the bomb object itself
			if ( !rigidBody || rigidBody == bomb )
			{
				continue;
			}

			//	We need to wake up all objects touching this removed element
			rigidBody->activate();

			hkpBvTreeShape* moppShape;
			{
				const hkpShape* shape = rigidBody->getCollidable()->getShape();
				if ( shape->getType() != HK_SHAPE_MOPP )
				{
					continue;
				}
				moppShape = const_cast<hkpBvTreeShape*>(static_cast<const hkpBvTreeShape*>(shape));
			}

			// collect the keys for and remove all triangles that are within the bomb's range
			this->removeTrianglesWithinSphere(*rigidBody, *moppShape, contactPosition, bombRadius, collectedTrianglesWithinRange);

		}
	}

	//
	// build new terrain mopp from collected triangle collection
	//
	if ( collectedTrianglesWithinRange.getSize() > 0 ) {

		hkpRigidBodyCinfo rigidBodyInfo;
		{
			//
			// build a mopp shape from collected triangles
			//
			hkpBvTreeShape* moppShape;
			{
				hkpShapeCollection* shapeCollection;
				{
					const hkpShape*const* firstTriangle = reinterpret_cast<const hkpShape*const*>(collectedTrianglesWithinRange.begin());
					shapeCollection = new hkpListShape(firstTriangle, collectedTrianglesWithinRange.getSize());

					{
						for (int i = 0; i < collectedTrianglesWithinRange.getSize(); i++)
						{
							hkpTriangleShape* triangle = collectedTrianglesWithinRange[i];
							triangle->removeReference();
						}
					}
				}

				hkpMoppCompilerInput mci;
				// Disable interleaved building of mopp as our new terrain object is rather small. This way we can save some memory
				// and slightly increase performance
				mci.m_enableInterleavedBuilding = false;
				// Disabling primitive splitting gives another small performance boost
				mci.m_enablePrimitiveSplitting = false;
				// Enable primitive extent caching
				mci.m_cachePrimitiveExtents = true;

				hkStopwatch m_stopwatch;
				m_stopwatch.start();

				hkArray<hkpMoppCodeReindexedTerminal> reindexedTerminals;
				hkpMoppCode* code = hkpMoppUtility::buildCode(shapeCollection, mci, &reindexedTerminals);

				m_stopwatch.stop();

				moppShape = new hkpMoppBvTreeShape(shapeCollection, code);

				code->removeReference();

				//
				// Output statistics
				//
				if ( m_env->m_reportingLevel >= hkDemoEnvironment::REPORT_INFO )
				{
					hkReal timeBuildMoppCode = m_stopwatch.getElapsedSeconds();
					m_stopwatch.reset();
					hkprintf("Time 'hkpMoppUtility::buildCode' with %d triangles : %g msec\n", collectedTrianglesWithinRange.getSize(), timeBuildMoppCode*1000.0f);
				}

				shapeCollection->removeReference();
			}

			rigidBodyInfo.m_shape = moppShape;
			rigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
			rigidBodyInfo.m_position = hkVector4(0.0f, 0.0f, 0.0f);
		}

		// create new object and insert it into the world
		{
			hkpRigidBody* terrainBody = new hkpRigidBody(rigidBodyInfo);
			rigidBodyInfo.m_shape->removeReference();
			m_world->addEntity(terrainBody);

			// make our new terrain object destructible
			(new BattlefieldTerrainModifier(m_env, m_world, terrainBody, &this->m_explosions))->removeReference();

			terrainBody->removeReference();
		}
	}
}


void BattlefieldMoppDemo::removeTrianglesWithinSphere(hkpRigidBody& rigidBody, hkpBvTreeShape& bvTreeShape, const hkVector4& sphereCenterWorldSpace, float sphereRadius, hkArray<hkpTriangleShape*>& collectedTrianglesWithinRange)
{

	hkVector4 sphereCenterShapeSpace; sphereCenterShapeSpace.setTransformedInversePos(rigidBody.getTransform(), sphereCenterWorldSpace);

	float sphereRadiusSquared = sphereRadius * sphereRadius;

	//
	// collect the keys for all triangles that are within a sphere-radius-sized cube around the sphere's center (using an AABB cast)
	//
	const int maxNumKeys = 2048;
	hkInplaceArray<hkpShapeKey, maxNumKeys> collectedKeysWithinRange;
	{
		hkAabb aabb;
		{
			hkVector4 minimum(-sphereRadius, -sphereRadius, -sphereRadius);
			hkVector4 maximum( sphereRadius,  sphereRadius,  sphereRadius);
			minimum.add4(sphereCenterShapeSpace);
			maximum.add4(sphereCenterShapeSpace);
			aabb.m_min = minimum;
			aabb.m_max = maximum;
		}
		bvTreeShape.queryAabb(aabb, collectedKeysWithinRange);
	}

	//
	// filter all triangles that are truly within the sphere's radius (as opposed to the cube-shaped AABB used above)
	//
	{
		for (int hitlistIdx = collectedKeysWithinRange.getSize()-1; hitlistIdx >= 0; hitlistIdx--)
		{
			hkpShapeCollection::ShapeBuffer buffer;
			const hkpShape* shape = bvTreeShape.getContainer()->getChildShape(collectedKeysWithinRange[hitlistIdx], buffer);

			// safety abort
			if ( shape->getType() != HK_SHAPE_TRIANGLE )
			{
				continue;
			}

			const hkpTriangleShape* triangleShape = static_cast<const hkpTriangleShape*>(shape);

			hkReal distanceSphereCenterToVerticesSquared[3];

			//
			// check for each vertex separately whether it is within the sphere's radius or not
			//
			bool triangleWithinRadius = false;
			{
				for (int j = 0; j < 3; j++)
				{
					hkVector4 vectorCenterToVertex = triangleShape->getVertex(j);
					vectorCenterToVertex.sub4(sphereCenterShapeSpace);

					distanceSphereCenterToVerticesSquared[j] = vectorCenterToVertex.lengthSquared3();

					if ( distanceSphereCenterToVerticesSquared[j] <= sphereRadiusSquared )
					{
						triangleWithinRadius = true;
						continue; // continue (instead of break) as we need the distance for ALL vertices later
					}
				}
			}

			// only process triangles that have at least one vertex within sphere's radius
			if ( triangleWithinRadius == true )
			{
				hkVector4 vertices[3];
				vertices[0] = triangleShape->getVertex(0);
				vertices[1] = triangleShape->getVertex(1);
				vertices[2] = triangleShape->getVertex(2);

				// slightly adapt the maximum 'push-down' to the sphere's size
				float maximumPushdown = (BOMB_MAX_CRATER_DEPTH * (sphereRadius/BOMB_MAX_RADIUS));

				// push down vertices depending on their distance to sphere's center
				{
					for (int j = 0; j < 3; j++)
					{
						if ( distanceSphereCenterToVerticesSquared[j] < sphereRadiusSquared )
						{
							float invPercentageDistance = 1.0f - (distanceSphereCenterToVerticesSquared[j] / sphereRadiusSquared);
							vertices[j](1) -= (invPercentageDistance * maximumPushdown);
						}
						// transform vertices back to world space
						vertices[j].setTransformedPos(rigidBody.getTransform(), vertices[j]);
					}
				}

				// create a new triangle from 'shifted' vertices and store it
				hkpTriangleShape* newTriangle = new hkpTriangleShape(vertices[0], vertices[1], vertices[2]);
				collectedTrianglesWithinRange.pushBack(newTriangle);
			}
			else
			{
				// if triangle is outside of sphere's radius remove shape key from original array (thus disabling collision
				// detection for this particular triangle)
				collectedKeysWithinRange.removeAt(hitlistIdx);
			}

		}
	}

	//
	// remove all triangles from collision that are referenced by the filtered keys
	//
	{
		const hkpMoppCode* moppCode;
		hkpMoppBvTreeShape* moppBvTreeShape = static_cast<hkpMoppBvTreeShape*>(&bvTreeShape);
		moppCode = moppBvTreeShape->getMoppCode();

		hkpRemoveTerminalsMoppModifier modifier(moppCode, bvTreeShape.getContainer(), collectedKeysWithinRange);
		modifier.applyRemoveTerminals( const_cast<hkpMoppCode*>(moppCode) );
	}

	//
	// remove all triangles from display. Note that this utility method will ONLY work with terrain mopps!
	//
	{
		this->removeTrianglesFromDisplayObject(rigidBody, bvTreeShape, collectedKeysWithinRange);
	}

	//
	// we need to reset aabb caching otherwise stationary objects (as aabb hasn't changed!) sitting on removed triangles
	// will hang on to the now-invalid agent.
	//
	{
		m_world->updateCollisionFilterOnEntity(&rigidBody,
			HK_UPDATE_FILTER_ON_ENTITY_DISABLE_ENTITY_ENTITY_COLLISIONS_ONLY,
			HK_UPDATE_COLLECTION_FILTER_PROCESS_SHAPE_COLLECTIONS);
	}

	return;
}



/// This method simply removes the supplied triangles from the object's visualization.
///
/// Note that this method will assume that we are dealing with a terrain mopp that has only one geometry,
/// one face set and one vertex set.
void BattlefieldMoppDemo::removeTrianglesFromDisplayObject(hkpRigidBody& rigidBody, hkpBvTreeShape& bvTreeShape, hkArray<hkpShapeKey>& trianglesToRemove)
{
	//
	// get the terrain's vertex set
	//
	hkgVertexSet* vertexSet;
	{
		const hkpCollidable* collidable = rigidBody.getCollidable();
		hkgDisplayObject* displayObject = m_env->m_displayHandler->findDisplayObject(hkUlong(collidable));
		hkgGeometry* geometry = displayObject->getGeometry(0);
		hkgFaceSet* faceSet = geometry->getMaterialFaceSet(0)->getFaceSet(0);
		vertexSet = faceSet->getVertexSet();
	}

	m_env->m_window->getContext()->lock();

	// vertex set has to be locked before any vertices can be manipulated
	vertexSet->lock(HKG_LOCK_WRITEONLY);

	{
		//
		// Simply zero-out all vertices. This will degenerate the triangles but that should be ok.
		//
		hkVector4 newPos(0,0,0);

		for (int i = 0; i < trianglesToRemove.getSize(); i++)
		{
			hkpShapeKey key = trianglesToRemove[i];

			vertexSet->setVertexComponentData(HKG_VERTEX_COMPONENT_POS, int(key)*3+0, &newPos(0));
			vertexSet->setVertexComponentData(HKG_VERTEX_COMPONENT_POS, int(key)*3+1, &newPos(0));
			vertexSet->setVertexComponentData(HKG_VERTEX_COMPONENT_POS, int(key)*3+2, &newPos(0));
		}

	}

	// unlock the vertex set again
	vertexSet->unlock();

	m_env->m_window->getContext()->unlock();

	return;
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO(
				BattlefieldMoppDemo,
				HK_DEMO_TYPE_PHYSICS,
				"Bombs & Craters (using a MOPP)",
				"Dynamic modification of a MOPP. 'Bombs' will alter the MOPP on impact and create craters " \
				"by removing a set of triangles from the terrain object(s) that are within the bomb's range. A small modified terrain object " \
				"will be created and inserted into the world."
				);

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
