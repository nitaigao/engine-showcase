/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/ShowCase/Gdc2005/Gdc2005Demo.h>

// Serialization
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>

// Asset mgt
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>

// Scene Data
#include <Common/SceneData/Scene/hkxScene.h>
#include <Common/SceneData/Graph/hkxNode.h>
#include <Common/SceneData/Mesh/hkxMesh.h>
#include <Common/SceneData/Mesh/hkxMeshSection.h>
#include <Common/SceneData/Mesh/hkxIndexBuffer.h>
#include <Common/SceneData/Mesh/hkxVertexBuffer.h>
#include <Animation/Animation/Rig/hkaPose.h>
// included for matrix polar decomposition util
#include <Common/Base/Math/Util/hkMathUtil.h>

// Physics
#include <Physics/Collide/Shape/Misc/Transform/hkpTransformShape.h>
#include <Physics/Collide/Shape/Deprecated/Mesh/hkpMeshShape.h>
#include <Physics/Internal/Collide/Mopp/Code/hkpMoppCode.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>


#include <Animation/Ragdoll/Instance/hkaRagdollInstance.h> // to fire shots at it
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/Collector/RayCollector/hkpClosestRayHitCollector.h>

#include <Physics/Collide/Shape/Query/hkpShapeRayCastInput.h>
#include <Physics/Collide/Shape/Query/hkpShapeRayCastOutput.h>
#include <Physics/Utilities/Actions/MouseSpring/hkpMouseSpringAction.h>

// Graphics & Window Stuff
#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.h>
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Graphics/Common/hkGraphics.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/Light/hkgLightManager.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/DisplayWorld/hkgDisplayWorld.h>
#include <Common/Visualize/hkDebugDisplay.h>
#include <Physics/Dynamics/World/BroadPhaseBorder/hkpBroadPhaseBorder.h>

class Gdc2005BroadPhaseBorder : public hkpBroadPhaseBorder
{
public:

	Gdc2005BroadPhaseBorder(Gdc2005Demo* game, hkpWorld* world )
		: hkpBroadPhaseBorder( world ), m_demo(game)
	{
	}

	virtual void maxPositionExceededCallback( hkpEntity* entity )
	{
		hkpRigidBody* body = static_cast<hkpRigidBody*>(entity);

		// see if it is a chuckable (deleteable) or a ragdoll (just remove from world as
		// assumptions may be made that it will exist).
		if (m_demo->isChuckable(body))
		{
			m_demo->removeChuckable(body);
		}
		else if (m_demo->isBrick(body))
		{
			m_demo->removeBrick(body);
		}
		else // must be a ragdoll part
			// just remove it? not yet as the ragdoll code assumes the bodies say in the world.
			// Could warp the guy back into a well known pos..
			// Should check to see if it is the main char (has to be when this was written anyway)
		{
			m_demo->warpCharacter(0);
		}
	}

	Gdc2005Demo* m_demo;
};

static void _findAllNodesByName(hkxNode* fromNode, const char* prefix, int prefixLen, hkArray<hkxNode*>& foundNodes)
{
	if (fromNode)
	{
		if (fromNode->m_name && (hkString::strNcasecmp(prefix, fromNode->m_name, prefixLen) == 0) )
		{
			foundNodes.pushBack( fromNode );
		}

		for (int i=0; i < fromNode->m_numChildren; ++i)
		{
			_findAllNodesByName( fromNode->m_children[i], prefix, prefixLen, foundNodes);
		}
	}
}

static void _decomposeNodeTransforms(hkArray<hkxNode*>& nodes, hkArray<hkTransform>& transforms)
{
	for (int wi=0; wi < nodes.getSize(); ++wi)
	{
		HK_ASSERT(0x0, nodes[wi]->m_numKeyFrames > 0);
		hkMatrix4& m = nodes[wi]->m_keyFrames[0]; // can have scale. A bit over kill to get rid of it, but might as well.
		hkTransform& t = transforms.expandOne();
		hkMathUtil::Decomposition decomposition;
		hkMathUtil::decomposeMatrix( m, decomposition );
		if( !decomposition.m_hasScale && !decomposition.m_hasSkew )
		{
			t.set( decomposition.m_rotation, decomposition.m_translation );
		}
		else
		{
			transforms.popBack(); // can't use it
		}
	}
}

