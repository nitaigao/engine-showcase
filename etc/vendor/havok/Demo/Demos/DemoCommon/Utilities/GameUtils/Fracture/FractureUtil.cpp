/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>

#include <Common/Base/Algorithm/Sort/hkSort.h>
#include <Common/Base/Memory/Memory/hkMemory.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>
#include <Common/Base/Math/Vector/hkVector4Util.h>
#include <Common/Base/Types/Geometry/hkGeometry.h>
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>

#include <Common/Internal/ConvexHull/hkGeometryUtility.h>
#include <Common/Internal/ConvexHull/hkPlaneEquationUtil.h>

#include <Physics/Dynamics/Entity/hkpRigidBody.h>

#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>

#include <Demos/DemoCommon/Utilities/GameUtils/Fracture/FractureUtil.h>

#include <Demos/DemoCommon/Utilities/hkGeometrySplitter.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>


FractureUtil::ShapeInfo::ShapeInfo()
{
	m_shape = HK_NULL;
	m_geometry = HK_NULL;
}

hkResult HK_CALL FractureUtil::createShapeFromPlanes( const hkArray<hkVector4>& planes, hkReal minVolume, FractureUtil::ShapeInfo& shapeInfo)
{
	// 
	// Generate new shape/body and verify that it should be created (check it's volume, for instance).
	// 

	hkInplaceArrayAligned16<hkVector4,32> vertices;
	if (HK_SUCCESS != hkPlaneEquationUtil::getConvexVerticesFromPlaneEquations( planes, vertices ))
	{
		return HK_FAILURE;
	}


	//check validity of the result
#if defined(HK_DEBUG)
	{
		hkBool allOk = true;
		for (int v = 0; v < vertices.getSize(); v++)
		{
			for (int p = 0; p < planes.getSize(); p++)
			{
				hkBool dingo = hkReal(vertices[v].dot3(planes[p])) + planes[p](3) < 0.01f;
				allOk = allOk & dingo;
			}
		}
		if (!allOk)
		{
			HK_ASSERT2(0, 0, "Invalid convex piece generated");
		}
	}
#endif

	if (vertices.getSize() >= 4)
	{
		hkStridedVertices vertsIn;
		vertsIn.m_vertices = reinterpret_cast<hkReal*>(&vertices[0]);
		vertsIn.m_numVertices = vertices.getSize();
		vertsIn.m_striding = sizeof(hkVector4);

		// Must create new planes in new reference frame
		hkGeometry* geometry = new hkGeometry();
		hkInplaceArrayAligned16<hkVector4,32> transformedPlanes;

		hkGeometryUtility::createConvexGeometry(vertsIn, *geometry, transformedPlanes);

		hkReal mass = 1.0f;
		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeGeometryVolumeMassPropertiesChecked(geometry, mass, massProperties);
		if (massProperties.m_volume > minVolume) // Discard smaller pieces
		{
			hkpConvexVerticesShape* cvs = new hkpConvexVerticesShape(vertsIn, transformedPlanes, 0.0f);
			shapeInfo.m_shape = cvs;
			shapeInfo.m_volume = massProperties.m_volume;
			shapeInfo.m_centerOfMass = massProperties.m_centerOfMass;
			shapeInfo.m_intertiaTensor = massProperties.m_inertiaTensor;
			shapeInfo.m_geometry = geometry;

			return HK_SUCCESS;
		}
		delete geometry;
	}
	return HK_FAILURE;
}

