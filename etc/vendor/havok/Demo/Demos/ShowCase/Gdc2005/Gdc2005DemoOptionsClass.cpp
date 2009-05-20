/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// WARNING: THIS FILE IS GENERATED. EDITS WILL BE LOST.
// Generated from 'Demos/ShowCase/Gdc2005/Gdc2005DemoOptions.h'
#include <Demos/demos.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>
#include <Common/Base/Reflection/hkTypeInfo.h>
#include <Demos/ShowCase/Gdc2005/Gdc2005DemoOptions.h>



// External pointer and enum types
extern const hkClass hkGdcDisplayOptionsClass;
extern const hkClass hkGdcGetHitOptionsClass;
extern const hkClass hkGdcGetHitOptionsPositionClass;
extern const hkClass hkGdcGetHitOptionsSnapClass;
extern const hkClass hkGdcGetHitOptionsVelocityClass;
extern const hkClass hkGdcGetupOptionsClass;
extern const hkClass hkGdcIkOptionsClass;
extern const hkClass hkGdcMatchOptionsClass;
extern const hkClass hkGdcMiscOptionsClass;
extern const hkClass hkGdcPhysicsOptionsClass;
extern const hkClass hkGdcProxyOptionsClass;

//
// Enum hkGdcDisplayOptions::CameraMode
//
static const hkInternalClassEnumItem hkGdcDisplayOptionsCameraModeEnumItems[] =
{
	{0, "TRACKING"},
	{1, "CLOSEST"},
	{-1, "FREE"},
};
static const hkInternalClassEnum hkGdcDisplayOptionsEnums[] = {
	{"CameraMode", hkGdcDisplayOptionsCameraModeEnumItems, 3, HK_NULL, 0 }
};
extern const hkClassEnum* hkGdcDisplayOptionsCameraModeEnum = reinterpret_cast<const hkClassEnum*>(&hkGdcDisplayOptionsEnums[0]);

