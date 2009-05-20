/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// WARNING: THIS FILE IS GENERATED. EDITS WILL BE LOST.
// Generated from 'Demos/DemoCommon/Utilities/Tweaker/Tweak.h'
#include <Demos/demos.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>
#include <Common/Base/Reflection/hkTypeInfo.h>
#include <Demos/DemoCommon/Utilities/Tweaker/Tweak.h>



// External pointer and enum types
extern const hkClass hkStructClass;

//
// Enum hkStruct::Option
//
static const hkInternalClassEnumItem hkStructOptionEnumItems[] =
{
	{0, "OptionA"},
	{1, "OptionB"},
	{2, "OptionC"},
	{3, "OptionD"},
};

//
// Enum hkStruct::Bits
//
static const hkInternalClassEnumItem hkStructBitsEnumItems[] =
{
	{1/*0x1*/, "BIT_X"},
	{2/*0x2*/, "BIT_Y"},
	{3/*BIT_X|BIT_Y*/, "BIT_XY"},
	{15/*0xf*/, "BIT_Z"},
	{240/*0xf0*/, "BIT_W"},
};
static const hkInternalClassEnum hkStructEnums[] = {
	{"Option", hkStructOptionEnumItems, 4, HK_NULL, 0 },
	{"Bits", hkStructBitsEnumItems, 5, HK_NULL, 0 }
};
extern const hkClassEnum* hkStructOptionEnum = reinterpret_cast<const hkClassEnum*>(&hkStructEnums[0]);
extern const hkClassEnum* hkStructBitsEnum = reinterpret_cast<const hkClassEnum*>(&hkStructEnums[1]);

//
// Class hkStruct
//
static const hkInternalClassMember hkStructClass_Members[] =
{
	{ "int", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkStruct,m_int), HK_NULL },
	{ "unit", HK_NULL, HK_NULL, hkClassMember::TYPE_UINT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkStruct,m_unit), HK_NULL },
	{ "bool", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkStruct,m_bool), HK_NULL },
	{ "float", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkStruct,m_float), HK_NULL },
	{ "flags0", HK_NULL, hkStructBitsEnum, hkClassMember::TYPE_FLAGS, hkClassMember::TYPE_INT32, 0, 0, HK_OFFSET_OF(hkStruct,m_flags0), HK_NULL },
	{ "flags1", HK_NULL, hkStructBitsEnum, hkClassMember::TYPE_FLAGS, hkClassMember::TYPE_INT32, 0, 0, HK_OFFSET_OF(hkStruct,m_flags1), HK_NULL },
	{ "flags2", HK_NULL, hkStructBitsEnum, hkClassMember::TYPE_FLAGS, hkClassMember::TYPE_INT32, 0, 0, HK_OFFSET_OF(hkStruct,m_flags2), HK_NULL },
	{ "option", HK_NULL, hkStructOptionEnum, hkClassMember::TYPE_ENUM, hkClassMember::TYPE_INT8, 0, 0, HK_OFFSET_OF(hkStruct,m_option), HK_NULL }
};
const hkClass hkStructClass(
	"hkStruct",
	HK_NULL, // parent
	sizeof(hkStruct),
	HK_NULL,
	0, // interfaces
	reinterpret_cast<const hkClassEnum*>(hkStructEnums),
	2, // enums
	reinterpret_cast<const hkClassMember*>(hkStructClass_Members),
	HK_COUNT_OF(hkStructClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(hkStruct);

//
// Class hkContainer
//
static const hkInternalClassMember hkContainerClass_Members[] =
{
	{ "embeddedStruct", &hkStructClass, HK_NULL, hkClassMember::TYPE_STRUCT, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkContainer,m_embeddedStruct), HK_NULL },
	{ "containerInt", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkContainer,m_containerInt), HK_NULL },
	{ "ptrToStruct", &hkStructClass, HK_NULL, hkClassMember::TYPE_POINTER, hkClassMember::TYPE_STRUCT, 0, 0, HK_OFFSET_OF(hkContainer,m_ptrToStruct), HK_NULL },
	{ "arrayInt", HK_NULL, HK_NULL, hkClassMember::TYPE_ARRAY, hkClassMember::TYPE_INT32, 0, 0, HK_OFFSET_OF(hkContainer,m_arrayInt), HK_NULL },
	{ "arrayPtr", &hkStructClass, HK_NULL, hkClassMember::TYPE_ARRAY, hkClassMember::TYPE_POINTER, 0, 0, HK_OFFSET_OF(hkContainer,m_arrayPtr), HK_NULL },
	{ "arrayStruct", &hkStructClass, HK_NULL, hkClassMember::TYPE_ARRAY, hkClassMember::TYPE_STRUCT, 0, 0, HK_OFFSET_OF(hkContainer,m_arrayStruct), HK_NULL }
};
extern const hkClass hkContainerClass;
const hkClass hkContainerClass(
	"hkContainer",
	HK_NULL, // parent
	sizeof(hkContainer),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(hkContainerClass_Members),
	HK_COUNT_OF(hkContainerClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(hkContainer);

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
