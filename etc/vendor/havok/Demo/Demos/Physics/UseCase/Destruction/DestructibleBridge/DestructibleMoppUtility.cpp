/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/UseCase/Destruction/DestructibleBridge/DestructibleMoppUtility.h>

#include <Physics/Collide/Agent/hkpProcessCollisionInput.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>
#include <Physics/Collide/Dispatch/hkpCollisionDispatcher.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>
#include <Physics/Collide/Shape/Convex/ConvexTransform/hkpConvexTransformShape.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/Modifiers/hkpRemoveTerminalsMoppModifier.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics/Collide/Shape/Misc/Transform/hkpTransformShape.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>



#define MIN_DISTANCE_FOR_NEW_ORIGIN			100.0f
#define MIN_DEVIATION_FOR_NEW_OFFSET_SHAPE	  0.02f

void DestructibleMoppUtility::collectAllConvexChildShapesWithinRadius( const hkpBvTreeShape* treeShape, hkVector4Parameter sphereCenterShapeSpace, hkReal radius, const hkpCollisionInput& collisionInput,
																	   hkArray<ConvexShapeData>& shapesOut, hkArray<hkpShapeKey>& keys )
{
	//hkVector4 sphereCenterShapeSpace; sphereCenterShapeSpace.setTransformedInversePos(m_rigidBody->getTransform(), event.m_contactPoint->getPosition());

	//
	// collect the keys for all shapes that are within a sphere-radius-sized cube around the sphere's center
	// note: we are using an AABB query as this is faster than a sphere query
	//
	{
		hkAabb aabb;
		{
			hkVector4 radius4; radius4.setAll(radius);
			aabb.m_max.setAdd4( sphereCenterShapeSpace, radius4 );
			aabb.m_min.setSub4( sphereCenterShapeSpace, radius4 );
		}
		treeShape->queryAabb(aabb, keys);
	}

	hkTransform sphereTransform;
	sphereTransform.getRotation().setIdentity();
	sphereTransform.setTranslation(sphereCenterShapeSpace);
	hkpSphereShape sphereShape( 0.0f );
	hkpCollidable  sphereCollidable(&sphereShape, &sphereTransform);

	{
		hkpClosestCdPointCollector collector;
		const hkpShapeContainer* container = treeShape->getContainer();

		hkpCollisionInput input = collisionInput;
		input.m_tolerance = radius;

		for (int i = 0; i < keys.getSize(); i ++ )
		{
			hkpShapeKey shapeKey = keys[i];

			hkpShapeCollection::ShapeBuffer buffer;
			const hkpShape* shape = container->getChildShape(shapeKey, buffer);

			// ignore all mesh-related and non-convex shapes
			hkBool32 isConvex = input.m_dispatcher->hasAlternateType( shape->getType(), HK_SHAPE_CONVEX );
			if ( shape->getType() == HK_SHAPE_TRIANGLE || !isConvex )
			{
				keys.removeAt(i);
				i--;
				continue;
			}

			{
				hkpCollidable  shapeCollidable(shape, &hkTransform::getIdentity() );

				hkpShapeType a = sphereShape.getType();
				hkpShapeType b = shape->getType();

				collector.reset();

				hkpCollisionDispatcher::GetClosestPointsFunc getClosestPoints = input.m_dispatcher->getGetClosestPointsFunc( a, b );
				getClosestPoints( sphereCollidable, shapeCollidable, input, collector );

				if ( !collector.hasHit() || collector.getHitContact().getDistance() > radius  )
				{
					keys.removeAt(i);
					i--;
					continue;
				}
			}
			hkTransform transform;
			transform.setIdentity();

			while ( shape->getType() == HK_SHAPE_CONVEX_TRANSLATE || shape->getType() == HK_SHAPE_CONVEX_TRANSFORM || shape->getType() == HK_SHAPE_TRANSFORM )
			{
				switch ( shape->getType() )
				{
				case HK_SHAPE_CONVEX_TRANSLATE:
					{
						const hkpConvexTranslateShape* cts = static_cast<const hkpConvexTranslateShape*>( shape );
						transform.getTranslation().add4( cts->getTranslation() );
						shape = cts->getChildShape();
						break;
					}

				case HK_SHAPE_CONVEX_TRANSFORM:
					{
						const hkpConvexTransformShape* cts = static_cast<const hkpConvexTransformShape*>( shape );
						hkTransform oldTransform = transform;
						transform.setMul(oldTransform, cts->getTransform());
						shape = cts->getChildShape();
						break;
					}
				case HK_SHAPE_TRANSFORM:
					{
						const hkpTransformShape* ts = static_cast<const hkpTransformShape*>( shape );
						hkTransform oldTransform = transform;
						transform.setMul(oldTransform, ts->getTransform());
						shape = ts->getChildShape();
						break;
					}
				default:
					break;
				}
			}
				// remove shapes sitting in the buffer
			if ( (void*)shape == (void*)buffer )
			{
				keys.removeAt(i);
				i--;
				continue;
			}

			ConvexShapeData& sad = shapesOut.expandOne();
			sad.m_shape = reinterpret_cast<const hkpConvexShape*>(shape);
			sad.m_transform = transform;
			sad.m_contactDetails = collector.getHitContact();
		}
	}
}



