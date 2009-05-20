/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// WARNING: THIS FILE IS GENERATED. EDITS WILL BE LOST.
// Generated from 'Demos/Common/Api/CustomAttributes/ClassWithAttributes.h'
#include <Demos/demos.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>
#include <Common/Base/Reflection/hkTypeInfo.h>
#include <Demos/Common/Api/CustomAttributes/ClassWithAttributes.h>



//
// Class ClassWithAttributes
//
// ClassWithAttributes_mass attributes
static const ExampleStringAttribute ClassWithAttributes_mass_ExampleIconPath = { "resources/icons/heavy.png" };
static const hkInternalCustomAttributes::Attribute ClassWithAttributes_mass_AttributesList[] =
{
	{ "Example.IconPath", &ClassWithAttributes_mass_ExampleIconPath, &ExampleStringAttributeClass }
};
static const hkInternalCustomAttributes ClassWithAttributes_mass_Attributes = { ClassWithAttributes_mass_AttributesList, HK_COUNT_OF(ClassWithAttributes_mass_AttributesList) };
// ClassWithAttributes_restitution attributes
static const ExampleStringAttribute ClassWithAttributes_restitution_ExampleGroupWith = { "mass" };
static const ExampleRangeReal ClassWithAttributes_restitution_ExampleRangeReal = { ExampleRangeReal::VALID_ABSMIN|ExampleRangeReal::VALID_ABSMAX|ExampleRangeReal::VALID_SOFTMAX, 0.0f,1.8f,0.0f,1.0f };
static const hkInternalCustomAttributes::Attribute ClassWithAttributes_restitution_AttributesList[] =
{
	{ "Example.GroupWith", &ClassWithAttributes_restitution_ExampleGroupWith, &ExampleStringAttributeClass },
	{ "Example.RangeReal", &ClassWithAttributes_restitution_ExampleRangeReal, &ExampleRangeRealClass }
};
static const hkInternalCustomAttributes ClassWithAttributes_restitution_Attributes = { ClassWithAttributes_restitution_AttributesList, HK_COUNT_OF(ClassWithAttributes_restitution_AttributesList) };
// ClassWithAttributes_pathToResource attributes
static const ExampleStringRole ClassWithAttributes_pathToResource_ExampleStringRole = { ExampleStringRole::ROLE_FILENAME };
static const ExampleStringPairsAttribute::Item ClassWithAttributes_pathToResource_ExampleFilename_items[] = { {"globpattern","*.hk*:*.xml"}, {"storeAs","relative"} };
static const ExampleStringPairsAttribute ClassWithAttributes_pathToResource_ExampleFilename = { ClassWithAttributes_pathToResource_ExampleFilename_items, HK_COUNT_OF(ClassWithAttributes_pathToResource_ExampleFilename_items) };
static const hkInternalCustomAttributes::Attribute ClassWithAttributes_pathToResource_AttributesList[] =
{
	{ "Example.StringRole", &ClassWithAttributes_pathToResource_ExampleStringRole, &ExampleStringRoleClass },
	{ "Example.Filename", &ClassWithAttributes_pathToResource_ExampleFilename, &ExampleStringPairsAttributeClass }
};
static const hkInternalCustomAttributes ClassWithAttributes_pathToResource_Attributes = { ClassWithAttributes_pathToResource_AttributesList, HK_COUNT_OF(ClassWithAttributes_pathToResource_AttributesList) };
// ClassWithAttributes_name attributes
extern const ExampleRawByteData IconData_Identifier;
static const ExampleRawByteData& ClassWithAttributes_name_ExampleIconData = IconData_Identifier;
static const ExampleStringRole ClassWithAttributes_name_ExampleStringRole = { ExampleStringRole::ROLE_IDENTIFIER };
static const hkInternalCustomAttributes::Attribute ClassWithAttributes_name_AttributesList[] =
{
	{ "Example.IconData", &ClassWithAttributes_name_ExampleIconData, &ExampleRawByteDataClass },
	{ "Example.StringRole", &ClassWithAttributes_name_ExampleStringRole, &ExampleStringRoleClass }
};
static const hkInternalCustomAttributes ClassWithAttributes_name_Attributes = { ClassWithAttributes_name_AttributesList, HK_COUNT_OF(ClassWithAttributes_name_AttributesList) };
// ClassWithAttributes attributes
static const ExampleBooleanAttribute ClassWithAttributes_ExampleVisibleInTool = { true };
static const ExampleStringAttribute ClassWithAttributes_ExampleIconPath = { "resources/icons/CustomAttributes.png" };
static const int ClassWithAttributes_ExampleGuiLayoutOrder_items[] = { 4, 0, 1 };
static const ExampleIntListAttribute ClassWithAttributes_ExampleGuiLayoutOrder = { ClassWithAttributes_ExampleGuiLayoutOrder_items, HK_COUNT_OF(ClassWithAttributes_ExampleGuiLayoutOrder_items) };
static const hkInternalCustomAttributes::Attribute ClassWithAttributes_AttributesList[] =
{
	{ "Example.VisibleInTool", &ClassWithAttributes_ExampleVisibleInTool, &ExampleBooleanAttributeClass },
	{ "Example.IconPath", &ClassWithAttributes_ExampleIconPath, &ExampleStringAttributeClass },
	{ "Example.GuiLayoutOrder", &ClassWithAttributes_ExampleGuiLayoutOrder, &ExampleIntListAttributeClass }
};
static const hkInternalCustomAttributes ClassWithAttributes_Attributes = { ClassWithAttributes_AttributesList, HK_COUNT_OF(ClassWithAttributes_AttributesList) };
static const hkInternalClassMember ClassWithAttributesClass_Members[] =
{
	{ "mass", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(ClassWithAttributes,m_mass), &ClassWithAttributes_mass_Attributes },
	{ "restitution", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(ClassWithAttributes,m_restitution), &ClassWithAttributes_restitution_Attributes },
	{ "pathToResource", HK_NULL, HK_NULL, hkClassMember::TYPE_CSTRING, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(ClassWithAttributes,m_pathToResource), &ClassWithAttributes_pathToResource_Attributes },
	{ "name", HK_NULL, HK_NULL, hkClassMember::TYPE_CSTRING, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(ClassWithAttributes,m_name), &ClassWithAttributes_name_Attributes }
};
extern const hkClass ClassWithAttributesClass;
const hkClass ClassWithAttributesClass(
	"ClassWithAttributes",
	HK_NULL, // parent
	sizeof(ClassWithAttributes),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(ClassWithAttributesClass_Members),
	HK_COUNT_OF(ClassWithAttributesClass_Members),
	HK_NULL, // defaults
	reinterpret_cast<const hkCustomAttributes*>(&ClassWithAttributes_Attributes),
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(ClassWithAttributes);

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
