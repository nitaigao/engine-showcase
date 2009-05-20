/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>

#include <Demos/DemoCommon/Utilities/KdTree/KdTreeDebugUtils.h>

#include <Common/Visualize/hkDebugDisplay.h>
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>

// Raycasting
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/Collector/KdTreeCollector/hkpKdTreeCollidableRaycaster.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhase.h>



static void drawAabb(hkgDisplayContext* context, const hkVector4& mini, const hkVector4& maxi, int color)
{
	HKG_ENABLED_STATE restoreState = context->getEnabledState();

	hkVector4 verts[8];
	verts[0].set( mini(0), mini(1), mini(2) );
	verts[1].set( mini(0), maxi(1), mini(2) );
	verts[2].set( mini(0), mini(1), maxi(2) );
	verts[3].set( mini(0), maxi(1), maxi(2) );

	verts[4].set( maxi(0), mini(1), mini(2) );
	verts[5].set( maxi(0), maxi(1), mini(2) );
	verts[6].set( maxi(0), mini(1), maxi(2) );
	verts[7].set( maxi(0), maxi(1), maxi(2) );

	context->setCurrentColorPacked( 0x1f7f7f10 );
	context->setBlendState( true );
	context->setCullFaceState( false );
	context->setBlendMode( HKG_BLEND_ADD );
	context->setDepthWriteState( false );

	// 3 faces
	context->beginGroup( HKG_IMM_TRIANGLE_STRIP );
	context->setCurrentPosition( (float*)&verts[0] );
	context->setCurrentPosition( (float*)&verts[1] );
	context->setCurrentPosition( (float*)&verts[2] );
	context->setCurrentPosition( (float*)&verts[3] );
	context->setCurrentPosition( (float*)&verts[6] );
	context->setCurrentPosition( (float*)&verts[7] );
	context->setCurrentPosition( (float*)&verts[4] );
	context->setCurrentPosition( (float*)&verts[5] );
	context->endGroup();

	// 3 faces
	context->beginGroup( HKG_IMM_TRIANGLE_STRIP );
	context->setCurrentPosition( (float*)&verts[6] );
	context->setCurrentPosition( (float*)&verts[2] );
	context->setCurrentPosition( (float*)&verts[4] );
	context->setCurrentPosition( (float*)&verts[0] );
	context->setCurrentPosition( (float*)&verts[5] );
	context->setCurrentPosition( (float*)&verts[1] );
	context->setCurrentPosition( (float*)&verts[7] );
	context->setCurrentPosition( (float*)&verts[3] );
	context->endGroup();

	context->setBlendMode( HKG_BLEND_MODULATE );
	context->setBlendState( false );
	context->setCurrentColorPacked( color );
	context->beginGroup( HKG_IMM_LINES );
	context->setCurrentPosition( (float*)&verts[0] );
	context->setCurrentPosition( (float*)&verts[1] );
	context->setCurrentPosition( (float*)&verts[1] );
	context->setCurrentPosition( (float*)&verts[3] );
	context->setCurrentPosition( (float*)&verts[3] );
	context->setCurrentPosition( (float*)&verts[2] );
	context->setCurrentPosition( (float*)&verts[2] );
	context->setCurrentPosition( (float*)&verts[0] );

	context->setCurrentPosition( (float*)&verts[4] );
	context->setCurrentPosition( (float*)&verts[5] );
	context->setCurrentPosition( (float*)&verts[5] );
	context->setCurrentPosition( (float*)&verts[7] );
	context->setCurrentPosition( (float*)&verts[7] );
	context->setCurrentPosition( (float*)&verts[6] );
	context->setCurrentPosition( (float*)&verts[6] );
	context->setCurrentPosition( (float*)&verts[4] );

	context->setCurrentPosition( (float*)&verts[4] );
	context->setCurrentPosition( (float*)&verts[0] );
	context->setCurrentPosition( (float*)&verts[6] );
	context->setCurrentPosition( (float*)&verts[2] );
	context->setCurrentPosition( (float*)&verts[7] );
	context->setCurrentPosition( (float*)&verts[3] );
	context->setCurrentPosition( (float*)&verts[5] );
	context->setCurrentPosition( (float*)&verts[1] );
	context->endGroup();

	context->matchState(restoreState, context->getCullfaceMode(), context->getBlendMode());
}

