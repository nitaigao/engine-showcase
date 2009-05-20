/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_KDTREE_DEBUG_UTILS_H
#define HK_KDTREE_DEBUG_UTILS_H

#include <Common/Internal/KdTree/hkKdTree.h>
#include <Common/Internal/KdTree/Build/hkKdTreeBuildInput.h>

class hkKdTreeDebugUtils
{
public:

	static void HK_CALL drawBounds( class hkgDisplayContext* context, const hkKdTreeNode* rootNode, const hkAabb& originalBounds, hkAabb& bounds, hkUint32 maxDepth = -1 );

	static void HK_CALL drawSplits( hkgDisplayContext* context, const hkKdTreeNode* rootNode, const hkAabb& originalBounds, hkAabb& bounds, hkUint32 maxDepth = -1 );

	struct DrawTreeLeafCallback
	{
		virtual void handleLeaf( const hkVector4& leafPos, const hkKdTreeNode* leaf ) = 0;
		virtual ~DrawTreeLeafCallback() {}
	};

	static void HK_CALL drawTree( const hkKdTreeNode* node, hkReal x, hkReal y, hkReal w, hkReal h, DrawTreeLeafCallback* callback = HK_NULL, hkUint32 maxDepth = -1);

	static void HK_CALL calcVolume( const hkKdTreeNode* rootNode, const hkAabb& bounds, hkReal& totalVolume, hkReal& emptyVolume, hkReal& leafVolume, hkUint32 maxDepth = -1 );

	struct Costs
	{
		hkReal traversal;
		hkReal intersection;
	};

	static hkReal HK_CALL calcCost(const Costs& costs,  const hkKdTreeNode* node, const hkAabb& bounds );

		/// Asserts trees are identical. Works in DEBUG / FULLDEBUG only
	static void HK_CALL compareTrees( const hkKdTree* a, const hkKdTree* b );
	static void HK_CALL compareTrees( const hkKdTreeNode* a, const hkKdTreeNode* b );

		/// Asserts that KdTree raycasts give the same results as hkWorld raycasts
	static void HK_CALL verifyTreeRaycasts( const hkKdTree* tree, class hkpWorld*, int numRays);

	static void HK_CALL validateTree( const hkKdTree* tree, class hkKdTreeBuildInput::Mediator& mediator );
};

#endif // HK_KDTREE_DEBUG_UTILS_H



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