void Gdc2005Demo::loadSceneWithPhysics(const char* filename, hkgAssetConverter::ConvertMask convMask, int layer, int systemGroup)
{
	hkString assetFile = hkAssetManagementUtil::getFilePath(filename);
	hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
	HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");

	hkxScene* scene = reinterpret_cast<hkxScene*>( container->findObjectByType( hkxSceneClass.getName() ));
	hkpPhysicsData* physics = reinterpret_cast<hkpPhysicsData*>( container->findObjectByType( hkpPhysicsDataClass.getName() ));
	//HK_ASSERT2(0x27343635, scene, "No physical scene loaded");

	// Graphics
	if (scene)
	{
		m_env->m_sceneConverter->convert( scene, convMask );

		// see if we have any warp points (Nodes that start with the world 'WarpPoint')
		hkArray<hkxNode*> warpNodes;
		_findAllNodesByName(scene->m_rootNode, "WarpPoint", 9, warpNodes);
		_decomposeNodeTransforms(warpNodes, m_warpPoints);

		// see if we have any brick wall references
		warpNodes.setSize(0);
		_findAllNodesByName(scene->m_rootNode, "BrickWallPoint", 14, warpNodes);
		_decomposeNodeTransforms(warpNodes, m_wallPoints);

		if (m_numCameras == 0)
		{
			m_numCameras = scene->m_numCameras;
			m_cameras = scene->m_cameras;
			m_currentCamera = 0;
		}
	}

	// Physics
	if (physics)
	{
		const hkArray<hkpPhysicsSystem*>& psys = physics->getPhysicsSystems();

		// Tie the two together
		for (int i=0; i<psys.getSize(); i++)
		{
			hkpPhysicsSystem* system = psys[i];

			// Change the layer of the rigid bodies, and also make the seesaw uses continuous CD
			if (layer>=0)
			{
				for (int rb=0; rb<system->getRigidBodies().getSize(); rb++)
				{
					hkpRigidBody* rigidBodyPtr = system->getRigidBodies()[rb];
					rigidBodyPtr->setCollisionFilterInfo( hkpGroupFilter::calcFilterInfo(layer,systemGroup) );

					// Find the seesaw, and ensure it uses continuous physics as it is quite thin
					if ( (hkString::strNcasecmp(rigidBodyPtr->getName(), "Box01", 5) == 0)
						&& ( rigidBodyPtr->getQualityType() == HK_COLLIDABLE_QUALITY_DEBRIS ) )
					{
						rigidBodyPtr->setQualityType( HK_COLLIDABLE_QUALITY_CRITICAL );
					}
				}
			}

			// Associate the display and physics (by name)
			if (scene)
			{
				addPrecreatedDisplayObjectsByName( psys[i]->getRigidBodies(), scene );
			}

			// add the lot to the world
			m_world->addPhysicsSystem(system);
		}
	}
}