void DestructibleMoppUtility::removeKeysFromMopp( const hkpMoppBvTreeShape* moppShape, hkVector4Parameter sphereCenterShapeSpace, hkReal radius, const hkArray<hkpShapeKey>& keys )
{
	hkAabb aabb;
	{
		hkVector4 radius4; radius4.setAll(radius);
		aabb.m_max.setAdd4( sphereCenterShapeSpace, radius4 );
		aabb.m_min.setSub4( sphereCenterShapeSpace, radius4 );
	}

	//
	// remove all broken off shapes from MOPP in one go
	//
	{
		hkpRemoveTerminalsMoppModifier modifier(moppShape->getMoppCode(), aabb, keys);
		modifier.applyRemoveTerminals( const_cast<hkpMoppCode*>(moppShape->getMoppCode()) );
	}
}



void DestructibleMoppUtility::mergeShapes(hkArray<ConvexShapeData>& shapeDataArray, ShapeData& shapeOut )
{
	shapeOut.m_shape = HK_NULL;

	if ( shapeDataArray.getSize() == 0 )
	{
		return;
	}

	hkTransform origin;
	origin.getTranslation().setZero4();
	origin.getRotation().setIdentity();

	hkArray<const hkpShape*> shapesForConvexList;

	hkReferencedObject::lockAll();
	{
		for (int i = shapeDataArray.getSize()-1; i >= 0; i--)
		{
			const ConvexShapeData*	shapeData	= &shapeDataArray[i];
			const hkpConvexShape*	shape		= shapeData->m_shape;
			const hkTransform*		transform	= &shapeData->m_transform;

			if ( i == 0 && transform->getTranslation().lengthSquared3() > (MIN_DISTANCE_FOR_NEW_ORIGIN*MIN_DISTANCE_FOR_NEW_ORIGIN) )
			{
				// if the first shape is far off (and thus all other shapes are, too, as we are dealing with a group of closely located shapes)
				// we will use this transform as the new origin for the newly created entity.
				// we also do not need a transform/translate shape in this case.
				origin = *transform;
				shape->addReference();
				shapesForConvexList.pushBack(shape);
			}
			else
			{
				if ( transform->isApproximatelyEqual(origin, MIN_DEVIATION_FOR_NEW_OFFSET_SHAPE) )
				{
					// shape's translation and rotation is so close to the new object's origin that we can omit any additional translate/transform shape in between
					shapesForConvexList.pushBack(shape);
					shape->addReference();
				}
				else
				{
					if ( transform->getRotation().isApproximatelyEqual(origin.getRotation(), MIN_DEVIATION_FOR_NEW_OFFSET_SHAPE) )
					{
						// if the shape's rotation is very close to the origin's rotation, we only need to use a hkpConvexTranslateShape
						hkVector4 offset; offset.setSub4( transform->getTranslation(), origin.getTranslation() );
						hkpConvexTranslateShape* translateShape = new hkpConvexTranslateShape(shape, offset);
						shapesForConvexList.pushBack(translateShape);
					}
					else
					{
						// shape's rotation is different to origin's rotation, so we need to use a hkpConvexTransformShape
						hkTransform offset; offset.setMulInverseMul(origin, *transform);
						hkpConvexTransformShape* transformShape = new hkpConvexTransformShape(shape, offset);
						shapesForConvexList.pushBack(transformShape);
					}
				}

			}
		}
	}

	hkpMoppBvTreeShape* moppShape;
	{
		hkpListShape* convexListShape = new hkpListShape( (const hkpShape*const*)shapesForConvexList.begin(), shapesForConvexList.getSize());

		{
			for ( int i=0; i<shapesForConvexList.getSize(); i++ )
			{
				shapesForConvexList[i]->removeReference();
			}
			shapesForConvexList.clear();
		}

		hkpMoppCompilerInput mci;
		mci.m_enableInterleavedBuilding = false;
		hkpMoppCode* code = hkpMoppUtility::buildCode( convexListShape , mci);
		moppShape = new hkpMoppBvTreeShape(convexListShape, code);
		code->removeReference();
		convexListShape->removeReference();
	}
	hkReferencedObject::unlockAll();

	shapeOut.m_shape    = moppShape;
	shapeOut.m_transform = origin;
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