#define NEW_SPLITTER
static inline void splitDisplayObjectWithPlanes(hkgDisplayObject* displayObject, hkgDisplayContext* displayContext, hkArray<hkVector4>& planes, int firstPlaneToUse, hkArray<hkgDisplayObject*>& displayObjectListToAppendOntoOut)
{
#if !defined NEW_SPLITTER
	HK_ASSERT2(0xad56565d, displayObject && displayContext, "internal error: display context not provided for splitting of a display object");

	hkgDisplayObject* oldObj = displayObject;
	hkgDisplayObject* newObj;

	HK_ASSERT2(0xad788855, firstPlaneToUse < planes.getSize(), "internal error: split planes are wrong");
	newObj = createSplitDisplayObject( oldObj, displayContext, planes[firstPlaneToUse] );
	oldObj = newObj;
	for (int p = firstPlaneToUse+1; p < planes.getSize(); p++)
	{
		newObj = createSplitDisplayObject( oldObj, displayContext, planes[p] );
		oldObj->release();
		oldObj = newObj;
	}
	displayObjectListToAppendOntoOut.pushBack(newObj);
#else

	// OR

	HK_ASSERT2(0xad788855, firstPlaneToUse < planes.getSize(), "internal error: split planes are wrong");

	// TODO -this may return HK_NULL if the assumptions regarding the display object arent met - should deal with this
	hkTempSplitGeom* oldObj = createTempFromDisplayGeom( displayObject ); // Note - we could cache this object, because it will be calculated several times for 1 fracture pattern
	hkTempSplitGeom* newObj;
	newObj = createTempSplitDisplayObject( oldObj, planes[firstPlaneToUse] );

	if (newObj != oldObj)
	{
		oldObj->removeReference();
		oldObj = newObj;
	}

	for (int p = firstPlaneToUse+1; p < planes.getSize(); p++)
	{
		newObj = createTempSplitDisplayObject( oldObj, planes[p] );
		if (newObj != oldObj)
		{
			oldObj->removeReference();
			oldObj = newObj;
		}
	}

	hkgDisplayObject* newDispObj = createDisplayGeomFromTemp( displayObject, newObj, displayContext );
	newObj->removeReference();

	displayObjectListToAppendOntoOut.pushBack(newDispObj);

#endif
}