void Gdc2005Demo::initLevel()
{
	m_env->m_displayHandler->setAutoDisplayCreationState(false);

	m_lightmapDisplay = HK_NULL;
	m_vertexColorDisplay = HK_NULL;

	// Level itself
	// See if we have to load a vertex color based baked shadow or can load a lightmap
	if (m_env->m_window->getMaxTextureBlendStages() > 1 )
	{
		loadSceneWithPhysics( "Resources/Animation/Showcase/Gdc2005/Level/hkGdcLevel_Lightmap.hkx", hkgAssetConverter::ConvertMask(hkgAssetConverter::NO_LIGHTS | hkgAssetConverter::NO_CAMERAS), LAYER_LANDSCAPE);

		if (m_env->m_sceneConverter->m_meshes.getSize() > 0)
		{
			m_lightmapDisplay = (hkgDisplayObject*) m_env->m_sceneConverter->m_meshes.back().m_hkgObject;
			m_lightmapDisplay->reference();
		}
	}

	// Vertex Color version (so we can toggle)
	// Include the lights, cameras etc from this one or the physics one.
	// For reduced texture size too, use hkGdcLevel_VertexColors_64.hkx (PSP(R) (PlayStation(R)Portable) etc)
	loadSceneWithPhysics( "Resources/Animation/Showcase/Gdc2005/Level/hkGdcLevel_VertexColors.hkx", hkgAssetConverter::CONVERT_ALL, LAYER_LANDSCAPE);
	if (m_env->m_sceneConverter->m_meshes.getSize() > 0)
	{
		m_vertexColorDisplay = (hkgDisplayObject*) m_env->m_sceneConverter->m_meshes.back().m_hkgObject;
		m_vertexColorDisplay->reference();
		if (m_vertexColorDisplay && m_lightmapDisplay) // prefer lightmap if one.
		{
			int idx = m_env->m_displayWorld->findDisplayObject(m_vertexColorDisplay);
			if (idx>=0)
			{
				m_env->m_displayWorld->removeDisplayObject(idx); // hands back ref
				m_vertexColorDisplay->release();
			}
		}
	}

	// Physics (reduced colision mesh). Can be included in one of the above files if you wanted
	// Vertex Color version (so we can toggle)
	loadSceneWithPhysics( "Resources/Animation/Showcase/Gdc2005/Level/hkGdcLevel.hkx", hkgAssetConverter::NO_LIGHTS, LAYER_LANDSCAPE);

	// See-saw
	loadSceneWithPhysics( "Resources/Animation/Showcase/Gdc2005/Level/hkGdcSeesaw.hkx", hkgAssetConverter::ConvertMask(hkgAssetConverter::NO_LIGHTS | hkgAssetConverter::NO_CAMERAS), LAYER_MOVABLE_ENVIRONMENT, 1);

	// Brick walls
	initBrickWall();

	// make sure nothing can fly off the world and disturb performance
	Gdc2005BroadPhaseBorder* border = new Gdc2005BroadPhaseBorder(this, m_world);
	border->removeReference();

	m_env->m_displayHandler->setAutoDisplayCreationState(true);
}

bool Gdc2005Demo::isBrick(hkpEntity* e)
{
	return m_bricks.indexOf((hkpRigidBody*)e) >= 0;
}

void Gdc2005Demo::removeBrick(hkpRigidBody* rb)
{
	int idx = m_bricks.indexOf(rb);
	if (idx >= 0)
	{
		hkpRigidBody* rbX = m_bricks[idx];
		m_world->removeEntity(rbX); // we don't hold a ref.
		m_bricks.removeAt(idx);
	}
}

void Gdc2005Demo::rebuildBrickWall()
{
	if ((m_wallPoints.getSize() < 1) || (m_bricks.getSize() < 1))
		return;

	hkpRigidBody* sourceBrickBody = m_bricks[0];
	hkgDisplayObject* sourceBrickObject = m_env->m_displayHandler->findDisplayObject( (hkUlong)(sourceBrickBody->getCollidable()) );

	const float BOX_HEIGHT = 0.4f;
	const float BOX_WIDTH = 0.75f;

	int curBrick = 0;
	for (int wi=0; sourceBrickBody && (wi < m_wallPoints.getSize()); ++wi)
	{
		hkTransform& startPos = m_wallPoints[wi];
		hkVector4 posx = startPos.getTranslation();

		for ( int x = 0; x < m_options.m_Physics.m_brickWallHeight; x++ )		// along the ground
		{
			hkVector4 pos = posx;
			if (x%2 == 1)
				pos(2) += BOX_WIDTH * 0.5f;

			for( int y = 0; y < m_options.m_Physics.m_brickWallWidth; y++ )
			{
				bool needAnotherBrick = curBrick >= m_bricks.getSize();
				hkpRigidBody* rbC = HK_NULL;
				if (needAnotherBrick ) // clone
				{
					// Clone physics
					rbC = sourceBrickBody->clone();

					// Clone display (share vbs etc)
					hkgDisplayObject* doC = sourceBrickObject->copy(HKG_DISPLAY_OBJECT_SHARE_GEOM, HK_NULL);

					// Add to world and graphics, but first make sure we use the graphical version for display
					m_env->m_displayHandler->addPrecreatedDisplayObject( (hkUlong)(rbC->getCollidable()), doC );
					m_env->m_displayWorld->addDisplayObject( doC );
					doC->removeReference();

					// Set sim quality
					rbC->setQualityType( HK_COLLIDABLE_QUALITY_MOVING );

					// Set the layer correctly for our filtering
					rbC->setCollisionFilterInfo(hkpGroupFilter::calcFilterInfo(LAYER_DEBRIS,0));

					m_bricks.pushBack(rbC);
				}
				else
				{
					rbC = m_bricks[curBrick];
				}

				rbC->setRotation( hkQuaternion(startPos.getRotation()) );
				rbC->setPosition(pos);

				if (needAnotherBrick)
				{
					m_world->addEntity(rbC);
					rbC->removeReference();
				}

				++curBrick;
				pos(2) += BOX_WIDTH; // next along
			}

			posx(1) += BOX_HEIGHT; // next row up
		}
	}

	// remove any extra ones
	int newBricksSize = curBrick;
	while (curBrick < m_bricks.getSize())
	{
		//
		hkpRigidBody* rbX = m_bricks[curBrick++];
		m_world->removeEntity(rbX); // we don't hold a ref.
	}

	if (m_bricks.getSize() != newBricksSize)
		m_bricks.setSize(newBricksSize); // reduce size.
}

