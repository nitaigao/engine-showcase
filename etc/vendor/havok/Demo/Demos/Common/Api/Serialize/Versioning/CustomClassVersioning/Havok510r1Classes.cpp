/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>

#include <Common/Compat/hkHavokAllClasses.h>

namespace hkHavok510r1Classes
{
	extern const char VersionString[];
	extern const int ClassVersion;
	extern const hkStaticClassNameRegistry CustomStaticRegistry;

	extern hkClass CustomClassLocalDataClass; /* 0x48F93E31 */
	extern hkClass CustomClassClass; /* 0xF4F71448 */
	extern hkClass hkReferencedObjectClass; /* 0x3B1C1113 */
	extern hkClass CustomClass2Class; /* 0x1C1B8F8F */

	static hkInternalClassMember CustomClassLocalDataClass_Members[] =
	{
		{ "name", HK_NULL, HK_NULL, hkClassMember::TYPE_CSTRING, hkClassMember::TYPE_VOID, 0, 0, 0, HK_NULL },
		{ "id", HK_NULL, HK_NULL, hkClassMember::TYPE_ULONG, hkClassMember::TYPE_VOID, 0, 0, 0, HK_NULL },
	};
	hkClass CustomClassLocalDataClass(
		"CustomClassLocalData",
		HK_NULL,
		0,
		HK_NULL,
		0,
		HK_NULL,
		0,
		reinterpret_cast<const hkClassMember*>(CustomClassLocalDataClass_Members),
		int(sizeof(CustomClassLocalDataClass_Members)/sizeof(hkInternalClassMember)),
		HK_NULL
	);
	static hkInternalClassMember CustomClassClass_Members[] =
	{
		{ "minNum", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, 0, HK_NULL },
		{ "maxNum", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, 0, HK_NULL },
		{ "data", &CustomClassLocalDataClass, HK_NULL, hkClassMember::TYPE_ARRAY, hkClassMember::TYPE_STRUCT, 0, 0, 0, HK_NULL },
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
			hkInt32 m_minNum;
			hkInt32 m_maxNum;
		};
		const CustomClass_DefaultStruct CustomClass_Default =
		{
			{HK_OFFSET_OF(CustomClass_DefaultStruct,m_minNum),HK_OFFSET_OF(CustomClass_DefaultStruct,m_maxNum),-1,},
			2,100,
		};
	}
	hkClass CustomClassClass(
		"CustomClass",
		&hkReferencedObjectClass,
		0,
		HK_NULL,
		0,
		HK_NULL,
		0,
		reinterpret_cast<const hkClassMember*>(CustomClassClass_Members),
		int(sizeof(CustomClassClass_Members)/sizeof(hkInternalClassMember)),
		&CustomClass_Default
	);
	static hkInternalClassMember CustomClass2Class_Members[] =
	{
		{ "data", HK_NULL, HK_NULL, hkClassMember::TYPE_ARRAY, hkClassMember::TYPE_CSTRING, 0, 0, 0, HK_NULL },
	};
	hkClass CustomClass2Class(
		"CustomClass2",
		HK_NULL,
		0,
		HK_NULL,
		0,
		HK_NULL,
		0,
		reinterpret_cast<const hkClassMember*>(CustomClass2Class_Members),
		int(sizeof(CustomClass2Class_Members)/sizeof(hkInternalClassMember)),
		HK_NULL
	);
	static hkClass* const Classes[] =
	{
		&CustomClassLocalDataClass,
		&CustomClassClass,
		&CustomClass2Class,
		HK_NULL
	};

	const hkStaticClassNameRegistry CustomStaticRegistry(Classes, ClassVersion, VersionString);
} // namespace hkHavok510r1Classes

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
