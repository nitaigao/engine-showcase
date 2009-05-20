/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// WARNING: THIS FILE IS GENERATED. EDITS WILL BE LOST.
// Generated from 'Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/DestructibleWallsDemo.h'
#include <Demos/demos.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>
#include <Common/Base/Reflection/hkTypeInfo.h>
#include <Demos/Physics/Test/Feature/Dynamics/DestructibleWalls/DestructibleWallsDemo.h>



// External pointer and enum types
extern const hkClass DestructibleWallsDemoBrickConstraintListenerClass;
extern const hkClass DestructibleWallsDemoWallFractureUtilityClass;

//
// Class DestructibleWallsDemo::Options
//
static const hkInternalClassMember DestructibleWallsDemo_OptionsClass_Members[] =
{
	{ "useParallelSimulation", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(DestructibleWallsDemo::Options,m_useParallelSimulation), HK_NULL },
	{ "WallsWidth", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(DestructibleWallsDemo::Options,m_WallsWidth), HK_NULL },
	{ "WallsHeight", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(DestructibleWallsDemo::Options,m_WallsHeight), HK_NULL },
	{ "bottomConstraintStrength", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(DestructibleWallsDemo::Options,m_bottomConstraintStrength), HK_NULL },
	{ "topConstraintStrength", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(DestructibleWallsDemo::Options,m_topConstraintStrength), HK_NULL },
	{ "attachToGround", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(DestructibleWallsDemo::Options,m_attachToGround), HK_NULL },
	{ "showDebugDisplay", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(DestructibleWallsDemo::Options,m_showDebugDisplay), HK_NULL },
	{ "wireFrameMode", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(DestructibleWallsDemo::Options,m_wireFrameMode), HK_NULL },
	{ "cannonBallRadius", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(DestructibleWallsDemo::Options,m_cannonBallRadius), HK_NULL },
	{ "cannonBallMass", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(DestructibleWallsDemo::Options,m_cannonBallMass), HK_NULL }
};
extern const hkClass DestructibleWallsDemoOptionsClass;
const hkClass DestructibleWallsDemoOptionsClass(
	"DestructibleWallsDemoOptions",
	HK_NULL, // parent
	sizeof(DestructibleWallsDemo::Options),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(DestructibleWallsDemo_OptionsClass_Members),
	HK_COUNT_OF(DestructibleWallsDemo_OptionsClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SCOPED_SIMPLE(DestructibleWallsDemo,Options);

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