static inline void convertSplitTypeToHkVector4(const hkSplitType* splits, hkVector4& v1, hkVector4& v2)
{
	v1.set(hkReal(splits[0]), hkReal(splits[1]), hkReal(splits[2]), hkReal(splits[3]));
	v2.set(hkReal(splits[4]), hkReal(splits[5]), hkReal(splits[6]), hkReal(splits[7]));
}

static void getAabbFromSplitArray(const hkSplitType* aabbData, hkAabb& aabb, hkVector4Parameter offset, hkVector4Parameter _scale)
{
	const hkQuadReal oneOver65K = HK_QUADREAL_CONSTANT(1.0f/65535.0f, 1.0f/65535.0f, 1.0f/65535.0f, 1.0f/65535.0f);
	hkVector4 scale; scale.getQuad() = oneOver65K;
	scale.mul4(_scale); 

	hkVector4 min, max;
	convertSplitTypeToHkVector4(aabbData, min, max);
	aabb.m_min.setAddMul4(offset, scale, min); // offset + (scale*min)
	aabb.m_max.setAddMul4(offset, scale, max);

	//HK_ASSERT(0x0, aabb.isValid());
}

void hkKdTreeDebugUtils::drawBounds( hkgDisplayContext* context, const hkKdTreeNode* node, const hkAabb& originalBounds, hkAabb& bounds, hkUint32 maxRecursion )
{
	hkVector4 scale; scale.setSub4( originalBounds.m_max, originalBounds.m_min );

	if (maxRecursion == 0)
	{
		drawAabb( context, bounds.m_min, bounds.m_max, 0xff0000ff );
		return;
	}

	if ( node->isLeaf() )
	{
		if ( !node->isEmptyLeaf() )
		{
			drawAabb( context, bounds.m_min, bounds.m_max, 0xff0000ff );

			
			HK_ASSERT(0x0, false);

			// Get the aabb
			const hkSplitType* aabbData = reinterpret_cast<const hkSplitType*> ((hkUlong)node + node->getNumPrimitivesInLeaf() );
			hkAabb aabb;
			getAabbFromSplitArray( aabbData, aabb, originalBounds.m_min, scale);

			drawAabb( context, aabb.m_min, aabb.m_max, 0xff00ff00 );

		}
		return;
	}

	// Split the Aabb along the axis.
	hkAabb left = bounds;
	left.m_max( node->getSplitAxis() ) = hkConvertSplitTypeToReal( node->getSplitMax(), originalBounds.m_min( node->getSplitAxis() ), scale( node->getSplitAxis() ));
	drawBounds( context, node->getLeft(), originalBounds, left, maxRecursion - 1 );

	hkAabb right = bounds;
	right.m_min( node->getSplitAxis() ) = hkConvertSplitTypeToReal( node->getSplitMin(), originalBounds.m_min( node->getSplitAxis() ), scale( node->getSplitAxis() ));
	drawBounds( context, node->getRight(), originalBounds, right, maxRecursion - 1 );

}

