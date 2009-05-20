/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Constraints/Chain/ChainNet/ChainNetDemo.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>


// We will need these collision agents
#include <Physics/Collide/Agent/ConvexAgent/BoxBox/hkpBoxBoxAgent.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/Convex/Cylinder/hkpCylinderShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>

#include <Physics/Dynamics/Constraint/Chain/BallSocket/hkpBallSocketChainData.h>

#include <Physics/Dynamics/Constraint/Chain/hkpConstraintChainInstance.h>

//#include <hkvisualize/hkDebugDisplay.h>


struct ChainNetVariant
{
	enum Scenario
	{
		FISHING_NET,
		WARM_BLANKET
	};
	const char*	 m_name;
	int m_numBodies;
	Scenario m_scenario;
	const char* m_details;
};

static const char details[] = \
"This demo is just an example of a system of a larger number of overlapping constraint chains. "\
"It shows a square grid of object linked by a series of vertical and horizontal chains. "\
"To see a clearer example of setting up a chain constraint refer to the BridgeDemo. ";

static const ChainNetVariant g_variants[] =
{
	{ "Fishing net,  Length   5" ,   5, ChainNetVariant::FISHING_NET, details },
	{ "Fishing net,  Length  10" ,  10, ChainNetVariant::FISHING_NET, details },
	{ "Fishing net,  Length  15" ,  15, ChainNetVariant::FISHING_NET, details },

	{ "Warm blanket, Length   5" ,   5, ChainNetVariant::WARM_BLANKET, details },
	{ "Warm blanket, Length  10" ,  10, ChainNetVariant::WARM_BLANKET, details },
	{ "Warm blanket, Length  15" ,  15, ChainNetVariant::WARM_BLANKET, details },
};


