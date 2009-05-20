/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// WARNING: THIS FILE IS GENERATED. EDITS WILL BE LOST.
// Generated from 'Demos/Common/Api/Serialize/Versioning/CustomClassVersioning/CustomClass.h'
#include <Demos/demos.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>
#include <Common/Base/Reflection/hkTypeInfo.h>
#include <Demos/Common/Api/Serialize/Versioning/CustomClassVersioning/CustomClass.h>



// External pointer and enum types
extern const hkClass CustomClassLocalDataClass;
extern const hkClassEnum* CustomClassDataStatusEnum;

//
// Enum CustomClass::DataStatus
//
static const hkInternalClassEnumItem CustomClassDataStatusEnumItems[] =
{
	{0, "SET_MINIMUM"},
	{1, "SET_MAXIMUM"},
};
static const hkInternalClassEnum CustomClassEnums[] = {
	{"DataStatus", CustomClassDataStatusEnumItems, 2, HK_NULL, 0 }
};
extern const hkClassEnum* CustomClassDataStatusEnum = reinterpret_cast<const hkClassEnum*>(&CustomClassEnums[0]);

//
// Class CustomClass::LocalData
//
static const hkInternalClassMember CustomClass_LocalDataClass_Members[] =
{
	{ "name", HK_NULL, HK_NULL, hkClassMember::TYPE_CSTRING, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(CustomClass::LocalData,m_name), HK_NULL },
	{ "id", HK_NULL, HK_NULL, hkClassMember::TYPE_ULONG, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(CustomClass::LocalData,m_id), HK_NULL }
};
const hkClass CustomClassLocalDataClass(
	"CustomClassLocalData",
	HK_NULL, // parent
	sizeof(CustomClass::LocalData),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(CustomClass_LocalDataClass_Members),
	HK_COUNT_OF(CustomClass_LocalDataClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SCOPED_SIMPLE(CustomClass,LocalData);

//
// Class CustomClass
//
static const hkInternalClassMember CustomClassClass_Members[] =
{
	{ "status", HK_NULL, CustomClassDataStatusEnum, hkClassMember::TYPE_ENUM, hkClassMember::TYPE_INT32, 0, 0, HK_OFFSET_OF(CustomClass,m_status), HK_NULL },
	{ "maxNum", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(CustomClass,m_maxNum), HK_NULL },
	{ "data", &CustomClassLocalDataClass, HK_NULL, hkClassMember::TYPE_ARRAY, hkClassMember::TYPE_STRUCT, 0, 0, HK_OFFSET_OF(CustomClass,m_data), HK_NULL }
};
namespace
{
	struct CustomClass_DefaultStruct
	{
		int s_defaultOffsets[3];
		typedef hkInt8 _hkBool;
		typedef hkReal _hkVector4[4];
		typedef hkReal _hkQuaternion[4];
		typedef hkReal _hkMatrix3[12];
		typedef hkReal _hkRotation[12];
		typedef hkReal _hkQsTransform[12];
		typedef hkReal _hkMatrix4[16];
		typedef hkReal _hkTransform[16];
		hkInt32 m_maxNum;
	};
	const CustomClass_DefaultStruct CustomClass_Default =
	{
		{-1,HK_OFFSET_OF(CustomClass_DefaultStruct,m_maxNum),-1},
		100
	};
}
extern const hkClass hkReferencedObjectClass;

extern const hkClass CustomClassClass;
const hkClass CustomClassClass(
	"CustomClass",
	&hkReferencedObjectClass, // parent
	sizeof(CustomClass),
	HK_NULL,
	0, // interfaces
	reinterpret_cast<const hkClassEnum*>(CustomClassEnums),
	1, // enums
	reinterpret_cast<const hkClassMember*>(CustomClassClass_Members),
	HK_COUNT_OF(CustomClassClass_Members),
	&CustomClass_Default,
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_NONVIRTUAL(CustomClass);

//
// Class CustomClass2
//
static const hkInternalClassMember CustomClass2Class_Members[] =
{
	{ "data", HK_NULL, HK_NULL, hkClassMember::TYPE_ARRAY, hkClassMember::TYPE_CSTRING, 0, 0, HK_OFFSET_OF(CustomClass2,m_data), HK_NULL }
};
extern const hkClass CustomClass2Class;
const hkClass CustomClass2Class(
	"CustomClass2",
	HK_NULL, // parent
	sizeof(CustomClass2),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(CustomClass2Class_Members),
	HK_COUNT_OF(CustomClass2Class_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_NONVIRTUAL(CustomClass2);

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
