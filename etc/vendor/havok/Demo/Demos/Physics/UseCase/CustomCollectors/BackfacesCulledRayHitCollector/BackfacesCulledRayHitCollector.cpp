/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Physics/Collide/hkpCollide.h>
#include <Demos/Physics/UseCase/CustomCollectors/BackfacesCulledRayHitCollector/BackfacesCulledRayHitCollector.h>
#include <Physics/Collide/Util/hkpTriangleUtil.h>
#include <Physics/Collide/Shape/hkpShapeContainer.h>
#include <Physics/Collide/Shape/Convex/Triangle/hkpTriangleShape.h>
#include <Common/Visualize/hkDebugDisplay.h>

void BackfacesCulledRayHitCollector::addRayHit( const hkpCdBody& cdBody, const hkpShapeRayCastCollectorOutput& hitInfo )
{
	// Check for backfacing triangles
	if (cdBody.getShape()->getType() == HK_SHAPE_TRIANGLE ) {
		const hkpTriangleShape* triangle = reinterpret_cast<const hkpTriangleShape*>( cdBody.getShape() );

		// Calculate the triangle normal to be compared with the hit normal
		hkVector4 triangleNormal;
		hkpTriangleUtil::calcNormal(triangleNormal,
			triangle->getVertex(0), triangle->getVertex(1), triangle->getVertex(2));
		triangleNormal.normalize3();

		// Get the hit normal and rotate it to the same space as triangleNormal
		const hkRotation rootCollidableRotation = cdBody.getRootCollidable()->getTransform().getRotation();
		hkVector4 invertTransformedHitNormal;
		invertTransformedHitNormal.setRotatedInverseDir(rootCollidableRotation, hitInfo.m_normal);

		// Bail out if the hit normal is facing the opposite direction to the triangle normal
		if (triangleNormal.dot3(invertTransformedHitNormal) < 0.0f) return;
	}

	hkpWorldRayCastOutput& hit = m_hits.expandOne();
	static_cast<hkpShapeRayCastCollectorOutput&>( hit ) = hitInfo;
	shapeKeysFromCdBody( hit.m_shapeKeys, hkpShapeRayCastOutput::MAX_HIERARCHY_DEPTH, cdBody );
	hit.m_rootCollidable = cdBody.getRootCollidable();
}

void BackfacesCulledRayHitCollector::sortHits( ) 
{
	hkSort( m_hits.begin(), m_hits.getSize() );
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
