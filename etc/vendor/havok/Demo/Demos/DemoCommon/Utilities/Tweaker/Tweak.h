/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEMO2_TWEAK_H
#define HK_DEMO2_TWEAK_H

struct hkStruct
{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, hkStruct);
	HK_DECLARE_REFLECTION();

	hkStruct()	:
		m_int(-5),
		m_unit(10),
		m_bool(false),
		m_float(10.54f),
		m_flags0(0),
		m_flags1(BIT_XY|BIT_W),
		m_flags2(BIT_XY|BIT_W|0x100),
		m_option(OptionA)
	{}

	enum Option {
		OptionA,
		OptionB,
		OptionC,
		OptionD
	};

	enum Bits {
		BIT_X = 0x1,
		BIT_Y = 0x2,
		BIT_XY = BIT_X|BIT_Y,
		BIT_Z = 0xf,
		BIT_W = 0xf0
	};

	int m_int;
	hkUint32 m_unit;
	bool m_bool;
	hkReal m_float;
	hkFlags<Bits,hkInt32> m_flags0;
	hkFlags<Bits,hkInt32> m_flags1;
	hkFlags<Bits,hkInt32> m_flags2;
	hkEnum<Option,hkInt8> m_option;
};

class hkContainer
{
public:
	
	HK_DECLARE_REFLECTION();

	hkContainer() : m_containerInt(0), m_arrayInt(3, 0), m_arrayPtr(3), m_arrayStruct(3)
	{
		m_ptrToStruct = new hkStruct();
		m_arrayPtr[0] = new hkStruct();
		m_arrayPtr[1] = new hkStruct();
		m_arrayPtr[2] = new hkStruct();
		m_arrayStruct[0] = hkStruct();
		m_arrayStruct[1] = hkStruct();
		m_arrayStruct[2] = hkStruct();
	}

	~hkContainer()
	{
		delete m_ptrToStruct;
		delete m_arrayPtr[0];
		delete m_arrayPtr[1];
		delete m_arrayPtr[2];
	}

	struct hkStruct m_embeddedStruct;
	
	int m_containerInt;

	struct hkStruct* m_ptrToStruct;

	hkArray<hkInt32>			m_arrayInt;	// array of integers
	hkArray<struct hkStruct*>	m_arrayPtr;	// array of pointers to hkStruct structure
	hkArray<struct hkStruct>	m_arrayStruct;	// array of hkStruct structures
};

#endif

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