void HK_CALL FractureUtil::fractureConcrete(const ConcreteFractureInput& in, ConcreteFractureOutput& out)
{
	hkpShapeType shapeType = in.m_shape->getType();
	HK_ASSERT2(0xad56bc4d, shapeType == HK_SHAPE_CONVEX_VERTICES || shapeType== HK_SHAPE_BOX, "Fracture supports only hkpConvexVerticesShape and hkpBoxShape.");
	HK_ASSERT2(0xadbbe2ee, !out.m_shapes.getSize() && !out.m_bulletHoleDebrisShapes.getSize(), "Out array must be empty.");
	HK_ASSERT2(0xad6b5d44, in.m_depth > 0.0f, "Ground fracture supports positive depth values only.");
	HK_ASSERT2(0xad34eb33, in.m_innerRadius <= in.m_outerRadius, "Outer radius must be greater than inner radius.");
	HK_ASSERT2(0xad34eb34, in.m_numSideFacets >= 2, "Number of side facets must be >= 2.");

	hkInplaceArray<hkVector4,32> planes; 

	if ( shapeType != HK_SHAPE_BOX )
	{
		const hkpConvexVerticesShape* cvs = static_cast<const hkpConvexVerticesShape*>(in.m_shape);
		planes.insertAt(0, cvs->getPlaneEquations().begin(), cvs->getPlaneEquations().getSize());
	}
	else
	{
		planes.setSizeUnchecked(6);
		const hkpBoxShape* box = static_cast<const hkpBoxShape*>(in.m_shape);
		const hkVector4& he = box->getHalfExtents();
		planes[0].set( 1, 0, 0,-he(0));
		planes[1].set(-1, 0, 0,-he(0));
		planes[2].set( 0, 1, 0,-he(1));
		planes[3].set( 0,-1, 0,-he(1));
		planes[4].set( 0, 0, 1,-he(2));
		planes[5].set( 0, 0,-1,-he(2));
	}


	hkInplaceArray<hkVector4, 16> centerPiecePlanes;

	hkVector4 normal = in.m_fractureOrigin.getNormal();
	hkVector4 contactPoint = in.m_fractureOrigin.getPosition();

	hkVector4 tangent; hkVector4Util::calculatePerpendicularVector(normal, tangent); tangent.fastNormalize3();

	// Calculate side facet parameters from input
	hkVector4 facetNormal;
	hkVector4 centerPieceNormal;
	hkReal facetDistanceFromCentrePoint;
	{
		hkVector4 tangent2; tangent2.setCross(normal, tangent);
		facetNormal.setMul4(in.m_outerRadius - in.m_innerRadius, normal);
		facetNormal.addMul4(-in.m_depth, tangent2);
		facetNormal.fastNormalize3();

		centerPieceNormal.setMul4(-1.0f, tangent2);

		tangent2.mul4(in.m_outerRadius);
		facetDistanceFromCentrePoint = facetNormal.dot3(tangent2);

		// This doesn't yield the exact fraction pattern, 
		// because the contactPoint is not on the body's surface.
	}

	hkQuaternion halfUnitRotateAroundNormal(normal, 2.0f * HK_REAL_PI / hkReal(in.m_numSideFacets) * 0.5f );
	int originalNumPlanes = planes.getSize();

	// Cork (middle part) top plane
	hkVector4 corkTopPlane = normal; 
	corkTopPlane(3) = -hkReal(corkTopPlane.dot3(contactPoint)) + in.m_depth;
	corkTopPlane.setNeg4(corkTopPlane);


	// Generate broken parts:
	hkQuaternion rotationAroundNormal; rotationAroundNormal.setIdentity();
	if (in.m_randomizeOrientationAroundNormal)
	{
		hkReal angle = in.m_generator->getRandRange(0.0f, 2.0f * HK_REAL_PI / hkReal(in.m_numSideFacets));
		rotationAroundNormal.setAxisAngle(normal, angle);
	}

	for (int i = 0; i < in.m_numSideFacets; i++)
	{
		// Add extra constraining planes.
		{
			// Pie-chart plane 1
			hkVector4& plane1 = planes.expandOne();
			// use initial rotation
			plane1.setRotatedDir(rotationAroundNormal, tangent);
			plane1(3) = -plane1.dot3(contactPoint);
		}
		{
			// Bullet hole center plane
			hkVector4 plane3;
			rotationAroundNormal.setMul(rotationAroundNormal, halfUnitRotateAroundNormal); // in-out var
			plane3.setRotatedDir(rotationAroundNormal, facetNormal);
			plane3(3) = -hkReal(plane3.dot3(contactPoint)) - facetDistanceFromCentrePoint;
			planes.pushBack(plane3);

			hkVector4 plane4;
			plane4.setRotatedDir(rotationAroundNormal, centerPieceNormal);
			plane4(3) = -hkReal(plane4.dot3(contactPoint)) - (-in.m_innerRadius);
			planes.pushBack(plane4);

			plane4.setNeg4(plane4);
			centerPiecePlanes.pushBack(plane4);
		}
		{
			// Pie-chart plane 2
			hkVector4 plane2;
			rotationAroundNormal.setMul(rotationAroundNormal, halfUnitRotateAroundNormal); // in-out var
			plane2.setRotatedDir(rotationAroundNormal, tangent);
			plane2.setNeg4(plane2);
			plane2(3) = -plane2.dot3(contactPoint);
			planes.pushBack(plane2);
		}


		ShapeInfo shapeInfo;
		if (HK_SUCCESS == createShapeFromPlanes(planes, in.m_minVolumeOfGeneratedShapes, shapeInfo))
		{
			out.m_shapes.pushBack(shapeInfo);

			if (in.m_generateDisplayObjects)
			{
				splitDisplayObjectWithPlanes(in.m_displayObject, in.m_displayContext, planes, originalNumPlanes, out.m_displayObjects);
			}
		}

		if (in.m_generateBulletHoleDebris)
		{
			// Flip side faced plane normal
			hkVector4& facetPlane = planes[planes.getSize()-3];
			facetPlane.setNeg4(facetPlane);

			// Remove centerPiecePlane
			planes.removeAt(planes.getSize()-2);

			// Add corkTop plane
			planes.pushBack(corkTopPlane);

			if (HK_SUCCESS == createShapeFromPlanes(planes, in.m_minVolumeOfGeneratedShapes, shapeInfo))
			{
				out.m_bulletHoleDebrisShapes.pushBack(shapeInfo); // debris.

				if (in.m_generateDisplayObjectsForDebris)
				{
					splitDisplayObjectWithPlanes(in.m_displayObject, in.m_displayContext, planes, originalNumPlanes, out.m_bulletHoleDebrisDisplayObjects);
				}
			}

		}

		// Reset plane array to the set of initial planes.
		planes.setSize(originalNumPlanes); 
	}

	if (in.m_generateCenterPiece)
	{
		// Create the center 'cork' shape
		planes.insertAt(planes.getSize(), centerPiecePlanes.begin(), centerPiecePlanes.getSize() );
		corkTopPlane.setNeg4(corkTopPlane);
		planes.pushBack(corkTopPlane);

		ShapeInfo shapeInfo;
		if (HK_SUCCESS == createShapeFromPlanes(planes, in.m_minVolumeOfGeneratedShapes, shapeInfo))
		{
			out.m_shapes.pushBack(shapeInfo);
		}
	}

}