//
// Class hkGdcDisplayOptions
//
static const hkInternalClassMember hkGdcDisplayOptionsClass_Members[] =
{
	{ "originalPose", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcDisplayOptions,m_originalPose), HK_NULL },
	{ "finalPose", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcDisplayOptions,m_finalPose), HK_NULL },
	{ "proxy", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcDisplayOptions,m_proxy), HK_NULL },
	{ "ragdoll", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcDisplayOptions,m_ragdoll), HK_NULL },
	{ "skinning", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcDisplayOptions,m_skinning), HK_NULL },
	{ "shadows", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcDisplayOptions,m_shadows), HK_NULL },
	{ "lightmaps", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcDisplayOptions,m_lightmaps), HK_NULL },
	{ "flashlight", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcDisplayOptions,m_flashlight), HK_NULL },
	{ "cameraMode", HK_NULL, hkGdcDisplayOptionsCameraModeEnum, hkClassMember::TYPE_ENUM, hkClassMember::TYPE_INT8, 0, 0, HK_OFFSET_OF(hkGdcDisplayOptions,m_cameraMode), HK_NULL },
	{ "cameraGain", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcDisplayOptions,m_cameraGain), HK_NULL },
	{ "showState", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcDisplayOptions,m_showState), HK_NULL },
	{ "showIkInfo", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcDisplayOptions,m_showIkInfo), HK_NULL }
};
const hkClass hkGdcDisplayOptionsClass(
	"hkGdcDisplayOptions",
	HK_NULL, // parent
	sizeof(hkGdcDisplayOptions),
	HK_NULL,
	0, // interfaces
	reinterpret_cast<const hkClassEnum*>(hkGdcDisplayOptionsEnums),
	1, // enums
	reinterpret_cast<const hkClassMember*>(hkGdcDisplayOptionsClass_Members),
	HK_COUNT_OF(hkGdcDisplayOptionsClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(hkGdcDisplayOptions);

//
// Class hkGdcProxyOptions
//
static const hkInternalClassMember hkGdcProxyOptionsClass_Members[] =
{
	{ "height", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcProxyOptions,m_height), HK_NULL },
	{ "radius", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcProxyOptions,m_radius), HK_NULL },
	{ "jumpImpulse", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcProxyOptions,m_jumpImpulse), HK_NULL },
	{ "friction", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcProxyOptions,m_friction), HK_NULL },
	{ "strength", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcProxyOptions,m_strength), HK_NULL },
	{ "safeFallTime", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcProxyOptions,m_safeFallTime), HK_NULL },
	{ "verticalGain", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcProxyOptions,m_verticalGain), HK_NULL },
	{ "horizontalGain", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcProxyOptions,m_horizontalGain), HK_NULL },
	{ "maxVerticalSeparation", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcProxyOptions,m_maxVerticalSeparation), HK_NULL },
	{ "maxHorizontalSeparation", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcProxyOptions,m_maxHorizontalSeparation), HK_NULL },
	{ "pushIfFootInAir", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcProxyOptions,m_pushIfFootInAir), HK_NULL }
};
const hkClass hkGdcProxyOptionsClass(
	"hkGdcProxyOptions",
	HK_NULL, // parent
	sizeof(hkGdcProxyOptions),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(hkGdcProxyOptionsClass_Members),
	HK_COUNT_OF(hkGdcProxyOptionsClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(hkGdcProxyOptions);

//
// Enum hkGdcIkOptions::LookAtMode
//
static const hkInternalClassEnumItem hkGdcIkOptionsLookAtModeEnumItems[] =
{
	{0, "LOOKAT_OFF"},
	{1, "LOOKAT_STANDING"},
	{2, "LOOKAT_ALIVE"},
	{3, "LOOKAT_ALWAYS"},
};

//
// Enum hkGdcIkOptions::LookAtTarget
//
static const hkInternalClassEnumItem hkGdcIkOptionsLookAtTargetEnumItems[] =
{
	{0, "LOOKAT_CAMERA"},
	{1, "LOOKAT_LAST_OBJECT"},
};

//
// Enum hkGdcIkOptions::HandIkMode
//
static const hkInternalClassEnumItem hkGdcIkOptionsHandIkModeEnumItems[] =
{
	{0, "HANDIK_OFF"},
	{1, "HANDIK_STANDING"},
	{2, "HANDIK_ALWAYS"},
};
static const hkInternalClassEnum hkGdcIkOptionsEnums[] = {
	{"LookAtMode", hkGdcIkOptionsLookAtModeEnumItems, 4, HK_NULL, 0 },
	{"LookAtTarget", hkGdcIkOptionsLookAtTargetEnumItems, 2, HK_NULL, 0 },
	{"HandIkMode", hkGdcIkOptionsHandIkModeEnumItems, 3, HK_NULL, 0 }
};
extern const hkClassEnum* hkGdcIkOptionsLookAtModeEnum = reinterpret_cast<const hkClassEnum*>(&hkGdcIkOptionsEnums[0]);
extern const hkClassEnum* hkGdcIkOptionsLookAtTargetEnum = reinterpret_cast<const hkClassEnum*>(&hkGdcIkOptionsEnums[1]);
extern const hkClassEnum* hkGdcIkOptionsHandIkModeEnum = reinterpret_cast<const hkClassEnum*>(&hkGdcIkOptionsEnums[2]);

//
// Class hkGdcIkOptions
//
static const hkInternalClassMember hkGdcIkOptionsClass_Members[] =
{
	{ "footIk", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcIkOptions,m_footIk), HK_NULL },
	{ "footIkOnOffGain", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcIkOptions,m_footIkOnOffGain), HK_NULL },
	{ "footIkAscendingGain", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcIkOptions,m_footIkAscendingGain), HK_NULL },
	{ "footIkStandAscendingGain", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcIkOptions,m_footIkStandAscendingGain), HK_NULL },
	{ "footIkDescendingGain", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcIkOptions,m_footIkDescendingGain), HK_NULL },
	{ "footIkMaxFootHeightMS", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcIkOptions,m_footIkMaxFootHeightMS), HK_NULL },
	{ "footIkMinFootHeightMS", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcIkOptions,m_footIkMinFootHeightMS), HK_NULL },
	{ "cosKneeLimit", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcIkOptions,m_cosKneeLimit), HK_NULL },
	{ "pelvisUpDownBias", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcIkOptions,m_pelvisUpDownBias), HK_NULL },
	{ "pelvisFeedback", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcIkOptions,m_pelvisFeedback), HK_NULL },
	{ "lookAtMode", HK_NULL, hkGdcIkOptionsLookAtModeEnum, hkClassMember::TYPE_ENUM, hkClassMember::TYPE_INT8, 0, 0, HK_OFFSET_OF(hkGdcIkOptions,m_lookAtMode), HK_NULL },
	{ "lookAtTarget", HK_NULL, hkGdcIkOptionsLookAtTargetEnum, hkClassMember::TYPE_ENUM, hkClassMember::TYPE_INT8, 0, 0, HK_OFFSET_OF(hkGdcIkOptions,m_lookAtTarget), HK_NULL },
	{ "lookAtLimit", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcIkOptions,m_lookAtLimit), HK_NULL },
	{ "lookAtGain", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcIkOptions,m_lookAtGain), HK_NULL },
	{ "lookAtTargetGain", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcIkOptions,m_lookAtTargetGain), HK_NULL },
	{ "handIkMode", HK_NULL, hkGdcIkOptionsHandIkModeEnum, hkClassMember::TYPE_ENUM, hkClassMember::TYPE_INT8, 0, 0, HK_OFFSET_OF(hkGdcIkOptions,m_handIkMode), HK_NULL },
	{ "handIkReachGain", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcIkOptions,m_handIkReachGain), HK_NULL },
	{ "handIkLeaveGain", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcIkOptions,m_handIkLeaveGain), HK_NULL },
	{ "handIkMoveGain", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcIkOptions,m_handIkMoveGain), HK_NULL }
};
const hkClass hkGdcIkOptionsClass(
	"hkGdcIkOptions",
	HK_NULL, // parent
	sizeof(hkGdcIkOptions),
	HK_NULL,
	0, // interfaces
	reinterpret_cast<const hkClassEnum*>(hkGdcIkOptionsEnums),
	3, // enums
	reinterpret_cast<const hkClassMember*>(hkGdcIkOptionsClass_Members),
	HK_COUNT_OF(hkGdcIkOptionsClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(hkGdcIkOptions);

//
// Class hkGdcMatchOptions
//
static const hkInternalClassMember hkGdcMatchOptionsClass_Members[] =
{
	{ "force", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcMatchOptions,m_force), HK_NULL },
	{ "tau", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcMatchOptions,m_tau), HK_NULL },
	{ "proportinalRecovery", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcMatchOptions,m_proportinalRecovery), HK_NULL },
	{ "constantRecovery", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcMatchOptions,m_constantRecovery), HK_NULL },
	{ "matchBlendSpeed", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcMatchOptions,m_matchBlendSpeed), HK_NULL },
	{ "hysterisis", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcMatchOptions,m_hysterisis), HK_NULL },
	{ "showDeactivation", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcMatchOptions,m_showDeactivation), HK_NULL }
};
const hkClass hkGdcMatchOptionsClass(
	"hkGdcMatchOptions",
	HK_NULL, // parent
	sizeof(hkGdcMatchOptions),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(hkGdcMatchOptionsClass_Members),
	HK_COUNT_OF(hkGdcMatchOptionsClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(hkGdcMatchOptions);

//
// Class hkGdcGetupOptions
//
static const hkInternalClassMember hkGdcGetupOptionsClass_Members[] =
{
	{ "autoGetup", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetupOptions,m_autoGetup), HK_NULL },
	{ "getupBlendSpeed", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetupOptions,m_getupBlendSpeed), HK_NULL },
	{ "dieAnimation", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetupOptions,m_dieAnimation), HK_NULL }
};
const hkClass hkGdcGetupOptionsClass(
	"hkGdcGetupOptions",
	HK_NULL, // parent
	sizeof(hkGdcGetupOptions),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(hkGdcGetupOptionsClass_Members),
	HK_COUNT_OF(hkGdcGetupOptionsClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(hkGdcGetupOptions);

//
// Class hkGdcGetHitOptions::Velocity
//
static const hkInternalClassMember hkGdcGetHitOptions_VelocityClass_Members[] =
{
	{ "velocityGain", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetHitOptions::Velocity,m_velocityGain), HK_NULL }
};
const hkClass hkGdcGetHitOptionsVelocityClass(
	"hkGdcGetHitOptionsVelocity",
	HK_NULL, // parent
	sizeof(hkGdcGetHitOptions::Velocity),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(hkGdcGetHitOptions_VelocityClass_Members),
	HK_COUNT_OF(hkGdcGetHitOptions_VelocityClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SCOPED_SIMPLE(hkGdcGetHitOptions,Velocity);

//
// Class hkGdcGetHitOptions::Position
//
static const hkInternalClassMember hkGdcGetHitOptions_PositionClass_Members[] =
{
	{ "positionGain", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetHitOptions::Position,m_positionGain), HK_NULL },
	{ "positionMaxLinearVelocity", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetHitOptions::Position,m_positionMaxLinearVelocity), HK_NULL },
	{ "positionMaxAngularVelocity", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetHitOptions::Position,m_positionMaxAngularVelocity), HK_NULL }
};
const hkClass hkGdcGetHitOptionsPositionClass(
	"hkGdcGetHitOptionsPosition",
	HK_NULL, // parent
	sizeof(hkGdcGetHitOptions::Position),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(hkGdcGetHitOptions_PositionClass_Members),
	HK_COUNT_OF(hkGdcGetHitOptions_PositionClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SCOPED_SIMPLE(hkGdcGetHitOptions,Position);

//
// Class hkGdcGetHitOptions::Snap
//
static const hkInternalClassMember hkGdcGetHitOptions_SnapClass_Members[] =
{
	{ "snapGain", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetHitOptions::Snap,m_snapGain), HK_NULL },
	{ "snapMaxLinearVelocity", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetHitOptions::Snap,m_snapMaxLinearVelocity), HK_NULL },
	{ "snapMaxAngularVelocity", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetHitOptions::Snap,m_snapMaxAngularVelocity), HK_NULL },
	{ "snapMaxLinearDistance", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetHitOptions::Snap,m_snapMaxLinearDistance), HK_NULL },
	{ "snapMaxAngularDistance", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetHitOptions::Snap,m_snapMaxAngularDistance), HK_NULL }
};
const hkClass hkGdcGetHitOptionsSnapClass(
	"hkGdcGetHitOptionsSnap",
	HK_NULL, // parent
	sizeof(hkGdcGetHitOptions::Snap),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(hkGdcGetHitOptions_SnapClass_Members),
	HK_COUNT_OF(hkGdcGetHitOptions_SnapClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SCOPED_SIMPLE(hkGdcGetHitOptions,Snap);

//
// Class hkGdcGetHitOptions
//
static const hkInternalClassMember hkGdcGetHitOptionsClass_Members[] =
{
	{ "fixLegs", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetHitOptions,m_fixLegs), HK_NULL },
	{ "showStress", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetHitOptions,m_showStress), HK_NULL },
	{ "ragdollFeedback", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetHitOptions,m_ragdollFeedback), HK_NULL },
	{ "hierarchyGain", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetHitOptions,m_hierarchyGain), HK_NULL },
	{ "dampVelocity", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetHitOptions,m_dampVelocity), HK_NULL },
	{ "accelerationGain", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetHitOptions,m_accelerationGain), HK_NULL },
	{ "velocity", &hkGdcGetHitOptionsVelocityClass, HK_NULL, hkClassMember::TYPE_STRUCT, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetHitOptions,m_velocity), HK_NULL },
	{ "position", &hkGdcGetHitOptionsPositionClass, HK_NULL, hkClassMember::TYPE_STRUCT, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetHitOptions,m_position), HK_NULL },
	{ "snap", &hkGdcGetHitOptionsSnapClass, HK_NULL, hkClassMember::TYPE_STRUCT, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcGetHitOptions,m_snap), HK_NULL }
};
const hkClass hkGdcGetHitOptionsClass(
	"hkGdcGetHitOptions",
	HK_NULL, // parent
	sizeof(hkGdcGetHitOptions),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(hkGdcGetHitOptionsClass_Members),
	HK_COUNT_OF(hkGdcGetHitOptionsClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(hkGdcGetHitOptions);

//
// Enum hkGdcPhysicsOptions::Objects
//
static const hkInternalClassEnumItem hkGdcPhysicsOptionsObjectsEnumItems[] =
{
	{0, "LEG"},
	{1, "BOXES"},
	{2, "SKULL"},
	{3, "PULLEY"},
	{4, "SPEARS"},
	{5, "CLUB"},
	{6, "DEBRIS"},
	{7, "RANDOM"},
};
static const hkInternalClassEnum hkGdcPhysicsOptionsEnums[] = {
	{"Objects", hkGdcPhysicsOptionsObjectsEnumItems, 8, HK_NULL, 0 }
};
extern const hkClassEnum* hkGdcPhysicsOptionsObjectsEnum = reinterpret_cast<const hkClassEnum*>(&hkGdcPhysicsOptionsEnums[0]);

//
// Class hkGdcPhysicsOptions
//
static const hkInternalClassMember hkGdcPhysicsOptionsClass_Members[] =
{
	{ "gravity", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcPhysicsOptions,m_gravity), HK_NULL },
	{ "throw", HK_NULL, hkGdcPhysicsOptionsObjectsEnum, hkClassMember::TYPE_ENUM, hkClassMember::TYPE_INT8, 0, 0, HK_OFFSET_OF(hkGdcPhysicsOptions,m_throw), HK_NULL },
	{ "massMultiplier", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcPhysicsOptions,m_massMultiplier), HK_NULL },
	{ "maxNumBodies", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcPhysicsOptions,m_maxNumBodies), HK_NULL },
	{ "launchVelocity", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcPhysicsOptions,m_launchVelocity), HK_NULL },
	{ "shotStrength", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcPhysicsOptions,m_shotStrength), HK_NULL },
	{ "brickWallWidth", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcPhysicsOptions,m_brickWallWidth), HK_NULL },
	{ "brickWallHeight", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcPhysicsOptions,m_brickWallHeight), HK_NULL },
	{ "rebuildWall", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcPhysicsOptions,m_rebuildWall), HK_NULL }
};
const hkClass hkGdcPhysicsOptionsClass(
	"hkGdcPhysicsOptions",
	HK_NULL, // parent
	sizeof(hkGdcPhysicsOptions),
	HK_NULL,
	0, // interfaces
	reinterpret_cast<const hkClassEnum*>(hkGdcPhysicsOptionsEnums),
	1, // enums
	reinterpret_cast<const hkClassMember*>(hkGdcPhysicsOptionsClass_Members),
	HK_COUNT_OF(hkGdcPhysicsOptionsClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(hkGdcPhysicsOptions);

//
// Enum hkGdcMiscOptions::FileCommand
//
static const hkInternalClassEnumItem hkGdcMiscOptionsFileCommandEnumItems[] =
{
	{0, "NONE"},
	{1, "SAVE"},
	{2, "LOAD"},
	{3, "DEFAULT"},
};
static const hkInternalClassEnum hkGdcMiscOptionsEnums[] = {
	{"FileCommand", hkGdcMiscOptionsFileCommandEnumItems, 4, HK_NULL, 0 }
};
extern const hkClassEnum* hkGdcMiscOptionsFileCommandEnum = reinterpret_cast<const hkClassEnum*>(&hkGdcMiscOptionsEnums[0]);

//
// Class hkGdcMiscOptions
//
static const hkInternalClassMember hkGdcMiscOptionsClass_Members[] =
{
	{ "walkToRunSpeed", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkGdcMiscOptions,m_walkToRunSpeed), HK_NULL },
	{ "settings", HK_NULL, hkGdcMiscOptionsFileCommandEnum, hkClassMember::TYPE_ENUM, hkClassMember::TYPE_INT8, 0, 0, HK_OFFSET_OF(hkGdcMiscOptions,m_settings), HK_NULL }
};
const hkClass hkGdcMiscOptionsClass(
	"hkGdcMiscOptions",
	HK_NULL, // parent
	sizeof(hkGdcMiscOptions),
	HK_NULL,
	0, // interfaces
	reinterpret_cast<const hkClassEnum*>(hkGdcMiscOptionsEnums),
	1, // enums
	reinterpret_cast<const hkClassMember*>(hkGdcMiscOptionsClass_Members),
	HK_COUNT_OF(hkGdcMiscOptionsClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(hkGdcMiscOptions);

//
// Class Gdc2005DemoOptions
//
static const hkInternalClassMember Gdc2005DemoOptionsClass_Members[] =
{
	{ "Display", &hkGdcDisplayOptionsClass, HK_NULL, hkClassMember::TYPE_STRUCT, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(Gdc2005DemoOptions,m_Display), HK_NULL },
	{ "Proxy", &hkGdcProxyOptionsClass, HK_NULL, hkClassMember::TYPE_STRUCT, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(Gdc2005DemoOptions,m_Proxy), HK_NULL },
	{ "IK", &hkGdcIkOptionsClass, HK_NULL, hkClassMember::TYPE_STRUCT, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(Gdc2005DemoOptions,m_IK), HK_NULL },
	{ "Matching", &hkGdcMatchOptionsClass, HK_NULL, hkClassMember::TYPE_STRUCT, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(Gdc2005DemoOptions,m_Matching), HK_NULL },
	{ "GetUp", &hkGdcGetupOptionsClass, HK_NULL, hkClassMember::TYPE_STRUCT, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(Gdc2005DemoOptions,m_GetUp), HK_NULL },
	{ "GetHit", &hkGdcGetHitOptionsClass, HK_NULL, hkClassMember::TYPE_STRUCT, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(Gdc2005DemoOptions,m_GetHit), HK_NULL },
	{ "Physics", &hkGdcPhysicsOptionsClass, HK_NULL, hkClassMember::TYPE_STRUCT, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(Gdc2005DemoOptions,m_Physics), HK_NULL },
	{ "Misc", &hkGdcMiscOptionsClass, HK_NULL, hkClassMember::TYPE_STRUCT, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(Gdc2005DemoOptions,m_Misc), HK_NULL },
	{ "interactiveTweaking", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(Gdc2005DemoOptions,m_interactiveTweaking), HK_NULL }
};
extern const hkClass Gdc2005DemoOptionsClass;
const hkClass Gdc2005DemoOptionsClass(
	"Gdc2005DemoOptions",
	HK_NULL, // parent
	sizeof(Gdc2005DemoOptions),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(Gdc2005DemoOptionsClass_Members),
	HK_COUNT_OF(Gdc2005DemoOptionsClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(Gdc2005DemoOptions);

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