static void drawSplitPlanes( hkgDisplayContext* context, hkReal minSplit, hkReal maxSplit, int axis, const hkVector4& mini, const hkVector4& maxi)
{
	hkVector4 verts[8];
	hkVector4 s, e;
	s.setAdd4( mini, maxi ); s.mul4( 0.5f ); e = s;
	s( axis ) = minSplit;
	e( axis ) = maxSplit;


	switch ( axis )
	{
	case 0:
		verts[0].set( minSplit, mini(1), mini(2) );
		verts[1].set( minSplit, maxi(1), mini(2) );
		verts[2].set( minSplit, mini(1), maxi(2) );
		verts[3].set( minSplit, maxi(1), maxi(2) );

		verts[4].set( maxSplit, mini(1), mini(2) );
		verts[5].set( maxSplit, maxi(1), mini(2) );
		verts[6].set( maxSplit, mini(1), maxi(2) );
		verts[7].set( maxSplit, maxi(1), maxi(2) );
		break;

	case 1:
		verts[0].set( mini(0), minSplit, mini(2) );
		verts[1].set( mini(0), minSplit, maxi(2) );
		verts[2].set( maxi(0), minSplit, mini(2) );
		verts[3].set( maxi(0), minSplit, maxi(2) );

		verts[4].set( mini(0), maxSplit, maxi(2) );
		verts[5].set( mini(0), maxSplit, mini(2) );
		verts[6].set( maxi(0), maxSplit, maxi(2) );
		verts[7].set( maxi(0), maxSplit, mini(2) );
		break;

	case 2:
		verts[0].set( mini(0), mini(1), minSplit );
		verts[1].set( maxi(0), mini(1), minSplit );
		verts[2].set( mini(0), maxi(1), minSplit );
		verts[3].set( maxi(0), maxi(1), minSplit );

		verts[4].set( mini(0), mini(1), maxSplit );
		verts[5].set( maxi(0), mini(1), maxSplit );
		verts[6].set( mini(0), maxi(1), maxSplit );
		verts[7].set( maxi(0), maxi(1), maxSplit );

		break;
	}

	HKG_ENABLED_STATE restoreState = context->getEnabledState();

	context->setCurrentColorPacked( 0x1f7f7f10 );
	context->setBlendState( true );
	context->setCullFaceState( false );
	context->setBlendMode( HKG_BLEND_ADD );
	context->setDepthWriteState( false );

	// Min Split
	context->beginGroup( HKG_IMM_TRIANGLE_STRIP );
	context->setCurrentPosition( (float*)&verts[0] );
	context->setCurrentPosition( (float*)&verts[1] );
	context->setCurrentPosition( (float*)&verts[2] );
	context->setCurrentPosition( (float*)&verts[3] );
	context->endGroup();

	// Max Split
	context->beginGroup( HKG_IMM_TRIANGLE_STRIP );
	context->setCurrentPosition( (float*)&verts[4] );
	context->setCurrentPosition( (float*)&verts[5] );
	context->setCurrentPosition( (float*)&verts[6] );
	context->setCurrentPosition( (float*)&verts[7] );
	context->endGroup();

	context->setBlendMode( HKG_BLEND_MODULATE );
	context->setBlendState( false );
	context->setCurrentColorPacked( 0x000000FF );
	context->beginGroup( HKG_IMM_LINES );
	context->setCurrentPosition( (float*)&verts[0] );
	context->setCurrentPosition( (float*)&verts[1] );
	context->setCurrentPosition( (float*)&verts[1] );
	context->setCurrentPosition( (float*)&verts[3] );
	context->setCurrentPosition( (float*)&verts[3] );
	context->setCurrentPosition( (float*)&verts[2] );
	context->setCurrentPosition( (float*)&verts[2] );
	context->setCurrentPosition( (float*)&verts[0] );

	context->setCurrentPosition( (float*)&verts[4] );
	context->setCurrentPosition( (float*)&verts[5] );
	context->setCurrentPosition( (float*)&verts[5] );
	context->setCurrentPosition( (float*)&verts[7] );
	context->setCurrentPosition( (float*)&verts[7] );
	context->setCurrentPosition( (float*)&verts[6] );
	context->setCurrentPosition( (float*)&verts[6] );
	context->setCurrentPosition( (float*)&verts[4] );

	//context->setCurrentColorPacked( 0x00FF00FF );
	//context->setCurrentPosition( (float*)&s );
	//context->setCurrentPosition( (float*)&e );

	hkReal arrowLength = 0.1f;
	hkVector4 dir; dir.setZero4();
	dir(axis) = arrowLength;
	HK_DISPLAY_ARROW(s, dir, hkColor::ORANGE);

	dir(axis) = -arrowLength;
	HK_DISPLAY_ARROW(e, dir, hkColor::MAGENTA); 

	context->endGroup();

	context->matchState(restoreState, context->getCullfaceMode(), context->getBlendMode());
}

