/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

/// Simple boolean value.
struct ExampleBooleanAttribute
{
	HK_DECLARE_REFLECTION();
	hkBool m_bool;
};
extern const hkClass ExampleBooleanAttributeClass;


/// Simple string value.
struct ExampleStringAttribute
{
	HK_DECLARE_REFLECTION();
	const char* m_str;
};
extern const hkClass ExampleStringAttributeClass;


/// List of integers.
struct ExampleIntListAttribute
{
	HK_DECLARE_REFLECTION();
	const int* m_items;
	int m_numItems;
};
extern const hkClass ExampleIntListAttributeClass;


/// List of string pairs.
struct ExampleStringPairsAttribute
{
	HK_DECLARE_REFLECTION();
	struct Item
	{
		HK_DECLARE_REFLECTION();
		const char* m_key;
		const char* m_value;
	};
	const struct Item* m_items;
	int m_numItems;
};
extern const hkClass ExampleStringPairsAttributeClass;


/// Defines what this string semantically represents.
struct ExampleStringRole
{
	enum Role
	{
		ROLE_FOLDERNAME,
		ROLE_FILENAME,
		ROLE_IDENTIFIER,
		ROLE_JUST_A_STRING,
	};
	HK_DECLARE_REFLECTION();
	hkEnum<Role, hkInt8> m_role;
};
extern const hkClass ExampleStringRoleClass;


/// The range of acceptable values for a hkReal.
struct ExampleRangeReal
{
	HK_DECLARE_REFLECTION();
	enum Valid
	{
		VALID_ABSMIN = 1,
		VALID_ABSMAX = 2,
		VALID_SOFTMIN = 4,
		VALID_SOFTMAX = 8
	};
	hkFlags<Valid, hkUint32> m_valid; // some fields may not be specified
	hkReal m_absmin; // absolute min
	hkReal m_absmax; // absolute min
	hkReal m_softmin; // suggested min
	hkReal m_softmax; // suggested max
};
extern const hkClass ExampleRangeRealClass;



struct ExampleRawByteData
{
	HK_DECLARE_REFLECTION();
	const char* m_data;
	int m_dataSize;
};
extern const hkClass ExampleRawByteDataClass;

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
