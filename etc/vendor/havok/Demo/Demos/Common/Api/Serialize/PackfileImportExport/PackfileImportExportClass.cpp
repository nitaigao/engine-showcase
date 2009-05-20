/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// WARNING: THIS FILE IS GENERATED. EDITS WILL BE LOST.
// Generated from 'Demos/Common/Api/Serialize/PackfileImportExport/PackfileImportExport.h'
#include <Demos/demos.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>
#include <Common/Base/Reflection/hkTypeInfo.h>
#include <Demos/Common/Api/Serialize/PackfileImportExport/PackfileImportExport.h>



// External pointer and enum types
extern const hkClass hkReferencedObjectClass;
extern const hkClass hkpPhysicsDataClass;

//
// Class PackfileImportExport::ReferencedObjectArray
//
static const hkInternalClassMember PackfileImportExport_ReferencedObjectArrayClass_Members[] =
{
	{ "array", &hkReferencedObjectClass, HK_NULL, hkClassMember::TYPE_ARRAY, hkClassMember::TYPE_POINTER, 0, 0, HK_OFFSET_OF(PackfileImportExport::ReferencedObjectArray,m_array), HK_NULL }
};
extern const hkClass PackfileImportExportReferencedObjectArrayClass;
const hkClass PackfileImportExportReferencedObjectArrayClass(
	"PackfileImportExportReferencedObjectArray",
	HK_NULL, // parent
	sizeof(PackfileImportExport::ReferencedObjectArray),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(PackfileImportExport_ReferencedObjectArrayClass_Members),
	HK_COUNT_OF(PackfileImportExport_ReferencedObjectArrayClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SCOPED_NONVIRTUAL(PackfileImportExport,ReferencedObjectArray);

//
// Enum PackfileImportExport::Options::FileOrder
//
static const hkInternalClassEnumItem PackfileImportExportOptionsFileOrderEnumItems[] =
{
	{0, "FILE_ORDER_SHAPES_THEN_BODIES"},
	{1, "FILE_ORDER_BODIES_THEN_SHAPES"},
};

//
// Enum PackfileImportExport::Options::FileFormat
//
static const hkInternalClassEnumItem PackfileImportExportOptionsFileFormatEnumItems[] =
{
	{0, "FILE_FORMAT_XML"},
	{1, "FILE_FORMAT_BINARY"},
};
static const hkInternalClassEnum PackfileImportExportOptionsEnums[] = {
	{"FileOrder", PackfileImportExportOptionsFileOrderEnumItems, 2, HK_NULL, 0 },
	{"FileFormat", PackfileImportExportOptionsFileFormatEnumItems, 2, HK_NULL, 0 }
};
extern const hkClassEnum* PackfileImportExportOptionsFileOrderEnum = reinterpret_cast<const hkClassEnum*>(&PackfileImportExportOptionsEnums[0]);
extern const hkClassEnum* PackfileImportExportOptionsFileFormatEnum = reinterpret_cast<const hkClassEnum*>(&PackfileImportExportOptionsEnums[1]);

//
// Class PackfileImportExport::Options
//
static const hkInternalClassMember PackfileImportExport_OptionsClass_Members[] =
{
	{ "loadOrder", HK_NULL, PackfileImportExportOptionsFileOrderEnum, hkClassMember::TYPE_ENUM, hkClassMember::TYPE_INT8, 0, 0, HK_OFFSET_OF(PackfileImportExport::Options,m_loadOrder), HK_NULL },
	{ "shapesFormat", HK_NULL, PackfileImportExportOptionsFileFormatEnum, hkClassMember::TYPE_ENUM, hkClassMember::TYPE_INT8, 0, 0, HK_OFFSET_OF(PackfileImportExport::Options,m_shapesFormat), HK_NULL },
	{ "bodiesFormat", HK_NULL, PackfileImportExportOptionsFileFormatEnum, hkClassMember::TYPE_ENUM, hkClassMember::TYPE_INT8, 0, 0, HK_OFFSET_OF(PackfileImportExport::Options,m_bodiesFormat), HK_NULL },
	{ "reload", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(PackfileImportExport::Options,m_reload), HK_NULL }
};
extern const hkClass PackfileImportExportOptionsClass;
const hkClass PackfileImportExportOptionsClass(
	"PackfileImportExportOptions",
	HK_NULL, // parent
	sizeof(PackfileImportExport::Options),
	HK_NULL,
	0, // interfaces
	reinterpret_cast<const hkClassEnum*>(PackfileImportExportOptionsEnums),
	2, // enums
	reinterpret_cast<const hkClassMember*>(PackfileImportExport_OptionsClass_Members),
	HK_COUNT_OF(PackfileImportExport_OptionsClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SCOPED_SIMPLE(PackfileImportExport,Options);

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