void HK_CALL FractureUtil::fractureGlass(const GlassFractureInput& in, GlassFractureOutput& out)
{
	hkpShapeType shapeType = in.m_shape->getType();
	HK_ASSERT2(0xadbbe2ee, !out.m_innerShapes.getSize() && !out.m_outerShapes.getSize(), "Out array must be empty.");
	HK_ASSERT2(0xad56bc4d, shapeType == HK_SHAPE_CONVEX_VERTICES || shapeType== HK_SHAPE_BOX, "Fracture supports only hkpConvexVerticesShape and hkpBoxShape.");
	HK_ASSERT2(0xad54333, in.m_numConcentricPieces >= 1, "Must have at lease one concentric fracture circle.");

	hkInplaceArray<hkVector4,32> planes; 

	if ( shapeType != HK_SHAPE_BOX )
	{
		const hkpConvexVerticesShape* cvs = static_cast<const hkpConvexVerticesShape*>(in.m_shape);
		planes.insertAt(0, cvs->getPlaneEquations().begin(), cvs->getPlaneEquations().getSize() );
	}
	else
	{
		planes.setSizeUnchecked(6);
		const hkpBoxShape* box = static_cast<const hkpBoxShape*>(in.m_shape);
		const hkVector4& he = box->getHalfExtents();
		planes[0].set( 1, 0, 0,-he(0));
		planes[1].set(-1, 0, 0,-he(0));
		planes[2].set( 0, 1, 0,-he(1));
		planes[3].set( 0,-1, 0,-he(1));
		planes[4].set( 0, 0, 1,-he(2));
		planes[5].set( 0, 0,-1,-he(2));
	}


		// get and randomize contact point
	hkVector4 contactPoint = in.m_fractureOrigin.getPosition();
	hkVector4 normal       = in.m_fractureOrigin.getNormal();
	if ( in.m_randomizeNormalAngle > 0 )
	{
		hkReal d = in.m_randomizeNormalAngle * 0.5f;
		normal(0) += in.m_generator->getRandRange( -d, d );
		normal(1) += in.m_generator->getRandRange( -d, d );
		normal(2) += in.m_generator->getRandRange( -d, d );
		normal.normalize3();
	}

		// Calculate a vector on the surface of the glass
	hkVector4 tangentFromFracturePoint;
	{
		hkVector4Util::calculatePerpendicularVector(normal, tangentFromFracturePoint); 
		tangentFromFracturePoint.normalize3();
	}

	hkReal deltaRadius = (in.m_numConcentricPieces - 1) ? (in.m_concentricFractureMaxRadius - in.m_holeRadius) / (in.m_numConcentricPieces - 1) : 0.0f;

	hkInplaceArray<hkVector4, 16> referencePointArrays[2];
	hkArray<hkVector4>* referencePoints[2] = { &referencePointArrays[0], &referencePointArrays[1] };
	hkInplaceArray<hkQuaternion, 16> quaternions;

	referencePoints[0]->setSize(in.m_numPieSlices);
	referencePoints[1]->setSize(in.m_numPieSlices);
	quaternions.setSize(in.m_numPieSlices);

		//
		//	build an initial ring of directions and positions
		//
	{
		hkQuaternion pieSliceAngle(normal, 2.0f * HK_REAL_PI / hkReal(in.m_numPieSlices));

		// Use random initial angle
		hkReal baseAngle = in.m_generator->getRandRange(0, HK_REAL_PI);
		hkQuaternion basePieSliceQuat(normal, baseAngle);

		for (int slice = 0; slice < in.m_numPieSlices; slice++)
		{
			hkReal randAngle = in.m_generator->getRandRange(-in.m_pieSliceCurvature, in.m_pieSliceCurvature);
			hkQuaternion randQuat(normal, randAngle);
			hkQuaternion quat; quat.setMul(basePieSliceQuat, randQuat);

			quaternions[slice] = quat;

			hkReal dist = in.m_holeRadius + in.m_generator->getRandRange(-0.5f * deltaRadius, 0.5f * deltaRadius);
			hkVector4 dir; dir.setRotatedDir(quaternions[slice], tangentFromFracturePoint);

			(*referencePoints[1])[slice].setAddMul4(in.m_fractureOrigin.getPosition(), dir, dist);

			basePieSliceQuat.mul(pieSliceAngle);
		}
	}

	int originalNumPlanes = planes.getSize();

	hkBool prevShapeCreated = false;

	for (int circle = 0; circle <= in.m_numConcentricPieces; circle++)
	{
		hkAlgorithm::swap(referencePoints[0], referencePoints[1]);

		{
			for (int slice = 0; slice < in.m_numPieSlices; slice++)
			{
				// Randomize crack direction
				hkReal randAngle = in.m_generator->getRandRange(-in.m_pieSliceCurvature, in.m_pieSliceCurvature);
				hkQuaternion randQuat(normal, randAngle);
				quaternions[slice].setMul(quaternions[slice], randQuat);

				hkReal dist = deltaRadius + in.m_generator->getRandRange(-0.3f * deltaRadius, 0.3f * deltaRadius);
				hkVector4 dir; dir.setRotatedDir(quaternions[slice], tangentFromFracturePoint);

				(*referencePoints[1])[slice].setAddMul4((*referencePoints[0])[slice], dir, dist);
			}
		}

		{
			for (int slice = 0; slice < in.m_numPieSlices; slice++)
			{
				// Reset planes array to the original set.
				planes.setSize(originalNumPlanes);

				// Got four reference points ordered counterclockwise
				hkVector4  points[] = { (*referencePoints[0])[slice], 
									  (*referencePoints[0])[(slice+1)%in.m_numPieSlices], 
									  (*referencePoints[1])[(slice+1)%in.m_numPieSlices], 
									  (*referencePoints[1])[slice]
									};

				for (int p = 0; p < 4; p++)
				{
					// Skip the third (the one furthest from the center) bounding plane for external pieces.
					if (circle == in.m_numConcentricPieces && p == 2)
					{
						p = 3;
					}

					hkVector4 pointDiff; pointDiff.setSub4(points[(p+1)%4], points[p]);

					if (hkReal(pointDiff.lengthSquared3()) > 0.0001f)
					{
						pointDiff.normalize3();
						hkVector4 planeNormal; 

						planeNormal.setCross(normal, pointDiff);
						planeNormal(3) = -planeNormal.dot3(points[p]);
						planes.pushBack(planeNormal);
					}
				}

				ShapeInfo shapeInfo;
				prevShapeCreated = false;
				if ( HK_SUCCESS != FractureUtil::createShapeFromPlanes(planes, in.m_minVolumeOfGeneratedShapes, shapeInfo) )
				{
					continue;
				}

				if (circle < in.m_numConcentricPieces)
				{
					out.m_innerShapes.pushBack(shapeInfo);

					if (in.m_generateDisplayObjects)
					{
						splitDisplayObjectWithPlanes(in.m_displayObject, in.m_displayContext, planes, originalNumPlanes, out.m_innerShapesDisplayObjects);
					}

				}
				else 
				{
					out.m_outerShapes.pushBack(shapeInfo);
					prevShapeCreated = true;

					if (in.m_generateDisplayObjects)
					{
						splitDisplayObjectWithPlanes(in.m_displayObject, in.m_displayContext, planes, originalNumPlanes, out.m_outerShapesDisplayObjects);
					}
				}

				//	if (prevShapeCreated)
				//	{
				//		// Add glue point between the last and the previous shape
				//		int size = out.m_outerShapes.getSize();
				//		IndexPair& ip = out.m_outerShapesGluePoints.expandOne();
				//      out.m_outerShapesGluePoints.pushBack( IndexPair(size-2, size-1) );
				//	}
			}
		}
	}
}