ChainNetDemo::ChainNetDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env, DEMO_FLAGS_NO_SERIALIZE)
{

	// Disable warnings:
	hkError::getInstance().setEnabled(0x2a1db936, false); //'Constraint added between two *colliding* dynamic rigid bodies. Check your collision filter logic...'

	const ChainNetVariant& variant = g_variants[env->m_variantId];

	const hkVector4 shapeHalfSize(0.15f, 0.15f, 0.025f);

	//
	// Setup the camera
	//
	{
		hkReal size = hkReal(variant.m_numBodies-1);
		hkReal fromDistFactor = variant.m_scenario == ChainNetVariant::FISHING_NET ? 1.0f : 2.0f;
		hkVector4 from(size * shapeHalfSize(0), -size * shapeHalfSize(1) * 2.0f * fromDistFactor, 0.9f * size * shapeHalfSize(1) * fromDistFactor);
		hkVector4 to  (size * shapeHalfSize(0), size * shapeHalfSize(1), 0.0f);
		hkVector4 up  (0.0f, 0.0f, 1.0f);
		setupDefaultCameras( env, from, to, up, 0.1f, 500.0f );
	}

	//
	// Create the world
	//
	{
		hkpWorldCinfo info;
		info.setBroadPhaseWorldSize( 1000.0f );
		info.m_gravity.set(0.0f, 0.0f, -9.81f);
		info.m_enableDeactivation = false;
		m_world = new hkpWorld( info );
		m_world->lock();
	
		setupGraphics();

		
		//
		// Create a group filter to avoid intra-collision for the fixed net
		//
		{
			hkpGroupFilter* filter = new hkpGroupFilter();
			m_world->setCollisionFilter( filter );
			filter->removeReference();
		}
		
		
	}

	hkpAgentRegisterUtil::registerAllAgents( m_world->getCollisionDispatcher() );

	hkReal elemSpacingToElemSizeRatio = 1.0f;
	if( ChainNetVariant::WARM_BLANKET == variant.m_scenario )
	{
		// object collide and must be placed further apart to avoid weird behavior
		elemSpacingToElemSizeRatio = 1.1f;
	}

	hkVector4 netEdges[2] = { hkVector4( shapeHalfSize(0) * 2.0f * elemSpacingToElemSizeRatio, 0.0f, 0.0f),
	                          hkVector4( 0.0f, shapeHalfSize(1) * 2.0f * elemSpacingToElemSizeRatio, 0.0f) };
	netEdges[0].mul4( hkReal(variant.m_numBodies-1) );
	netEdges[1].mul4( hkReal(variant.m_numBodies-1) );

	//
	// Create ground
	//

	if (variant.m_scenario == ChainNetVariant::WARM_BLANKET)
	{
		hkpRigidBodyCinfo info;
		info.m_motionType = hkpMotion::MOTION_FIXED;
		info.m_shape = new hkpBoxShape( hkVector4(10.0f, 10.0f, 0.1f) );

		hkpRigidBody* ground = new hkpRigidBody(info);
		info.m_shape->removeReference();

		m_world->addEntity(ground);
		ground->removeReference();
	}

	//
	// Create fish for the fishing_net scenario
	//

	if (ChainNetVariant::FISHING_NET == variant.m_scenario)
	{
		int numFish = 40;

		hkVector4 baseFishHalfSize(0.150f, 0.075f, 0.050f);

		hkPseudoRandomGenerator generator(0xB52);

		hkpRigidBodyCinfo info;
		info.m_motionType = hkpMotion::MOTION_DYNAMIC;

		for (int d = 0; d < numFish; d++)
		{
			info.m_position.setMul4( generator.getRandReal01(), netEdges[0] );
			info.m_position.addMul4( generator.getRandReal01(), netEdges[1] );
			info.m_position.addMul4( generator.getRandRange(0.0f, 5.0f), hkTransform::getIdentity().getColumn(2) );

			generator.getRandomRotation( info.m_rotation );

			hkVector4 fishSize; fishSize.setMul4( generator.getRandRange(1.0f, 3.0f), baseFishHalfSize );
			info.m_shape = new hkpBoxShape( fishSize, 0.01f );
			hkpInertiaTensorComputer::setShapeVolumeMassProperties( info.m_shape, 10.0f, info );

			hkpRigidBody* fish = new hkpRigidBody(info);
			info.m_shape->removeReference();

			m_world->addEntity(fish);
			fish->removeReference();
		}

	}


	//
	// Create net 
	//

	{
		//
		// Create net-element info structure, build shape
		//
		hkpRigidBodyCinfo info;

		switch( variant.m_scenario )
		{
			case ChainNetVariant::FISHING_NET:
			{
				hkpShape* shapes[2] = { new hkpCapsuleShape( hkVector4(  shapeHalfSize(0), 0.0f, 0.0f ), 
														    hkVector4( -shapeHalfSize(0), 0.0f, 0.0f ), 
															shapeHalfSize(2) ),
									   new hkpCapsuleShape( hkVector4( 0.0f,  shapeHalfSize(1), 0.0f ), 
															hkVector4( 0.0f, -shapeHalfSize(1), 0.0f ), 
															shapeHalfSize(2) )
									 };
				info.m_shape = new hkpListShape(shapes, 2);
				shapes[0]->removeReference();
				shapes[1]->removeReference();
				break;
			}
			case ChainNetVariant::WARM_BLANKET:
			{
				info.m_shape = new hkpCylinderShape( hkVector4( 0.0f, 0.0f,  shapeHalfSize(2) ),
					                                hkVector4( 0.0f, 0.0f, -shapeHalfSize(2) ),
													hkMath::min2( shapeHalfSize(0), shapeHalfSize(1)), 
													0.01f );
				break;
			}
		}

		info.m_linearDamping = 0.1f;
		info.m_angularDamping = 0.5f;
		hkpInertiaTensorComputer::setShapeVolumeMassProperties(info.m_shape, 5.0f, info);
		info.m_mass = 1.0f;
		info.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		// disable inter collision for the fishing net
		info.m_collisionFilterInfo = hkpGroupFilter::calcFilterInfo( 1, ChainNetVariant::FISHING_NET == variant.m_scenario ? 1 : 0, 0, 0 );
		info.m_position(2) = 3.0f * hkReal(variant.m_scenario == ChainNetVariant::WARM_BLANKET);

		createNetAndAddToWorld( info, netEdges[0], netEdges[1], variant.m_numBodies, variant, m_world );

		info.m_shape->removeReference();
	}

	m_world->unlock();
}