void hkKdTreeDebugUtils::drawSplits( hkgDisplayContext* context, const hkKdTreeNode* node, const hkAabb& originalBounds, hkAabb& bounds, hkUint32 maxRecursion )
{
	hkVector4 scale; scale.setSub4( originalBounds.m_max, originalBounds.m_min );

	if (!node || (maxRecursion == 0))
		return;

	if ( node->isLeaf() )
	{
		return;
	}

	// Split the Aabb along the axis.
	hkAabb left = bounds;
	left.m_max( node->getSplitAxis() ) = hkConvertSplitTypeToReal( node->getSplitMax(), originalBounds.m_min( node->getSplitAxis() ), scale( node->getSplitAxis() ));
	drawSplits( context, node->getLeft(), originalBounds, left, maxRecursion - 1 );

	hkAabb right = bounds;
	right.m_min( node->getSplitAxis() ) = hkConvertSplitTypeToReal( node->getSplitMin(), originalBounds.m_min( node->getSplitAxis() ), scale( node->getSplitAxis() ));
	drawSplits( context, node->getRight(), originalBounds, right, maxRecursion - 1 );

	drawSplitPlanes( context, right.m_min( node->getSplitAxis() ), left.m_max( node->getSplitAxis()), node->getSplitAxis(), bounds.m_min, bounds.m_max );
}


 void hkKdTreeDebugUtils::drawTree( const hkKdTreeNode* node, hkReal x, hkReal y, hkReal w, hkReal h, DrawTreeLeafCallback* callback, hkUint32 maxDepth)
{
	if (!node || (maxDepth == 0) )
		return;

	hkVector4 pos ( x, 0, y );

	if ( node->isLeaf() )
	{
		if ( !node->isEmptyLeaf() )
		{
			HK_DISPLAY_STAR( pos, 0.05f, hkColor::GREEN );
			if (callback)
				callback->handleLeaf( pos, node );
		}
		else
		{
			HK_DISPLAY_STAR( pos, 0.05f, hkColor::RED );
		}
		return;
	}

	hkString text; 
	text.printf(" (%d) (%.4f, %.4f)", node->getSplitAxis(), node->getSplitMin(), node->getSplitMax() );
	//HK_DISPLAY_3D_TEXT( text.cString(), pos, hkColor::BLUE);

	drawTree( node->getLeft(),  x - w*0.5f, y - h*0.75f, w*0.5f, h*0.75f, callback, maxDepth-1);
	drawTree( node->getRight(), x + w*0.5f, y - h*0.75f, w*0.5f, h*0.75f, callback, maxDepth-1);

	hkVector4 left ( x - w*0.5f, 0, y - h*0.75f) ;
	hkVector4 right( x + w*0.5f, 0, y - h*0.75f) ;

	HK_DISPLAY_LINE( pos, left, hkColor::BLUE );
	HK_DISPLAY_LINE( pos, right, hkColor::BLUE );


}

 void hkKdTreeDebugUtils::calcVolume( const hkKdTreeNode* node, const hkAabb& bounds, hkReal& totalVolume, hkReal& emptyVolume, hkReal& leafVolume, hkUint32 maxDepth )
 {
	hkVector4 scale; scale.setSub4( bounds.m_max, bounds.m_min );
	hkReal volume = scale(0)*scale(1)*scale(2);
	totalVolume += volume;

	if (!node || (maxDepth == 0) )
		return;

	if ( node->isLeaf() )
	{
		 if ( !node->isEmptyLeaf() )
		 {
			 leafVolume += volume;
		 }
		 else
		 {
			 emptyVolume += volume;
		 }
		 return;
	}

	 hkReal splitMax = hkConvertSplitTypeToReal( node->getSplitMax(), bounds.m_min( node->getSplitAxis() ), scale( node->getSplitAxis() ));
	 hkReal splitMin = hkConvertSplitTypeToReal( node->getSplitMin(), bounds.m_min( node->getSplitAxis() ), scale( node->getSplitAxis() ));

	 if (splitMin> splitMax)
		 emptyVolume += volume * (splitMin - splitMax) / scale( node->getSplitAxis() );

	 // Split the Aabb along the axis.
	 hkAabb left = bounds;
	 left.m_max( node->getSplitAxis() ) = hkConvertSplitTypeToReal( node->getSplitMax(), bounds.m_min( node->getSplitAxis() ), scale( node->getSplitAxis() ));
	 calcVolume( node->getLeft(), left, totalVolume, emptyVolume, leafVolume, maxDepth -1 ); 

	 hkAabb right = bounds;
	 right.m_min( node->getSplitAxis() ) = hkConvertSplitTypeToReal( node->getSplitMin(), bounds.m_min( node->getSplitAxis() ), scale( node->getSplitAxis() ));
	 calcVolume( node->getRight(), right, totalVolume, emptyVolume, leafVolume, maxDepth -1 );


 }

 inline hkReal surfaceArea( const hkAabb& bounds )
 {
	 hkVector4 extents;
	 extents.setSub4( bounds.m_max, bounds.m_min );
	 return 2.0f * ( extents(0)*extents(1) + extents(1)*extents(2) + extents(2)*extents(0) );
 }

 hkReal hkKdTreeDebugUtils::calcCost( const Costs& costs, const hkKdTreeNode* node, const hkAabb& bounds )
 {
	 hkVector4 scale; scale.setSub4( bounds.m_max, bounds.m_min );

	 if (!node)
		 return 0.0f;

	 if ( node->isLeaf() )
	 {
		 if ( !node->isEmptyLeaf() )
		 {
			 return costs.intersection;
		 }
		 return 0.0f;
	 }

	 hkReal splitMax = hkConvertSplitTypeToReal( node->getSplitMax(), bounds.m_min( node->getSplitAxis() ), scale( node->getSplitAxis() ));
	 hkReal splitMin = hkConvertSplitTypeToReal( node->getSplitMin(), bounds.m_min( node->getSplitAxis() ), scale( node->getSplitAxis() ));

	 hkReal totalSA = surfaceArea( bounds );

	 hkAabb leftBounds = bounds;
	 leftBounds.m_max( node->getSplitAxis() ) = splitMax;
	 hkReal costLeft = calcCost( costs, node->getLeft(), leftBounds ); 

	 hkAabb rightBounds = bounds;
	 rightBounds.m_min( node->getSplitAxis() ) = splitMin;
	 hkReal costRight = calcCost( costs, node->getRight(), rightBounds );

	 totalSA = hkMath::max2( totalSA, HK_REAL_MIN );
	 return costs.traversal + surfaceArea(leftBounds) / totalSA * costLeft + surfaceArea(rightBounds) / totalSA * costRight;
 }