void HK_CALL FractureUtil::convertConvexVerticesShapeToPrincipalAxesFrame(ShapeInfo& shapeInfo, hkTransform& principalAxesTransform)
{
	// for a convex object: 
	HK_ASSERT2(0XADBDDF3, shapeInfo.m_shape->getType() == HK_SHAPE_CONVEX_VERTICES, "This code can only process hkConvexVeritcesShapes");

	hkpConvexVerticesShape* cvs = static_cast<hkpConvexVerticesShape*>(shapeInfo.m_shape);

	hkInplaceArray<hkVector4,64> vertices;
	cvs->getOriginalVertices(vertices);

	// Calculate proper local frame for the new piece
	{
		principalAxesTransform.setTranslation( shapeInfo.m_centerOfMass );

		hkpInertiaTensorComputer::convertInertiaTensorToPrincipleAxis( shapeInfo.m_intertiaTensor, principalAxesTransform.getRotation() );

		shapeInfo.m_centerOfMass.setZero4();
	}

	hkTransform principalAxesTransformInv; principalAxesTransformInv.setInverse(principalAxesTransform);

		//
		// Change original vertices information and geometry
		//
	{
		hkVector4Util::transformPoints(principalAxesTransformInv, vertices.begin(), vertices.getSize(), vertices.begin());
		cvs->copyVertexData(&vertices[0](0), sizeof(hkVector4), vertices.getSize());

		hkVector4* verts = shapeInfo.m_geometry->m_vertices.begin();
		int size = shapeInfo.m_geometry->m_vertices.getSize();
		hkVector4Util::transformPoints(principalAxesTransformInv, verts, size, verts);
	}

		//
		// Transform original plane equations
		//
	{
		hkArray<hkVector4>& planes = const_cast<hkArray<hkVector4>&>(cvs->getPlaneEquations());
		if ( planes.getSize() )
		{
			hkVector4Util::transformPlaneEquations( principalAxesTransformInv, planes.begin(), planes.getSize(), planes.begin() );
		}
	}


	//
	//	clamp inertia tensor
	//
	{
		hkMatrix3& m = shapeInfo.m_intertiaTensor;
		hkReal maxVal = hkMath::max2( m(0,0), hkMath::max2( m(1,1), m(2,2) ));
		hkReal minVal = 0.02f * maxVal;
		m(0,0) = hkMath::max2( minVal, m(0,0) );
		m(1,1) = hkMath::max2( minVal, m(1,1) );
		m(2,2) = hkMath::max2( minVal, m(2,2) );
	}
}