void Gdc2005Demo::initBrickWall()
{
	if (m_wallPoints.getSize() < 1)
		return;

	// Load the bricks
	int beforeNum = m_env->m_sceneConverter->m_meshes.getSize();
	loadSceneWithPhysics( "Resources/Animation/Showcase/Gdc2005/Objects/hkGdcBricks.hkx", hkgAssetConverter::ConvertMask(hkgAssetConverter::NO_LIGHTS | hkgAssetConverter::NO_CAMERAS), LAYER_MOVABLE_ENVIRONMENT);

	// find the two we are expecting
	int afterNum = m_env->m_sceneConverter->m_meshes.getSize();

	int largeBrickIndex = -1;
//	int smallBrickIndex = -1;
	for (int mi=beforeNum; mi < afterNum; ++mi)
	{
		hkgDisplayObject* o = (hkgDisplayObject*)m_env->m_sceneConverter->m_meshes[mi].m_hkgObject;
		if ( o->getName().getLength() > 4 )
		{
			if (hkString::strNcasecmp(o->getName().cString(), "Large", 5) == 0)
			{
				largeBrickIndex = mi;
			}
			else // not used, remove     if (hkString::strNcasecmp(o->getName().cString(), "Small", 5) == 0)
			{
				const hkUlong id = m_env->m_displayHandler->getDisplayObjectId( o );
				hkpRigidBody* rb = hkGetRigidBody( (const hkpCollidable*)id );
				if (rb)
				{
					m_world->removeEntity(rb);
				}
			}
		}
	}

	HK_ASSERT2(0x0, m_bricks.getSize() == 0, "Only call initBrickWall once");

	if ( /*(smallBrickIndex >=0) && */ (largeBrickIndex >= 0) )
	{
		hkgDisplayObject* largeBrickObject = (hkgDisplayObject*)m_env->m_sceneConverter->m_meshes[largeBrickIndex].m_hkgObject;
	//	hkgDisplayObject* smallBrickObject = (hkgDisplayObject*)m_env->m_sceneConverter->m_meshes[smallBrickIndex].m_hkgObject;
		hkUlong id = m_env->m_displayHandler->getDisplayObjectId( largeBrickObject );
		hkpRigidBody* largeBrickBody = (id > 0)? hkGetRigidBody( (hkpCollidable*)id ) : HK_NULL;
	//	hkpRigidBody* smallBrickBody = hkGetRigidBody( (hkpCollidable*)m_env->m_displayHandler->getDisplayObjectId( smallBrickObject )->m_id );

		// Set sim quality
		largeBrickBody->setQualityType( HK_COLLIDABLE_QUALITY_MOVING );

		// Set the layer correctly for our filtering
		largeBrickBody->setCollisionFilterInfo(hkpGroupFilter::calcFilterInfo(LAYER_DEBRIS,0));

		m_bricks.pushBack( largeBrickBody ); // one brick to clone

		rebuildBrickWall();
	}
}
void Gdc2005Demo::initDynamicObjects()
{
	m_env->m_displayHandler->setAutoDisplayCreationState(false);

	hkString assetFile = hkAssetManagementUtil::getFilePath("Resources/Animation/Showcase/Gdc2005/Objects/Objects_Default.hkx");
	hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
	HK_ASSERT2(0x27343437, container != HK_NULL , "Could not load asset");

	hkxScene* scene = reinterpret_cast<hkxScene*>( container->findObjectByType( hkxSceneClass.getName() ));
	hkpPhysicsData* physics = reinterpret_cast<hkpPhysicsData*>( container->findObjectByType( hkpPhysicsDataClass.getName() ));
	HK_ASSERT2(0x27343635, scene && physics, "No physical scene loaded");

	// Graphics
	int beforeNum = m_env->m_sceneConverter->m_meshes.getSize();
	m_env->m_sceneConverter->convert( scene );
	int afterNum = m_env->m_sceneConverter->m_meshes.getSize();

	// Physics
	const hkArray<hkpPhysicsSystem*>& psys = physics->getPhysicsSystems();

	hkArray<hkgDisplayObject*> usedObjects;
	// We don't want them in either the graphics or physical world
	// until we chuck them, so don't add them to the physical one
	// and remove from the graphical
	for (int i=0; i<psys.getSize(); i++)
	{
		hkpPhysicsSystem* system = psys[i];
		const hkArray<hkpRigidBody*>& rbs = system->getRigidBodies();
		for (int j=0; j < rbs.getSize(); ++j)
		{
			if (rbs[j]->isFixedOrKeyframed()) continue; // not a chuckable item

			rbs[j]->getMaterial().setRestitution(0.0f);

			hkgDisplayObject* dispObj = findMeshDisplay( rbs[j]->getName(), scene );
			if (dispObj)
			{
				// add to our chuckable items
				Gdc2005Demo::ObjectMap& c = m_chuckables.expandOne();
				c.m_dispObj = dispObj;
				c.m_rb = rbs[j]; // no need to add ref as it is from a resource / held implicitly by the hkLoader

				// Default exported quality type for moving objects is 'debris' which won't have full
				// continous detection so we want proper quality:
				c.m_rb->setQualityType( HK_COLLIDABLE_QUALITY_MOVING );

				// don't want it in our world yet
				int idx = m_env->m_displayWorld->findDisplayObject(dispObj);
				if (idx >= 0)
				{
					m_env->m_displayWorld->removeDisplayObject(idx); // gives us the reference
				}
				else // add our reference
				{
					dispObj->reference();
				}

				usedObjects.pushBack(dispObj);
			}
		}
	}

	// remove any used meshes (some graphical versions of collision meshes etc perhaps)
	for (int di=beforeNum; di < afterNum; ++di)
	{
		hkgDisplayObject* dispObj = (hkgDisplayObject*) m_env->m_sceneConverter->m_meshes[di].m_hkgObject;
		if (usedObjects.indexOf(dispObj) < 0)
		{
			int idx = m_env->m_displayWorld->findDisplayObject(dispObj);
			if (idx >= 0)
			{
				m_env->m_displayWorld->removeDisplayObject(idx)->removeReference();
			}
		}
	}

	m_env->m_displayHandler->setAutoDisplayCreationState(true);
}