void hkKdTreeDebugUtils::compareTrees( const hkKdTree* a, const hkKdTree* b )
{
#ifdef HK_DEBUG
	HK_ASSERT(0x0, a->m_maxDepth == b->m_maxDepth);

	compareTrees(a->getRoot(), b->getRoot() );

	HK_ASSERT(0x0, a->getNumPrimitives() == b->getNumPrimitives());
	const hkKdTreeBuildInput::ProjectedEntry* entA = a->getProjectedEntries();
	const hkKdTreeBuildInput::ProjectedEntry* entB = b->getProjectedEntries();

	for (int i=0; i<a->getNumPrimitives(); i++)
	{
		HK_ASSERT(0x0, entA[i].m_min[0] == entB[i].m_min[0]);
		HK_ASSERT(0x0, entA[i].m_min[1] == entB[i].m_min[1]);
		HK_ASSERT(0x0, entA[i].m_min[2] == entB[i].m_min[2]);
		
		HK_ASSERT(0x0, entA[i].m_max[0] == entB[i].m_max[0]);
		HK_ASSERT(0x0, entA[i].m_max[1] == entB[i].m_max[1]);
		HK_ASSERT(0x0, entA[i].m_max[2] == entB[i].m_max[2]);
	}
#endif
}

 void hkKdTreeDebugUtils::compareTrees( const hkKdTreeNode* a, const hkKdTreeNode* b )
 {
	 if (a->isLeaf())
	 {
		 HK_ASSERT(0x0, b->isLeaf());
		 if (!a->isEmptyLeaf())
		 {
			 HK_ASSERT(0x0, !b->isEmptyLeaf() && ( a->getPrimitiveId() == b->getPrimitiveId() ) );

			 return;

		 }
		 HK_ASSERT(0x0, b->isEmptyLeaf());
		 return;
	 }

	 HK_ASSERT(0x0, a->getSplitAxis() == b->getSplitAxis() );
	 HK_ASSERT(0x0, a->getSplitMin() == b->getSplitMin() );
	 HK_ASSERT(0x0, a->getSplitMax() == b->getSplitMax() );

	 compareTrees( a->getLeft(), b->getLeft() );
	 compareTrees( a->getRight(), b->getRight() );
 }

 static void getSplitBoundsUnion(hkKdTreeBuildInput::ProjectedEntry& boundOut, hkKdTreeBuildInput::ProjectedEntry& boundIn0, hkKdTreeBuildInput::ProjectedEntry& boundIn1)
 {
	 boundOut.m_min[0] = hkMath::min2( boundIn0.m_min[0], boundIn1.m_min[0] );
	 boundOut.m_min[1] = hkMath::min2( boundIn0.m_min[1], boundIn1.m_min[1] );
	 boundOut.m_min[2] = hkMath::min2( boundIn0.m_min[2], boundIn1.m_min[2] );
	 boundOut.m_max[0] = hkMath::max2( boundIn0.m_max[0], boundIn1.m_max[0] );
	 boundOut.m_max[1] = hkMath::max2( boundIn0.m_max[1], boundIn1.m_max[1] );
	 boundOut.m_max[2] = hkMath::max2( boundIn0.m_max[2], boundIn1.m_max[2] );
 }

 void _validateNode( const hkKdTreeNode* current, const hkKdTreeBuildInput::ProjectedEntry* projEntries, hkKdTreeBuildInput::ProjectedEntry& boundOut, hkKdTreeBuildInput::Mediator& mediator, hkArray<hkPrimitiveId>& primIdList )
{
	if (current->isLeaf())
	{
		// Can't use hkSplitTypeBounds::reset
		boundOut.m_min[0] = boundOut.m_min[1] = boundOut.m_min[2] = HK_KD_NODE_MAX_SPLIT;
		boundOut.m_max[0] = boundOut.m_max[1] = boundOut.m_max[2] = HK_KD_NODE_MIN_SPLIT;

		// Nothing to check for empty nodes
		if(current->isEmptyLeaf())
		{
			return;
		}
		
		const int numPrimsInLeaf = current->getNumPrimitivesInLeaf();

		for (int i=0; i<numPrimsInLeaf; i++)
		{
			const int peIdx = (int)current->getPrimitiveId() + i;
			const hkKdTreeBuildInput::ProjectedEntry& pe = projEntries[ peIdx ];
			const int primId = (int) pe.getPrimitiveId();

			// primBounds is the bounds of this primitive
			// boundOut is the union of all primitives' bounds
			hkKdTreeBuildInput::ProjectedEntry primBounds; 
			primBounds =  *reinterpret_cast<const hkKdTreeBuildInput::ProjectedEntry*> ( &projEntries[ peIdx ] );

			hkKdTreeBuildInput::ProjectedEntry mediatorBounds;
			mediator.projectPrimitive( primId, mediatorBounds );

			HK_ASSERT(0x0, mediatorBounds.m_min[0] == primBounds.m_min[0]);
			HK_ASSERT(0x0, mediatorBounds.m_min[1] == primBounds.m_min[1]);
			HK_ASSERT(0x0, mediatorBounds.m_min[2] == primBounds.m_min[2]);

			HK_ASSERT(0x0, mediatorBounds.m_max[0] == primBounds.m_max[0]);
			HK_ASSERT(0x0, mediatorBounds.m_max[1] == primBounds.m_max[1]);
			HK_ASSERT(0x0, mediatorBounds.m_max[2] == primBounds.m_max[2]);

			getSplitBoundsUnion(boundOut, boundOut, primBounds);

			int idx = primIdList.indexOf( primId );
			HK_ASSERT(0x0, idx >=0);
			primIdList.removeAt( idx );
		}
		return;
	}

	hkKdTreeBuildInput::ProjectedEntry  leftBound;
	hkKdTreeBuildInput::ProjectedEntry rightBound;

	_validateNode(  current->getLeft(), projEntries, leftBound, mediator, primIdList );
	_validateNode( current->getRight(), projEntries, rightBound, mediator, primIdList );

	HK_ASSERT(0x0,  leftBound.m_max[ current->getSplitAxis() ] <= current->getSplitMax() );
	HK_ASSERT(0x0,  rightBound.m_min[ current->getSplitAxis() ] >= current->getSplitMin() );

	getSplitBoundsUnion(boundOut, leftBound, rightBound);

}

