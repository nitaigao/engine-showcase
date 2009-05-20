/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/Midphase/NmMachine/NmMachineDemo.h>
#include <Common/Base/Container/LocalArray/hkLocalBuffer.h>

#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTransform/hkpConvexTransformShape.h>
#include <Physics/Collide/Shape/Misc/Transform/hkpTransformShape.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>

#include <Physics/Collide/Dispatch/hkpCollisionDispatcher.h>
#include <Physics/Internal/Collide/Agent3/CollectionCollection3/hkpCollectionCollectionAgent3.h>

#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpMidphaseViewer.h>

#include <Demos/DemoCommon/Utilities/GameUtils/Landscape/FlatLand.h>

#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Common/Visualize/Shape/hkDisplayGeometry.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/Material/hkgMaterial.h>

struct NmMachineDemoVariant
{
	enum Type
	{
		OLD_LIST_AGENT, 
		OLD_BVTREE_AGENT,

		NEW_COLLECTION_COLLECTION_AGENT,
		NEW_BVTREE_BVTREE_AGENT,

		MOPP_OF_TRANSFORMED_MOPPS
	};

	enum ObjectType
	{
		SNOW_FLAKE,
		MULTI_RING,
	};
	const char*	 m_name;
	ObjectType m_objectType;
	Type m_type;
	const char* m_details;
};


static const NmMachineDemoVariant g_variants[] =
{
	{ "SnowFlakes No_Midphase_ListShape",   NmMachineDemoVariant::SNOW_FLAKE, NmMachineDemoVariant::OLD_LIST_AGENT, "Colliding lists with the old hkListAgent3, n-n collisions." },// with improvement ??
	{ "SnowFlakes No_Midphase_BvTreeShape", NmMachineDemoVariant::SNOW_FLAKE, NmMachineDemoVariant::OLD_BVTREE_AGENT, "Colliding mopps with the old hkBvTreeAgent3, less collisions, many MOPP-AABB queries." }, 

	{ "SnowFlakes Midphase_ListShape",	    NmMachineDemoVariant::SNOW_FLAKE, NmMachineDemoVariant::NEW_COLLECTION_COLLECTION_AGENT, "Colliding lists with the new hkpCollectionCollectionAgent3." },
	{ "SnowFlakes Midphase_BvTreeShape",    NmMachineDemoVariant::SNOW_FLAKE, NmMachineDemoVariant::NEW_BVTREE_BVTREE_AGENT, "Colliding mopps with the new hkpCollectionCollectionAgent3." },
	{ "SnowFlakes Mopp of Transformed Mopps",    NmMachineDemoVariant::SNOW_FLAKE, NmMachineDemoVariant::MOPP_OF_TRANSFORMED_MOPPS, "Colliding MOPPs of transformed MOPPs." },

	{ "Rings No_Midphase_ListShape",   NmMachineDemoVariant::MULTI_RING, NmMachineDemoVariant::OLD_LIST_AGENT, "Colliding lists with the old hkListAgent3, n-n collisions." },// with improvement ??
	{ "Rings No_Midphase_BvTreeShape", NmMachineDemoVariant::MULTI_RING, NmMachineDemoVariant::OLD_BVTREE_AGENT, "Colliding mopps with the old hkBvTreeAgent3, less collisions, many MOPP-AABB queries." }, 

	{ "Rings Midphase_ListShape",      NmMachineDemoVariant::MULTI_RING, NmMachineDemoVariant::NEW_COLLECTION_COLLECTION_AGENT, "Colliding lists with the new hkpCollectionCollectionAgent3." },
	{ "Rings Midphase_BvTreeShape",    NmMachineDemoVariant::MULTI_RING, NmMachineDemoVariant::NEW_BVTREE_BVTREE_AGENT, "Colliding mopps with the new hkpCollectionCollectionAgent3." },
	{ "Rings MOPP of transformed MOPPs",    NmMachineDemoVariant::MULTI_RING, NmMachineDemoVariant::MOPP_OF_TRANSFORMED_MOPPS, "Colliding MOPPs of transformed MOPPs." }
};


NmMachineDemo::~NmMachineDemo()
{
	m_track->removeReference();
}

class MyFlatLand: public FlatLand
{
	public:
		MyFlatLand( int size ): FlatLand(size){ m_borderHeight = 6.0f; }
		hkReal getHeight( int i, int j )
		{
			hkReal invS = 2.0f / m_side;
			hkReal dx = hkReal(i-(m_side>>1)) * invS;
			hkReal dy = hkReal(j-(m_side>>1)) * invS;
			hkReal h = dx * dx + dy * dy;
			return h * 5.0f;
		}
};