void Gdc2005Demo::chuckSomething( const hkVector4& startPos, const hkVector4& velocity )
{
	// what to chuck?
	hkpRigidBody* rbC = HK_NULL;
	bool clonedItem = m_chuckedItems.getSize() < m_options.m_Physics.m_maxNumBodies;
	if (clonedItem)
	{
		int numChuckable = m_chuckables.getSize();
		int objectIdx = 0;

		if (m_options.m_Physics.m_throw < numChuckable)
		{
			objectIdx = m_options.m_Physics.m_throw;
		}
		else
		{
			int dontThrow = (m_options.m_Physics.m_throw == hkGdcPhysicsOptions::DEBRIS) ? hkGdcPhysicsOptions::BOXES /* Box */ : -1;
			do
			{
				float v = m_randomNumberGen.getRandReal01() * numChuckable;
				objectIdx = hkMath::hkFloatToInt(v);
			} while(objectIdx == dontThrow);
		}

		if (objectIdx == m_chuckables.getSize())
			objectIdx = m_chuckables.getSize() - 1;

		ObjectMap& toClone = m_chuckables[objectIdx];

		// Clone physics
		rbC = toClone.m_rb->clone();

		// Clone display (share vbs etc)
		hkgDisplayObject* doC = toClone.m_dispObj->copy(HKG_DISPLAY_OBJECT_SHARE_GEOM, HK_NULL);

		// Add to world and graphics, but first make sure we use the graphical version for display
		m_env->m_displayHandler->addPrecreatedDisplayObject( (hkUlong)(rbC->getCollidable()), doC );
		m_env->m_displayWorld->addDisplayObject( doC );
		doC->removeReference();
	}
	else // take oldest (should be first in list)
	{
		rbC = m_chuckedItems[0];
	}

	// Setup
	rbC->setPosition(startPos);
	rbC->setLinearVelocity( velocity );
	rbC->setMass( rbC->getMass() * m_options.m_Physics.m_massMultiplier );
	rbC->setLinearDamping(0.9f);
	rbC->setAngularDamping(0.9f);

	hkMatrix3 i; rbC->getRigidMotion()->getInertiaInvLocal(i);

	// Get the major component
	hkVector4 diag;
	diag.set( i(0,0), i(1,1) , i(2,2) );
	int maj = diag.getMajorAxis();

	// Find the minimum
	hkReal min = hkMath::min2( hkMath::min2( diag(0), diag(1) ) , diag(2) );

	// If they are very significantly different
	const hkReal maxRatio = 3.0f;
	if ( maxRatio* min < diag(maj))
	{
		// clamp to ratio
		diag.set(min, min, min);
		diag(maj) *= maxRatio;
	}

	// Set tensor
	i.setDiagonal(diag(0), diag(1), diag(2) );
	rbC->setInertiaInvLocal(i);

	rbC->getRigidMotion()->setDeactivationClass( hkpSolverInfo::DEACTIVATION_CLASS_AGGRESSIVE );

	if ( clonedItem )
	{
		// store so we can recycle it if we want
		m_chuckedItems.pushBack(rbC);

		m_world->addEntity(rbC);
	}
	else // make sure it is active
	{
		rbC->activate();

		// rotate array
		m_chuckedItems.removeAtAndCopy(0);
		m_chuckedItems.pushBack(rbC); // keep ref
	}
}

