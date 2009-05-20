/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// WARNING: THIS FILE IS GENERATED. EDITS WILL BE LOST.
// Generated from 'Demos/Physics/UseCase/Machine/ConveyorBelts/ConveyorBeltsDemo.h'
#include <Demos/demos.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>
#include <Common/Base/Reflection/hkTypeInfo.h>
#include <Demos/Physics/UseCase/Machine/ConveyorBelts/ConveyorBeltsDemo.h>



//
// Enum ConveyorBeltsDemo::Options::ShapeType
//
static const hkInternalClassEnumItem ConveyorBeltsDemoOptionsShapeTypeEnumItems[] =
{
	{0, "BOX"},
	{1, "CONVEX_VERTICES"},
};
static const hkInternalClassEnum ConveyorBeltsDemoOptionsEnums[] = {
	{"ShapeType", ConveyorBeltsDemoOptionsShapeTypeEnumItems, 2, HK_NULL, 0 }
};
extern const hkClassEnum* ConveyorBeltsDemoOptionsShapeTypeEnum = reinterpret_cast<const hkClassEnum*>(&ConveyorBeltsDemoOptionsEnums[0]);

//
// Class ConveyorBeltsDemo::Options
//
static const hkInternalClassMember ConveyorBeltsDemo_OptionsClass_Members[] =
{
	{ "shapeType", HK_NULL, ConveyorBeltsDemoOptionsShapeTypeEnum, hkClassMember::TYPE_ENUM, hkClassMember::TYPE_INT8, 0, 0, HK_OFFSET_OF(ConveyorBeltsDemo::Options,m_shapeType), HK_NULL },
	{ "shapeScale", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(ConveyorBeltsDemo::Options,m_shapeScale), HK_NULL },
	{ "numBodies", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(ConveyorBeltsDemo::Options,m_numBodies), HK_NULL }
};
namespace
{
	struct ConveyorBeltsDemoOptions_DefaultStruct
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
		hkReal m_shapeScale;
		int m_numBodies;
	};
	const ConveyorBeltsDemoOptions_DefaultStruct ConveyorBeltsDemoOptions_Default =
	{
		{-1,HK_OFFSET_OF(ConveyorBeltsDemoOptions_DefaultStruct,m_shapeScale),HK_OFFSET_OF(ConveyorBeltsDemoOptions_DefaultStruct,m_numBodies)},
		.55f,200
	};
}
extern const hkClass ConveyorBeltsDemoOptionsClass;
const hkClass ConveyorBeltsDemoOptionsClass(
	"ConveyorBeltsDemoOptions",
	HK_NULL, // parent
	sizeof(ConveyorBeltsDemo::Options),
	HK_NULL,
	0, // interfaces
	reinterpret_cast<const hkClassEnum*>(ConveyorBeltsDemoOptionsEnums),
	1, // enums
	reinterpret_cast<const hkClassMember*>(ConveyorBeltsDemo_OptionsClass_Members),
	HK_COUNT_OF(ConveyorBeltsDemo_OptionsClass_Members),
	&ConveyorBeltsDemoOptions_Default,
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SCOPED_SIMPLE(ConveyorBeltsDemo,Options);

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