void HK_CALL FractureUtil::createRigidBodiesFromShapes( const hkpRigidBodyCinfo& templateCinfo, hkReal originalVolume, ExportGeometryAsProperty exportGeometry, hkArray<ShapeInfo>& shapeInfos, hkArray<hkpRigidBody*>& bodies, hkArray<hkTransform>* toPrincipalAxesTransformInOldLocalRefernceFrame)
{
	hkpRigidBodyCinfo info = templateCinfo;
	hkTransform templateTransform(templateCinfo.m_rotation, templateCinfo.m_position);

	for (int i = 0; i < shapeInfos.getSize(); i++)
	{
		FractureUtil::ShapeInfo& shapeInfo = shapeInfos[i];

		hkTransform principalAxesTransform;
		FractureUtil::convertConvexVerticesShapeToPrincipalAxesFrame(shapeInfo, principalAxesTransform);

		info.m_shape = shapeInfo.m_shape;
		info.m_centerOfMass = shapeInfo.m_centerOfMass;
		info.m_mass = templateCinfo.m_mass * shapeInfo.m_volume / originalVolume;
		info.m_inertiaTensor.setMul( info.m_mass, shapeInfo.m_intertiaTensor );


	HK_ASSERT(0x7708edc8,  shapeInfo.m_intertiaTensor(0,0) >0 );
	HK_ASSERT(0x1ff66a9d,  shapeInfo.m_intertiaTensor(1,1) >0 );
	HK_ASSERT(0x51ff9422,  shapeInfo.m_intertiaTensor(2,2) >0 );


		// Change position, orientation, linVelocity, and angVelocity information
		{
			info.m_position.setTransformedPos( templateTransform, principalAxesTransform.getTranslation());
			info.m_rotation.setMul ( templateCinfo.m_rotation,    hkQuaternion(principalAxesTransform.getRotation()) );

			hkVector4 templateMassCentreW; 
			templateMassCentreW.setTransformedPos(templateTransform, templateCinfo.m_centerOfMass);
			hkVector4 relPos; relPos.setSub4( info.m_position,  templateMassCentreW );
			hkVector4 pointVelocity;
			pointVelocity.setCross( templateCinfo.m_angularVelocity, relPos);
			pointVelocity.add4( templateCinfo.m_linearVelocity );

			// angular velocity unchanged

		}
		hkReferencedObject::lockAll();
		hkpRigidBody* body = new hkpRigidBody(info);
		hkReferencedObject::unlockAll();
		bodies.pushBack( body );

		body->addProperty( HK_PROPERTY_RIGID_BODY_VOLUME, shapeInfo.m_volume);

		// exportGeometry
		if ( exportGeometry == EXPORT_GEOMETRY_IN_PROPERTY )
		{
			body->addProperty( HK_PROPERTY_OVERRIDE_DEBUG_DISPLAY_GEOMETRY, shapeInfo.m_geometry );
		}
		else
		{
			delete shapeInfo.m_geometry;
		}
		shapeInfo.m_geometry = HK_NULL;

		if (toPrincipalAxesTransformInOldLocalRefernceFrame)
		{
			hkTransform& transform = toPrincipalAxesTransformInOldLocalRefernceFrame->expandOne();
			transform = principalAxesTransform;
		}
	}
}

