/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// WARNING: THIS FILE IS GENERATED. EDITS WILL BE LOST.
// Generated from 'Demos/Common/Api/CustomAttributes/ExampleAttributes.h'
#include <Demos/demos.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>
#include <Common/Base/Reflection/hkTypeInfo.h>
#include <Demos/Common/Api/CustomAttributes/ExampleAttributes.h>



// External pointer and enum types
extern const hkClass ExampleIntListAttributeintClass;
extern const hkClass ExampleStringPairsAttributeItemClass;

//
// Class ExampleBooleanAttribute
//
static const hkInternalClassMember ExampleBooleanAttributeClass_Members[] =
{
	{ "bool", HK_NULL, HK_NULL, hkClassMember::TYPE_BOOL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(ExampleBooleanAttribute,m_bool), HK_NULL }
};
extern const hkClass ExampleBooleanAttributeClass;
const hkClass ExampleBooleanAttributeClass(
	"ExampleBooleanAttribute",
	HK_NULL, // parent
	sizeof(ExampleBooleanAttribute),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(ExampleBooleanAttributeClass_Members),
	HK_COUNT_OF(ExampleBooleanAttributeClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(ExampleBooleanAttribute);

//
// Class ExampleStringAttribute
//
static const hkInternalClassMember ExampleStringAttributeClass_Members[] =
{
	{ "str", HK_NULL, HK_NULL, hkClassMember::TYPE_CSTRING, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(ExampleStringAttribute,m_str), HK_NULL }
};
extern const hkClass ExampleStringAttributeClass;
const hkClass ExampleStringAttributeClass(
	"ExampleStringAttribute",
	HK_NULL, // parent
	sizeof(ExampleStringAttribute),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(ExampleStringAttributeClass_Members),
	HK_COUNT_OF(ExampleStringAttributeClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(ExampleStringAttribute);

//
// Class ExampleIntListAttribute
//
static const hkInternalClassMember ExampleIntListAttributeClass_Members[] =
{
	{ "items", HK_NULL, HK_NULL, hkClassMember::TYPE_SIMPLEARRAY, hkClassMember::TYPE_INT32, 0, 0, HK_OFFSET_OF(ExampleIntListAttribute,m_items), HK_NULL }
};
extern const hkClass ExampleIntListAttributeClass;
const hkClass ExampleIntListAttributeClass(
	"ExampleIntListAttribute",
	HK_NULL, // parent
	sizeof(ExampleIntListAttribute),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(ExampleIntListAttributeClass_Members),
	HK_COUNT_OF(ExampleIntListAttributeClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(ExampleIntListAttribute);

//
// Class ExampleStringPairsAttribute::Item
//
static const hkInternalClassMember ExampleStringPairsAttribute_ItemClass_Members[] =
{
	{ "key", HK_NULL, HK_NULL, hkClassMember::TYPE_CSTRING, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(ExampleStringPairsAttribute::Item,m_key), HK_NULL },
	{ "value", HK_NULL, HK_NULL, hkClassMember::TYPE_CSTRING, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(ExampleStringPairsAttribute::Item,m_value), HK_NULL }
};
const hkClass ExampleStringPairsAttributeItemClass(
	"ExampleStringPairsAttributeItem",
	HK_NULL, // parent
	sizeof(ExampleStringPairsAttribute::Item),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(ExampleStringPairsAttribute_ItemClass_Members),
	HK_COUNT_OF(ExampleStringPairsAttribute_ItemClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SCOPED_SIMPLE(ExampleStringPairsAttribute,Item);

//
// Class ExampleStringPairsAttribute
//
static const hkInternalClassMember ExampleStringPairsAttributeClass_Members[] =
{
	{ "items", &ExampleStringPairsAttributeItemClass, HK_NULL, hkClassMember::TYPE_SIMPLEARRAY, hkClassMember::TYPE_STRUCT, 0, 0, HK_OFFSET_OF(ExampleStringPairsAttribute,m_items), HK_NULL }
};
extern const hkClass ExampleStringPairsAttributeClass;
const hkClass ExampleStringPairsAttributeClass(
	"ExampleStringPairsAttribute",
	HK_NULL, // parent
	sizeof(ExampleStringPairsAttribute),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(ExampleStringPairsAttributeClass_Members),
	HK_COUNT_OF(ExampleStringPairsAttributeClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(ExampleStringPairsAttribute);

//
// Enum ExampleStringRole::Role
//
static const hkInternalClassEnumItem ExampleStringRoleRoleEnumItems[] =
{
	{0, "ROLE_FOLDERNAME"},
	{1, "ROLE_FILENAME"},
	{2, "ROLE_IDENTIFIER"},
	{3, "ROLE_JUST_A_STRING"},
};
static const hkInternalClassEnum ExampleStringRoleEnums[] = {
	{"Role", ExampleStringRoleRoleEnumItems, 4, HK_NULL, 0 }
};
extern const hkClassEnum* ExampleStringRoleRoleEnum = reinterpret_cast<const hkClassEnum*>(&ExampleStringRoleEnums[0]);

//
// Class ExampleStringRole
//
static const hkInternalClassMember ExampleStringRoleClass_Members[] =
{
	{ "role", HK_NULL, ExampleStringRoleRoleEnum, hkClassMember::TYPE_ENUM, hkClassMember::TYPE_INT8, 0, 0, HK_OFFSET_OF(ExampleStringRole,m_role), HK_NULL }
};
extern const hkClass ExampleStringRoleClass;
const hkClass ExampleStringRoleClass(
	"ExampleStringRole",
	HK_NULL, // parent
	sizeof(ExampleStringRole),
	HK_NULL,
	0, // interfaces
	reinterpret_cast<const hkClassEnum*>(ExampleStringRoleEnums),
	1, // enums
	reinterpret_cast<const hkClassMember*>(ExampleStringRoleClass_Members),
	HK_COUNT_OF(ExampleStringRoleClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(ExampleStringRole);

//
// Enum ExampleRangeReal::Valid
//
static const hkInternalClassEnumItem ExampleRangeRealValidEnumItems[] =
{
	{1, "VALID_ABSMIN"},
	{2, "VALID_ABSMAX"},
	{4, "VALID_SOFTMIN"},
	{8, "VALID_SOFTMAX"},
};
static const hkInternalClassEnum ExampleRangeRealEnums[] = {
	{"Valid", ExampleRangeRealValidEnumItems, 4, HK_NULL, 0 }
};
extern const hkClassEnum* ExampleRangeRealValidEnum = reinterpret_cast<const hkClassEnum*>(&ExampleRangeRealEnums[0]);

//
// Class ExampleRangeReal
//
static const hkInternalClassMember ExampleRangeRealClass_Members[] =
{
	{ "valid", HK_NULL, ExampleRangeRealValidEnum, hkClassMember::TYPE_FLAGS, hkClassMember::TYPE_UINT32, 0, 0, HK_OFFSET_OF(ExampleRangeReal,m_valid), HK_NULL },
	{ "absmin", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(ExampleRangeReal,m_absmin), HK_NULL },
	{ "absmax", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(ExampleRangeReal,m_absmax), HK_NULL },
	{ "softmin", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(ExampleRangeReal,m_softmin), HK_NULL },
	{ "softmax", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(ExampleRangeReal,m_softmax), HK_NULL }
};
extern const hkClass ExampleRangeRealClass;
const hkClass ExampleRangeRealClass(
	"ExampleRangeReal",
	HK_NULL, // parent
	sizeof(ExampleRangeReal),
	HK_NULL,
	0, // interfaces
	reinterpret_cast<const hkClassEnum*>(ExampleRangeRealEnums),
	1, // enums
	reinterpret_cast<const hkClassMember*>(ExampleRangeRealClass_Members),
	HK_COUNT_OF(ExampleRangeRealClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(ExampleRangeReal);

//
// Class ExampleRawByteData
//
static const hkInternalClassMember ExampleRawByteDataClass_Members[] =
{
	{ "data", HK_NULL, HK_NULL, hkClassMember::TYPE_CSTRING, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(ExampleRawByteData,m_data), HK_NULL },
	{ "dataSize", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(ExampleRawByteData,m_dataSize), HK_NULL }
};
extern const hkClass ExampleRawByteDataClass;
const hkClass ExampleRawByteDataClass(
	"ExampleRawByteData",
	HK_NULL, // parent
	sizeof(ExampleRawByteData),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(ExampleRawByteDataClass_Members),
	HK_COUNT_OF(ExampleRawByteDataClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(ExampleRawByteData);

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