NmMachineDemo::NmMachineDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	const NmMachineDemoVariant& variant = g_variants[env->m_variantId];

	//
	// Setup the camera
	//
	{
		hkVector4 from(-15.0f, 24.0f, 14.0f);
		hkVector4 to  (  4.0f,  4.0f,  0.0f);
		hkVector4 up  (  0.0f,  1.0f,  0.0f);
		setupDefaultCameras( env, from, to, up );

		forceShadowState(false);

	}

	{
		hkpWorldCinfo info;
		info.m_gravity.set( 0.0f, -9.81f, 0.0f );
		info.setBroadPhaseWorldSize(1000.0f);
		info.m_collisionTolerance = 0.02f;
		info.m_enableDeactivation = false;
		m_world = new hkpWorld( info );
		m_world->lock();

		// Register ALL agents (though some may not be necessary)
		hkpCollisionDispatcher* dis = m_world->getCollisionDispatcher();
		hkRegisterAlternateShapeTypes(dis);
		hkpAgentRegisterUtil::_registerBvTreeAgents(dis);
		hkpAgentRegisterUtil::_registerListAgents(dis);

	    if (   variant.m_type == NmMachineDemoVariant::NEW_COLLECTION_COLLECTION_AGENT
			|| variant.m_type == NmMachineDemoVariant::NEW_BVTREE_BVTREE_AGENT  )
		{
			m_world->getCollisionDispatcher()->setEnableChecks( false );	// this overrides the more specialized list types
			hkpCollectionCollectionAgent3::registerAgent3(m_world->getCollisionDispatcher());
			m_world->getCollisionDispatcher()->setEnableChecks( true );
		}
		hkpAgentRegisterUtil::_registerTerminalAgents(dis);

		//m_debugViewerNames.pushBack(hkpMidphaseViewer::getName());

		setupGraphics();
	}



	//
	// Create a pile of rubbish
	//
	const int useRing = variant.m_objectType == NmMachineDemoVariant::MULTI_RING;

	{
		hkpShape* shape;
		{
			const bool useSubMopps = (variant.m_type == NmMachineDemoVariant::MOPP_OF_TRANSFORMED_MOPPS);
			hkpListShape* list = useRing ? createRing(useSubMopps) : createSnowFlake(useSubMopps);
			shape = list;

			if ( variant.m_type == NmMachineDemoVariant::OLD_BVTREE_AGENT
			  || variant.m_type == NmMachineDemoVariant::NEW_BVTREE_BVTREE_AGENT 
			  || variant.m_type == NmMachineDemoVariant::MOPP_OF_TRANSFORMED_MOPPS)
			{
				// Build mopp around the shape
				hkpMoppCompilerInput mci;
				mci.m_enableChunkSubdivision = true;
				hkpMoppCode* code = hkpMoppUtility::buildCode( list,mci);
				hkpMoppBvTreeShape* moppShape = new hkpMoppBvTreeShape( list, code );

				code->removeReference();
				list->removeReference();

				shape = moppShape;
			}
		}

		hkpRigidBodyCinfo info;
		{
			info.m_shape = shape;
			hkpInertiaTensorComputer::setShapeVolumeMassProperties(info.m_shape, 1.0f, info);
			info.m_motionType = hkpMotion::MOTION_DYNAMIC;
			info.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING;
			info.m_restitution = 0.0f;
			info.m_position.set(0.0f, 2.0f, 0.0f );
		}

		const int numBodies = useRing ? 2 : 20;
		for (int i = 0; i < numBodies; i++)
		{
			info.m_shape = shape;
			hkpRigidBody* body = new hkpRigidBody(info);
			info.m_position.add4(hkVector4(0.0f, 2.5f, 0.1f));
			m_world->addEntity(body);
			body->removeReference();
		}
		shape->removeReference();


		//
		// Create ground
		//
		hkgMaterial* floorMaterial;
		{
			hkgDisplayContext* ctx = m_env->m_window->getContext();

			floorMaterial = hkgMaterial::create();
			floorMaterial->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
			floorMaterial->setSpecularColor(1.0f, 0.7f, 0.7f);
			floorMaterial->setSpecularPower(10);
			hkgTexture* texture = hkgTexture::create(ctx);
			texture->loadFromFile(".\\Resources\\Internal\\Showcase\\Gdc2005\\Level\\bricks1.png" );
			//texture->loadBuiltIn( HKG_TEXTURE_PERLINNOISE );
			texture->setAutoMipMaps(true);
			texture->realize();
			floorMaterial->addTexture(texture);
			texture->release();

			m_env->m_displayHandler->setTextureMapScale(0.5f);
			m_env->m_displayHandler->setTextureMapLocalMode(false);
		}

		hkpRigidBodyCinfo groundInfo;
		{
			FlatLand* fl = new MyFlatLand(16);
			m_track = fl;
			fl->setScaling( hkVector4( 1.0f, 1.0f, 1.0f ));
			groundInfo.m_shape = fl->createMoppShape();
			groundInfo.m_friction = 0.0f;
			groundInfo.m_position.set(.0f, -2.0f, .0f);
		}
		groundInfo.m_motionType = hkpMotion::MOTION_FIXED;
		groundInfo.m_friction = 0.5f;
		hkpRigidBody* groundbody = new hkpRigidBody(groundInfo);
		groundInfo.m_shape->removeReference();

		m_world->addEntity(groundbody);
		groundbody->removeReference();

		hkgDisplayObject* dispObj = m_env->m_displayHandler->findDisplayObject( (hkUlong)groundbody->getCollidable() );
		if (dispObj)
		{
			dispObj->setMaterialOnAll(floorMaterial);
		}
		floorMaterial->removeReference();
	}

	m_world->unlock();
}