ChainNetDemo::~ChainNetDemo()
{
	// Re-enable warning
	hkError::getInstance().setEnabled(0x2a1db936,true);
}


void ChainNetDemo::createNetAndAddToWorld(const hkpRigidBodyCinfo& bodyInfo, const hkVector4& edgeX, const hkVector4& edgeY, int numBodies, const struct ChainNetVariant& variant, hkpWorld* world)
{
	//
	// Create a grid of bodies
	//

	hkVector4 spacingX; spacingX.setMul4( 1.0f / (numBodies-1), edgeX );
	hkVector4 spacingY; spacingY.setMul4( 1.0f / (numBodies-1), edgeY );

	hkArray<hkpRigidBody*> bodies;
	{
		hkBool fixCorners = ChainNetVariant::FISHING_NET == variant.m_scenario;

		for (int x = 0; x < numBodies; x++)
		{
			for (int y = 0; y < numBodies; y++)
			{
				hkpRigidBodyCinfo info = bodyInfo;

				info.m_position.addMul4( hkReal(x), spacingX );
				info.m_position.addMul4( hkReal(y), spacingY );

				hkpRigidBody* body = HK_NULL;

				if( (0 == x || numBodies-1 == x) && (0 == y || numBodies-1 == y) )
				{
					if (fixCorners)
					{
						info.m_motionType = hkpMotion::MOTION_FIXED;
						info.m_position(2) += 0.01f;
					}
					else // make the corners heavier
					{
						const hkReal scale = 10.0f;
						info.m_mass *= scale;
						info.m_inertiaTensor.mul( scale );

						{
							HK_ASSERT2(0xad677788, HK_SHAPE_CYLINDER == info.m_shape->getType(), "The blanket scenario of ChainNetDemo only supports cylindical shapes (you may disable corner element scaling if you want to use other shapes)." );
							const hkpCylinderShape* cylinder = static_cast<const hkpCylinderShape*>(info.m_shape);
							hkVector4 verts[2];
							verts[0].setMul4( scale, cylinder->getVertex(0) );
							verts[1].setMul4( scale, cylinder->getVertex(1) );
							info.m_shape = new hkpCylinderShape( verts[0], verts[1], cylinder->getCylinderRadius(), cylinder->getRadius() );
							body = new hkpRigidBody(info);
							info.m_shape->removeReference();
						}
					}
				}

				if (!body)
				{
					body = new hkpRigidBody(info);
				}

				m_world->addEntity(body);

				bodies.pushBack(body);

				// We're just removing the reference here, as we know that another is kept in the hkpWorld
				// until the end of this function.
				body->removeReference();

			}
		}
	}



	for (int dir = 0; dir < 2; dir++)
	{
		int bodyStriding = dir ? 1 : numBodies;

		int& numChains = numBodies;
		for( int c = 0; c < numChains; c++ )
		{
			int firstBody = c * (dir ? numBodies : 1);

			hkpConstraintChainInstance* chainInstance = HK_NULL;

			{
				hkpBallSocketChainData* chainData = new hkpBallSocketChainData();
				chainData->m_cfm = HK_REAL_EPSILON;
				chainInstance = new hkpConstraintChainInstance( chainData );

				hkVector4 pivots[2];
				pivots[0].setMul4( 0.51f, dir ? spacingY : spacingX );
				pivots[1].setNeg4( pivots[0] );

				chainInstance->addEntity( bodies[firstBody] );
				for (int e = 1; e < numBodies; e++)
				{
					chainData->addConstraintInfoInBodySpace( pivots[0], pivots[1] );
					chainInstance->addEntity( bodies[firstBody + e * bodyStriding] );
				}

				chainData->removeReference();
			}

			m_world->addConstraint( chainInstance );
			chainInstance->removeReference();
		}
	}

}




////////////////////////////////////////////////////////////////////////////////

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"Show examples of using a larger number of overlapping constraint chains.";


HK_DECLARE_DEMO_VARIANT_USING_STRUCT( ChainNetDemo, HK_DEMO_TYPE_PRIME, ChainNetVariant, g_variants, helpString );

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
