/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CUSTOMCLASS_H
#define HK_CUSTOMCLASS_H

#include <Common/Serialize/Packfile/hkPackfileReader.h>

class CustomClass : public hkReferencedObject
{
public:
	HK_DECLARE_REFLECTION();

	CustomClass() {}

	enum DataStatus
	{
		SET_MINIMUM,
		SET_MAXIMUM
	};

	struct LocalData
	{
		HK_DECLARE_REFLECTION();
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, CustomClass::LocalData );

		const char* m_name;
		hkUlong m_id;
	};

	hkEnum<CustomClass::DataStatus, hkInt32> m_status;
	hkInt32 m_maxNum; // +default(100)
	hkArray<struct LocalData> m_data;

	CustomClass(hkFinishLoadedObjectFlag flag) : m_data(flag) {}
};

class CustomClass2
{
public:
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, CustomClass2 );

	CustomClass2() {}

	hkArray<char*> m_data;

	CustomClass2(hkFinishLoadedObjectFlag flag) : m_data(flag) {}
};

#endif // HK_CUSTOMCLASS_H

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