static hkpConvexShape* _createBoxFromTo( hkVector4Parameter from, hkVector4Parameter to, float radius )
{
	hkTransform transform;

	transform.getTranslation().setInterpolate4( from, to, 0.5f );
	hkReal length; 
	{
		hkVector4 x; x.setSub4( to, from );
		length = x.normalizeWithLength3();

		hkVector4 up( 0.0f, 1.0f, 0.0f);
		hkVector4 z; z.setCross( x, up ); z.normalize3();
		hkVector4 y; y.setCross( z, x );  y.normalize3();

		transform.getRotation().setCols(x,y,z);
	}
	hkpBoxShape* shape = new hkpBoxShape( hkVector4(length, radius, radius), 0.0f );
	hkpConvexTransformShape* tShape = new hkpConvexTransformShape(shape, transform );
	shape->removeReference();
	return tShape;
}

static hkpListShape* _createMiniRing( hkVector4Parameter center, float radius, int sides, float capsuleRadius )
{
	hkLocalBuffer<hkVector4> verts(sides);

	int idx = 0;
	for ( float x = 0.0f; x < 2 * HK_REAL_PI-.001f; x+= 2 * HK_REAL_PI / sides )
	{
		verts[idx].set( radius * hkMath::sin(x), x * radius * 0.2f, radius * hkMath::cos(x));
		verts[idx].add4( center );
		idx ++;
	}
	hkLocalBuffer<const hkpConvexShape*> shapes(idx);

	{
		for (int i=0; i < idx;i++)
		{
			shapes[i] = _createBoxFromTo( verts[i], verts[(i+1)%idx], capsuleRadius ) ;
		}
	}


	hkpListShape* list = new hkpListShape( (hkpShape*const*)shapes.begin(), idx);

	{for (int i=0; i < idx;i++){ shapes[i]->removeReference(); }}
	return list;
}


hkpListShape* NmMachineDemo::createRing(bool useSubMopps)
{
	int sides = 80;
	float radius = 4.0f;
	float miniRadius = 0.2f;
	int miniSide = 3;
	float capsRadius = 0.1f;

	hkLocalBuffer<hkpShape*> shapes(sides);

	{
		float x = 0.0f;
		for (int i=0; i < sides;i++, x+= 2 * HK_REAL_PI / sides)
		{
			hkVector4 center; center.setZero4();
			if (!useSubMopps)
			{
				center.set( radius * hkMath::sin(x), 0.0f, radius * hkMath::cos(x));
			}

			hkpListShape* subShape = _createMiniRing(center, miniRadius, miniSide, capsRadius );

			if (useSubMopps)
			{
				// Build mopp around the subList
				hkpMoppCompilerInput mci;
				mci.m_enableChunkSubdivision = true; 
				hkpMoppCode* code = hkpMoppUtility::buildCode(subShape,mci);
				hkpMoppBvTreeShape* moppSubShape = new hkpMoppBvTreeShape(subShape, code);
				code->removeReference();
				subShape->removeReference();				

				center.set( radius * hkMath::sin(x), 0.0f, radius * hkMath::cos(x));
				hkpTransformShape* transformShape = new hkpTransformShape(moppSubShape, hkTransform(hkQuaternion::getIdentity(), center));
				moppSubShape->removeReference();

				shapes[i] = transformShape;
			}
			else
			{
				shapes[i] = subShape;
			}
		}
	}

	hkpListShape* list = new hkpListShape(shapes.begin(), sides);

	{for (int i=0; i < sides;i++){ shapes[i]->removeReference(); }}
	return list;
}

