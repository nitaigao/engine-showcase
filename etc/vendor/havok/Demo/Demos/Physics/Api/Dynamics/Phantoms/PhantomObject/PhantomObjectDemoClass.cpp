/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// WARNING: THIS FILE IS GENERATED. EDITS WILL BE LOST.
// Generated from 'Demos/Physics/Api/Dynamics/Phantoms/PhantomObject/PhantomObjectDemo.h'
#include <Demos/demos.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>
#include <Common/Base/Reflection/hkTypeInfo.h>
#include <Demos/Physics/Api/Dynamics/Phantoms/PhantomObject/PhantomObjectDemo.h>



// External pointer and enum types
extern const hkClass hkpAabbPhantomClass;

//
// Class PhantomObjectDemo::Options
//
static const hkInternalClassMember PhantomObjectDemo_OptionsClass_Members[] =
{
	{ "numberOfBoxes", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(PhantomObjectDemo::Options,m_numberOfBoxes), HK_NULL },
	{ "aabbWidth", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(PhantomObjectDemo::Options,m_aabbWidth), HK_NULL }
};
namespace
{
	struct PhantomObjectDemoOptions_DefaultStruct
	{
		int s_defaultOffsets[2];
		typedef hkInt8 _hkBool;
		typedef hkReal _hkVector4[4];
		typedef hkReal _hkQuaternion[4];
		typedef hkReal _hkMatrix3[12];
		typedef hkReal _hkRotation[12];
		typedef hkReal _hkQsTransform[12];
		typedef hkReal _hkMatrix4[16];
		typedef hkReal _hkTransform[16];
		int m_numberOfBoxes;
		int m_aabbWidth;
	};
	const PhantomObjectDemoOptions_DefaultStruct PhantomObjectDemoOptions_Default =
	{
		{HK_OFFSET_OF(PhantomObjectDemoOptions_DefaultStruct,m_numberOfBoxes),HK_OFFSET_OF(PhantomObjectDemoOptions_DefaultStruct,m_aabbWidth)},
		70,2
	};
}
extern const hkClass PhantomObjectDemoOptionsClass;
const hkClass PhantomObjectDemoOptionsClass(
	"PhantomObjectDemoOptions",
	HK_NULL, // parent
	sizeof(PhantomObjectDemo::Options),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(PhantomObjectDemo_OptionsClass_Members),
	HK_COUNT_OF(PhantomObjectDemo_OptionsClass_Members),
	&PhantomObjectDemoOptions_Default,
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SCOPED_SIMPLE(PhantomObjectDemo,Options);

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