void hkKdTreeDebugUtils::validateTree( const hkKdTree* tree, hkKdTreeBuildInput::Mediator& mediator )
{
	HK_TIME_CODE_BLOCK("ValidateTree", HK_NULL);
	hkKdTreeBuildInput::ProjectedEntry bounds;

	hkArray<hkPrimitiveId> prims;
	for (int i=0; i < mediator.getNumPrimitives(); i++)
	{
		prims.pushBack( mediator.getPrimitiveId(i) );
	}
	_validateNode( tree->getRoot(), tree->getProjectedEntries(), bounds, mediator, prims );

	// Check all primitives exist in the tree
	HK_ASSERT(0x0, prims.getSize() == 0 );

	// Check all primitives exist in the projectedPrimitives array
	
	{
		HK_ASSERT(0x0, mediator.getNumPrimitives() == tree->getNumPrimitives() );
		for (int i=0; i < mediator.getNumPrimitives(); i++)
		{
			prims.pushBack( mediator.getPrimitiveId(i) );
		}

		for (int i=0; i<tree->getNumPrimitives(); i++)
		{
			hkPrimitiveId primId;
			const hkKdTreeBuildInput::ProjectedEntry& pe = tree->getProjectedEntries()[i];
			primId = pe.getPrimitiveId();

			int idx = prims.indexOf(primId);
			HK_ASSERT(0x0, idx>=0);

			prims.removeAt(idx);
		}
		HK_ASSERT(0x0, prims.getSize() == 0 );
	}

}