void FractureUtil::GlassFractureOutput::removeReferencesToShapes()
{
	//
	// Remove shape references.
	//
	hkReferencedObject::lockAll();
	{
		for (int i = 0; i < m_innerShapes.getSize(); i++)
		{
			m_innerShapes[i].m_shape->removeReference();
			if ( m_innerShapes[i].m_geometry )
			{
				delete m_innerShapes[i].m_geometry;
			}
		}
	}
	{
		for (int i = 0; i < m_outerShapes.getSize(); i++)
		{
			m_outerShapes[i].m_shape->removeReference();
			if ( m_outerShapes[i].m_geometry )
			{
				delete m_outerShapes[i].m_geometry;
			}
		}
	}
	hkReferencedObject::unlockAll();
	m_innerShapes.clear();
	m_outerShapes.clear();
}


void FractureUtil::ConcreteFractureOutput::removeReferencesToShapes()
{
	//
	// Remove shape references.
	//
	hkReferencedObject::lockAll();
	{
		for (int i = 0; i < m_shapes.getSize(); i++)
		{
			m_shapes[i].m_shape->removeReference();
			if ( m_shapes[i].m_geometry )
			{
				delete m_shapes[i].m_geometry;
			}
		}
	}
	{
		for (int i = 0; i < m_bulletHoleDebrisShapes.getSize(); i++)
		{
			m_bulletHoleDebrisShapes[i].m_shape->removeReference();
			if ( m_bulletHoleDebrisShapes[i].m_geometry )
			{
				delete m_bulletHoleDebrisShapes[i].m_geometry;
			}
		}
	}
	hkReferencedObject::unlockAll();
	m_shapes.clear();
	m_bulletHoleDebrisShapes.clear();
}



hkReal FractureUtil::calcShapeVolume( const hkpConvexShape* shape )
{
	if ( shape->getType() == HK_SHAPE_BOX )
	{
		const hkVector4& he = static_cast<const hkpBoxShape*>( shape )->getHalfExtents();
		return 8.0f * he(0)* he(1) * he(2);
	}
	HK_ASSERT( 0xf0ef3212, shape->getType() == HK_SHAPE_CONVEX_VERTICES );

	const hkpConvexVerticesShape* cvs = static_cast<const hkpConvexVerticesShape*>( shape );
	hkInplaceArray<hkVector4,64> vertices;
	cvs->getOriginalVertices(vertices);

	hkStridedVertices vertsIn;
	vertsIn.m_vertices = reinterpret_cast<hkReal*>(&vertices[0]);
	vertsIn.m_numVertices = vertices.getSize();
	vertsIn.m_striding = sizeof(hkVector4);

	// Must create new planes in new reference frame
	hkGeometry geometry;
	hkInplaceArrayAligned16<hkVector4,32> transformedPlanes;

	hkGeometryUtility::createConvexGeometry(vertsIn, geometry, transformedPlanes);

	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::computeGeometryVolumeMassPropertiesChecked(&geometry, 1.0f, massProperties);
	return massProperties.m_volume;
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
