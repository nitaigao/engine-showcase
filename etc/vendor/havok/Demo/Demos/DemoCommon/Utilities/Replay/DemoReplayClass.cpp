/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// WARNING: THIS FILE IS GENERATED. EDITS WILL BE LOST.
// Generated from 'Demos/DemoCommon/Utilities/Replay/DemoReplay.h'
#include <Demos/demos.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>
#include <Common/Base/Reflection/hkTypeInfo.h>
#include <Demos/DemoCommon/Utilities/Replay/DemoReplay.h>



// External pointer and enum types
extern const hkClass ReplayDataFrameClass;
extern const hkClass hkBitFieldClass;
extern const hkClass hkDemoReplayUtilityCameraClass;
extern const hkClass hkDemoReplayUtilityMouseCallbacksClass;
extern const hkClass hkDemoReplayUtilityReplayDataClass;
extern const hkClass hkPackfileDataClass;

//
// Class hkDemoReplayUtility::Camera
//
static const hkInternalClassMember hkDemoReplayUtility_CameraClass_Members[] =
{
	{ "from", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 3, 0, HK_OFFSET_OF(hkDemoReplayUtility::Camera,m_from), HK_NULL },
	{ "to", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 3, 0, HK_OFFSET_OF(hkDemoReplayUtility::Camera,m_to), HK_NULL },
	{ "up", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 3, 0, HK_OFFSET_OF(hkDemoReplayUtility::Camera,m_up), HK_NULL }
};
const hkClass hkDemoReplayUtilityCameraClass(
	"hkDemoReplayUtilityCamera",
	HK_NULL, // parent
	sizeof(hkDemoReplayUtility::Camera),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(hkDemoReplayUtility_CameraClass_Members),
	HK_COUNT_OF(hkDemoReplayUtility_CameraClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SCOPED_SIMPLE(hkDemoReplayUtility,Camera);

//
// Class hkDemoReplayUtility::Frame
//
static const hkInternalClassMember hkDemoReplayUtility_FrameClass_Members[] =
{
	{ "keysDown", &hkBitFieldClass, HK_NULL, hkClassMember::TYPE_STRUCT, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkDemoReplayUtility::Frame,m_keysDown), HK_NULL },
	{ "camera", &hkDemoReplayUtilityCameraClass, HK_NULL, hkClassMember::TYPE_STRUCT, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkDemoReplayUtility::Frame,m_camera), HK_NULL },
	{ "mouseX", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkDemoReplayUtility::Frame,m_mouseX), HK_NULL },
	{ "mouseY", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkDemoReplayUtility::Frame,m_mouseY), HK_NULL },
	{ "padButtons", HK_NULL, HK_NULL, hkClassMember::TYPE_UINT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkDemoReplayUtility::Frame,m_padButtons), HK_NULL },
	{ "mouseButtons", HK_NULL, HK_NULL, hkClassMember::TYPE_UINT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkDemoReplayUtility::Frame,m_mouseButtons), HK_NULL },
	{ "sticks", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 4, 0, HK_OFFSET_OF(hkDemoReplayUtility::Frame,m_sticks), HK_NULL }
};
extern const hkClass hkDemoReplayUtilityFrameClass;
const hkClass hkDemoReplayUtilityFrameClass(
	"hkDemoReplayUtilityFrame",
	HK_NULL, // parent
	sizeof(hkDemoReplayUtility::Frame),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(hkDemoReplayUtility_FrameClass_Members),
	HK_COUNT_OF(hkDemoReplayUtility_FrameClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SCOPED_NONVIRTUAL(hkDemoReplayUtility,Frame);

//
// Class hkDemoReplayUtility::ReplayData
//
static const hkInternalClassMember hkDemoReplayUtility_ReplayDataClass_Members[] =
{
	{ "framesWereAllocatedExternally", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkDemoReplayUtility::ReplayData,m_framesWereAllocatedExternally), HK_NULL },
	{ "frames", &hkDemoReplayUtilityFrameClass, HK_NULL, hkClassMember::TYPE_ARRAY, hkClassMember::TYPE_POINTER, 0, 0, HK_OFFSET_OF(hkDemoReplayUtility::ReplayData,m_frames), HK_NULL },
	{ "initialMouseX", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkDemoReplayUtility::ReplayData,m_initialMouseX), HK_NULL },
	{ "initialMouseY", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkDemoReplayUtility::ReplayData,m_initialMouseY), HK_NULL }
};
extern const hkClass hkReferencedObjectClass;

const hkClass hkDemoReplayUtilityReplayDataClass(
	"hkDemoReplayUtilityReplayData",
	&hkReferencedObjectClass, // parent
	sizeof(hkDemoReplayUtility::ReplayData),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(hkDemoReplayUtility_ReplayDataClass_Members),
	HK_COUNT_OF(hkDemoReplayUtility_ReplayDataClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SCOPED_VIRTUAL(hkDemoReplayUtility,ReplayData);

//
// Class hkDemoReplayUtility::MouseCallbacks
//
static const hkInternalClassMember hkDemoReplayUtility_MouseCallbacksClass_Members[] =
{
	{ "mouseMoveFunc", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkDemoReplayUtility::MouseCallbacks,m_mouseMoveFunc), HK_NULL },
	{ "mouseWheelFunc", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkDemoReplayUtility::MouseCallbacks,m_mouseWheelFunc), HK_NULL },
	{ "mouseButtonFunc", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkDemoReplayUtility::MouseCallbacks,m_mouseButtonFunc), HK_NULL }
};
const hkClass hkDemoReplayUtilityMouseCallbacksClass(
	"hkDemoReplayUtilityMouseCallbacks",
	HK_NULL, // parent
	sizeof(hkDemoReplayUtility::MouseCallbacks),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(hkDemoReplayUtility_MouseCallbacksClass_Members),
	HK_COUNT_OF(hkDemoReplayUtility_MouseCallbacksClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SCOPED_SIMPLE(hkDemoReplayUtility,MouseCallbacks);

//
// Class hkDemoReplayUtility
//
static const hkInternalClassMember hkDemoReplayUtilityClass_Members[] =
{
	{ "replayData", &hkDemoReplayUtilityReplayDataClass, HK_NULL, hkClassMember::TYPE_POINTER, hkClassMember::TYPE_STRUCT, 0, 0, HK_OFFSET_OF(hkDemoReplayUtility,m_replayData), HK_NULL },
	{ "currentFrame", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkDemoReplayUtility,m_currentFrame), HK_NULL },
	{ "allocatedData", HK_NULL, HK_NULL, hkClassMember::TYPE_POINTER, hkClassMember::TYPE_VOID, 0, 0|hkClassMember::SERIALIZE_IGNORED, HK_OFFSET_OF(hkDemoReplayUtility,m_allocatedData), HK_NULL },
	{ "oldMouseCallbacks", &hkDemoReplayUtilityMouseCallbacksClass, HK_NULL, hkClassMember::TYPE_STRUCT, hkClassMember::TYPE_VOID, 0, 0|hkClassMember::SERIALIZE_IGNORED, HK_OFFSET_OF(hkDemoReplayUtility,m_oldMouseCallbacks), HK_NULL }
};
extern const hkClass hkDemoReplayUtilityClass;
const hkClass hkDemoReplayUtilityClass(
	"hkDemoReplayUtility",
	HK_NULL, // parent
	sizeof(hkDemoReplayUtility),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(hkDemoReplayUtilityClass_Members),
	HK_COUNT_OF(hkDemoReplayUtilityClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(hkDemoReplayUtility);

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