void Gdc2005Demo::fireShot( const hkVector4& start, const hkVector4& end, float strength)
{
	// ray - ragdoll intersection test:
	float closestHit = 1;
	int hitBodyIndex = -1;

	for (int ri=0; m_ragdollInstance && (ri < m_ragdollInstance->getNumBones()); ++ri)
	{
		hkpRigidBody* rb = m_ragdollInstance->getRigidBodyOfBone( ri);
		const hkpShape* s = rb->getCollidable()->getShape();

		hkpShapeRayCastInput input;
		hkpShapeRayCastOutput output;
		input.m_from.setTransformedInversePos(rb->getTransform(), start);
		input.m_to.setTransformedInversePos(rb->getTransform(), end);
		hkBool hit = s->castRay(input, output );
		if( hit && (output.m_hitFraction < closestHit) )
		{
			closestHit = output.m_hitFraction;
			hitBodyIndex = ri;
		}
	}

	if (hitBodyIndex >= 0)
	{
		hkVector4 intersectionPointWorld;
		intersectionPointWorld.setInterpolate4(start, end, closestHit);

		// apply an impulse at that point
		hkpRigidBody* rb = m_ragdollInstance->getRigidBodyOfBone ( hitBodyIndex );
		hkVector4 impl; impl.setSub4( end, start );
		impl.fastNormalize3();
		impl.mul4(strength);
		//rb->applyPointImpulse(impl, intersectionPointWorld);
		rb->applyLinearImpulse(impl);
	}
}

