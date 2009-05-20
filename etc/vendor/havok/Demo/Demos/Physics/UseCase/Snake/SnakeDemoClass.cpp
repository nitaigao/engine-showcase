/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// WARNING: THIS FILE IS GENERATED. EDITS WILL BE LOST.
// Generated from 'Demos/Physics/UseCase/Snake/SnakeDemo.h'
#include <Demos/demos.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>
#include <Common/Base/Reflection/hkTypeInfo.h>
#include <Demos/Physics/UseCase/Snake/SnakeDemo.h>



// External pointer and enum types
extern const hkClass hkpHingeConstraintDataClass;
extern const hkClass hkpLimitedHingeConstraintDataClass;

//
// Class SnakeDemo::Options
//
static const hkInternalClassMember SnakeDemo_OptionsClass_Members[] =
{
	{ "waveLength", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(SnakeDemo::Options,m_waveLength), HK_NULL },
	{ "wavePeriod", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(SnakeDemo::Options,m_wavePeriod), HK_NULL },
	{ "waveAmplitude", HK_NULL, HK_NULL, hkClassMember::TYPE_REAL, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(SnakeDemo::Options,m_waveAmplitude), HK_NULL }
};
namespace
{
	struct SnakeDemoOptions_DefaultStruct
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
		hkReal m_waveLength;
		hkReal m_wavePeriod;
		hkReal m_waveAmplitude;
	};
	const SnakeDemoOptions_DefaultStruct SnakeDemoOptions_Default =
	{
		{HK_OFFSET_OF(SnakeDemoOptions_DefaultStruct,m_waveLength),HK_OFFSET_OF(SnakeDemoOptions_DefaultStruct,m_wavePeriod),HK_OFFSET_OF(SnakeDemoOptions_DefaultStruct,m_waveAmplitude)},
		4,1.5,1
	};
}
extern const hkClass SnakeDemoOptionsClass;
const hkClass SnakeDemoOptionsClass(
	"SnakeDemoOptions",
	HK_NULL, // parent
	sizeof(SnakeDemo::Options),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(SnakeDemo_OptionsClass_Members),
	HK_COUNT_OF(SnakeDemo_OptionsClass_Members),
	&SnakeDemoOptions_Default,
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SCOPED_SIMPLE(SnakeDemo,Options);

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