hkpListShape* NmMachineDemo::createSnowFlake(bool useSubMopps)
{
	hkReal crossLength = 0.5f;
	hkReal crossWidth = 0.15f;
	hkpBoxShape* boxes[] = { new hkpBoxShape(hkVector4(crossLength*0.5f, crossWidth*0.5f, crossWidth*0.5f), 0.02f),
		new hkpBoxShape(hkVector4(crossWidth*0.5f, crossLength*0.5f, crossWidth*0.5f), 0.02f),
		new hkpBoxShape(hkVector4(crossWidth*0.5f, crossWidth*0.5f, crossLength*0.5f), 0.02f)
	};

	hkVector4 shiftA[] = { hkVector4((crossLength+crossWidth)* 0.5f, 0.0f, 0.0f),
		hkVector4(0.0f, (crossLength+crossWidth)* 0.5f, 0.0f),
		hkVector4(0.0f, 0.0f, (crossLength+crossWidth)* 0.5f)
	};
	hkVector4 shiftB[] = { hkVector4((crossLength+crossWidth), 0.0f, 0.0f),
		hkVector4(0.0f, (crossLength+crossWidth), 0.0f),
		hkVector4(0.0f, 0.0f, (crossLength+crossWidth))
	};

	hkArray<hkpShape*> allShapes; 
	for (int a = 0; a < 3; a++)
	{
		hkArray<hkpShape*> someShapes;
		for (int b = 0; b < 3; b++)
		{
			hkTransform transform;
			transform.setIdentity();
			transform.getRotation().setAxisAngle(hkVector4(1.0f, 0.0f, 0.0f), 0.731f);
			hkVector4& shift = transform.getTranslation();
			shift.setAdd4(shiftB[b], shiftA[a]);

			someShapes.pushBack( new hkpConvexTransformShape(boxes[b], transform ) );
			shift.setNeg4(shift);
			someShapes.pushBack( new hkpConvexTransformShape(boxes[b], transform ) );
			shift.setSub4(shiftB[b], shiftA[a]);
			someShapes.pushBack( new hkpConvexTransformShape(boxes[b], transform ) );
			shift.setNeg4(shift);
			someShapes.pushBack( new hkpConvexTransformShape(boxes[b], transform ) );
		}

		if (useSubMopps)
		{
			hkpListShape* subList = new hkpListShape(someShapes.begin(), someShapes.getSize());
			for (int i =0; i < someShapes.getSize(); i++){ someShapes[i]->removeReference(); }

			// Build mopp around the subList
			hkpMoppCompilerInput mci;
			mci.m_enableChunkSubdivision = true; 
			hkpMoppCode* code = hkpMoppUtility::buildCode(subList,mci);
			hkpMoppBvTreeShape* moppSubShape = new hkpMoppBvTreeShape(subList, code);
			code->removeReference();
			subList->removeReference();

			hkpTransformShape* transformShape = new hkpTransformShape(moppSubShape, hkTransform::getIdentity());
			moppSubShape->removeReference();

			allShapes.pushBack(transformShape);

		}
		else
		{
			allShapes.insertAt(allShapes.getSize(), someShapes.begin(), someShapes.getSize());
		}
	}

	hkpListShape* list = new hkpListShape(allShapes.begin(), allShapes.getSize());

	{	for (int i =0; i < allShapes.getSize(); i++){ allShapes[i]->removeReference(); }	}

	boxes[0]->removeReference();
	boxes[1]->removeReference();
	boxes[2]->removeReference();

	return list;
}


hkDemo::Result NmMachineDemo::stepDemo()
{
	return hkDefaultPhysicsDemo::stepDemo();
}



#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif


HK_DECLARE_DEMO_VARIANT_USING_STRUCT( NmMachineDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_CRITICAL, NmMachineDemoVariant, g_variants, HK_NULL);

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