void hkKdTreeDebugUtils::verifyTreeRaycasts( const hkKdTree* tree, class hkpWorld* world, int numRays)
{
#ifdef HK_DEBUG
	world->markForWrite();
	
	hkVector4 minExtent, maxExtent;
	const hkpBroadPhase* bp = world->getBroadPhase();
	bp->getExtents(minExtent, maxExtent);
	hkVector4 extent; extent.setSub4( maxExtent, minExtent);

	HK_TIMER_BEGIN("verifyTree", HK_NULL);
	for (int i=0; i<numRays; i++)
	{
		hkpWorldRayCastInput input;
		input.m_from(0) = .95f * hkMath::randRange(minExtent(0), maxExtent(0));
		input.m_from(1) = .95f * hkMath::randRange(minExtent(1), maxExtent(1));
		input.m_from(2) = .95f * hkMath::randRange(minExtent(2), maxExtent(2));

		input.m_to(0) = .95f * hkMath::randRange(minExtent(0), maxExtent(0));
		input.m_to(1) = .95f * hkMath::randRange(minExtent(1), maxExtent(1));
		input.m_to(2) = .95f * hkMath::randRange(minExtent(2), maxExtent(2));

		hkpWorldRayCastOutput worldOutput, treeOutput;

		world->castRay(input, worldOutput);

		hkpKdTreeCollidableRaycaster treeRc;
		treeRc.castRayIterative(tree, input, treeOutput);

		if (worldOutput.hasHit())
		{
			hkpRigidBody* rbHit = hkGetRigidBody(worldOutput.m_rootCollidable);
			if (rbHit)
			{
				HK_ASSERT(0x0, treeOutput.hasHit() == worldOutput.hasHit());
				HK_ASSERT(0x0, hkMath::equal(treeOutput.m_hitFraction, worldOutput.m_hitFraction) );
			}
		}
	}
	HK_TIMER_END();

	world->unmarkForWrite();
#endif 
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