void Gdc2005Demo::updateFlashlight(bool on)
{
	hkgLightManager* lm = m_env->m_displayWorld->getLightManager();

	// the first light is the flashlight, the rest are oart of the scene
	int nl = lm->getNumLights();
	HK_ASSERT2(0x0, nl >= 0, " No lights!");

	if (on)
	{
		hkgLight* l = lm->getLight(0);
		float dir[3];
		m_env->m_window->getViewport(0)->getCamera()->getDir(dir);
		l->setDirection( dir );
		lm->enable(0);
	}
	else
	{
		lm->disable(0);
	}

	for (int i=1; i < nl; ++i )
	{
		if (on)
			lm->disable(i);
		else
			lm->enable(i);
	}

	lm->computeActiveSet(HKG_VEC3_ZERO);
}

void Gdc2005Demo::mouseDown()
{
	m_world->lock();

	hkVector4 rayStart;
	hkVector4 rayEnd;

	// unproject current 'mouse' pos.
	hkgCamera* cam = m_env->m_window->getViewport(0)->getCamera();
	int xPos = m_env->m_window->getMouse().getPosX();
	int yPos = m_env->m_window->getMouse().getPosY();

	int w = m_env->m_window->getWidth();
	int h = m_env->m_window->getHeight();
	cam->unProject( xPos, yPos, 0, w, h, (float*)&rayStart);
	cam->unProject( xPos, yPos, 1, w, h, (float*)&rayEnd);

	// fire ray into our world and see what we get

	hkpWorldRayCastInput input;
	input.m_from = rayStart;
	input.m_to = rayEnd;
	input.m_filterInfo = LAYER_PICKING;

	hkpClosestRayHitCollector output;
	m_world->castRay(input, output );

	if( output.hasHit() )
	{
		hkVector4 intersectionPointWorld;
		intersectionPointWorld.setInterpolate4( input.m_from, input.m_to, output.getHit().m_hitFraction );

		const hkpWorldRayCastOutput& hit = output.getHit();
		hkpRigidBody* rb = hkGetRigidBody( hit.m_rootCollidable );
		if( rb && !rb->isFixed() )
		{
			hkVector4 positionAinA;
			positionAinA.setTransformedInversePos( rb->getTransform(), intersectionPointWorld );

			const hkReal springDamping = 0.5f;
			const hkReal springElasticity = 0.3f;
			const hkReal objectDamping = 0.95f;
			m_mouseSpring = new hkpMouseSpringAction( positionAinA, intersectionPointWorld, springDamping, springElasticity, objectDamping, rb );

			// Modify mouse spring force . If we're grabbing the seesaw, make it less strong
			if( hkString::strNcasecmp(rb->getName(), "Box01", 5) == 0)
			{
				m_mouseSpring->setMaxRelativeForce(m_mouseSpringMaxRelativeForce * 0.1f);
			}
			else
			{
				m_mouseSpring->setMaxRelativeForce(m_mouseSpringMaxRelativeForce);
			}
			m_world->addAction( m_mouseSpring );

			// store the camera Z depth
			float result[3];
			cam->project( intersectionPointWorld(0), intersectionPointWorld(1), intersectionPointWorld(2), w, h, result );
			m_mousePosDepth = result[2];	// remember the z value

			m_mouseActive = true;
		}
	}

	m_world->unlock();
}

bool Gdc2005Demo::isChuckable(hkpEntity* e)
{
	return (m_chuckedItems.indexOf((hkpRigidBody*)e) >= 0);
}

void Gdc2005Demo::removeChuckable(hkpRigidBody* rb)
{
	int idx = m_chuckedItems.indexOf(rb);
	if (idx)
	{
		m_chuckedItems.removeAtAndCopy(idx);
		rb->removeReference(); // remove our ref (let it delete)
	}
	m_world->removeEntity(rb);
}

void Gdc2005Demo::quitLevel()
{
	for (int i=0; i < m_chuckables.getSize(); ++i)
	{
		m_chuckables[i].m_dispObj->removeReference();
		m_chuckables[i].m_rb->removeReference();
	}
	m_chuckables.setSize(0);

	for (int c=0; c < m_chuckedItems.getSize(); ++c)
	{
		m_chuckedItems[c]->removeReference();
	}
	m_chuckedItems.setSize(0);

	if (m_vertexColorDisplay)
	{
		m_vertexColorDisplay->release();
		m_vertexColorDisplay = HK_NULL;
	}
	if (m_lightmapDisplay)
	{
		m_lightmapDisplay->release();
		m_lightmapDisplay = HK_NULL;
	}
}



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
