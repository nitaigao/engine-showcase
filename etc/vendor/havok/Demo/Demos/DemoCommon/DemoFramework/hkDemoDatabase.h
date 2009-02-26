/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEMOFRAMEWORK_DEMODATABASE_H
#define HK_DEMOFRAMEWORK_DEMODATABASE_H

#include <Demos/DemoCommon/DemoFramework/hkDemo.h>
#include <Common/Base/Container/Array/hkObjectArray.h>


// Demo creation function
typedef hkDemo* (HK_CALL *DemoCreationFunction)(class hkDemoEnvironment* env);

enum DemoType
{
	HK_DEMO_TYPE_MENU			= 1,
	HK_DEMO_TYPE_PRIME			= 1<<1,
	HK_DEMO_TYPE_PHYSICS		= 1<<2,
	HK_DEMO_TYPE_ANIMATION		= 1<<3,
	HK_DEMO_TYPE_COMPLETE		= 1<<4,
	HK_DEMO_TYPE_BEHAVIOR		= 1<<5,
	HK_DEMO_TYPE_TEST			= 1<<6,
	HK_DEMO_TYPE_SERIALIZE		= 1<<7,
	HK_DEMO_TYPE_CLOTH			= 1<<8,
	HK_DEMO_TYPE_DESTRUCTION	= 1<<9,
	HK_DEMO_TYPE_OTHER			= 1<<11,
	HK_DEMO_TYPE_CRITICAL		= 1<<12,
	HK_DEMO_TYPE_STATS			= 1<<13,
};

// Demo entry register
struct hkDemoEntryRegister
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO, hkDemoEntryRegister );

	// for multiple entries
	hkDemoEntryRegister() {}

	// for single entries
	hkDemoEntryRegister(DemoCreationFunction func, int type, const char* path,
						int variant, const char* variantName,
						const char* help, const char* details, bool actuallyReg = true);

	// for variants
	hkDemoEntryRegister(DemoCreationFunction func, int type, const char* path,
						hkDemoEntryRegister* entries, int numVariants, int sizeOfVariantStruct,
						const char*const* variantNames, const char* help, const char*const* details, bool actuallyReg = true);

	// for non auto register demos (from libs etc)
	void registerDemo();

	DemoCreationFunction	m_func;
	int						m_demoTypeFlags;
	const char*				m_name;
	const char*				m_demoPath;
	int						m_variantId;
	const char*				m_variantName;
	const char*				m_help;
	const char*				m_details;
	hkDemoEntryRegister*	m_next;
};


// Demo database
class hkDemoDatabase : public hkReferencedObject, public hkSingleton<hkDemoDatabase>
{
	public:
		hkDemoDatabase();

		void rebuildDatabase();

		int findDemo(const char* name) const;
		hkDemo* createDemo(const char* name, hkDemoEnvironment* env) const;
		hkDemo* createDemo(int index, hkDemoEnvironment* env) const;

		struct hkDemoEntry;
		const hkObjectArray<hkDemoEntry>& getDemos() const { return m_demos; }

		const hkString& getPrefix() const { return m_prefix; }

		/***********
		* INTERNAL *
		***********/

		// Registered demos are formatted into an array of Entries
		struct hkDemoEntry
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO_FRAMEWORK, hkDemoEntry);
			DemoCreationFunction m_func;
			int 		m_demoTypeFlags;
			hkString	m_menuPath;
			hkString	m_demoPath;
			int			m_variantId;
			const char* m_help;
			const char* m_details;
		};

		hkObjectArray<hkDemoEntry> m_demos;
		hkString m_prefix;


		// Demo registration
		static hkDemoEntryRegister* s_demoList;
};


// Demo declaration macros

// declare a single demo: the name, a help string, the path including the file name and the help details
// (-1 is used for the variant id so that 0,1,2,... identify actual variants)
#define HK_DECLARE_DEMO_RENAME(DEMO_CLASS, DEMO_TYPE, HELP, DETAILS, DEMO_NAME) \
	static hkDemo* HK_CALL hkCreate ## DEMO_CLASS(hkDemoEnvironment* env) { return new DEMO_CLASS(env); } \
	hkDemoEntryRegister hkDemoEntryRegister##DEMO_CLASS (&hkCreate##DEMO_CLASS, DEMO_TYPE, DEMO_NAME, -1, HK_NULL, HELP, DETAILS, true)

#define HK_DECLARE_DEMO(DEMO_CLASS, DEMO_TYPE, HELP, DETAILS) HK_DECLARE_DEMO_RENAME(DEMO_CLASS, DEMO_TYPE, HELP, DETAILS, __FILE__)

#define HK_DECLARE_DEMO_NO_REG(DEMO_CLASS, DEMO_TYPE, HELP, DETAILS) \
	static hkDemo* HK_CALL hkCreate ## DEMO_CLASS(hkDemoEnvironment* env) { return new DEMO_CLASS(env); } \
	hkDemoEntryRegister hkDemoEntryRegister##DEMO_CLASS (&hkCreate##DEMO_CLASS, DEMO_TYPE, __FILE__, -1, HK_NULL, HELP, DETAILS, false)


// You can declare a demo several times using variants. The hkDemoEnvironment has a member m_variantId, which you can query to
// find which variant is used.
// Note: the VARIANT_ID must be a simple integer only. The parameter VARIANT_ID is also used as an internal identifier,
//       so 'VARIANT_ID' must be a simple integer as well.

#define HK_DECLARE_DEMO_VARIANT_RENAME(DEMO_CLASS, DEMO_TYPE, VARIANT_NAME, VARIANT_ID, HELP, DETAILS, DEMO_NAME) \
	static hkDemo* HK_CALL hkCreate ## DEMO_CLASS ## VARIANT_ID(hkDemoEnvironment* env) { return new DEMO_CLASS(env); } \
	hkDemoEntryRegister hkDemoEntryRegister##DEMO_CLASS##VARIANT_ID						\
	(&hkCreate##DEMO_CLASS##VARIANT_ID, DEMO_TYPE, DEMO_NAME, VARIANT_ID, VARIANT_NAME, HELP, DETAILS, true)

#define HK_DECLARE_DEMO_VARIANT(DEMO_CLASS, DEMO_TYPE, VARIANT_NAME, VARIANT_ID, HELP, DETAILS) HK_DECLARE_DEMO_VARIANT_RENAME(DEMO_CLASS, DEMO_TYPE, VARIANT_NAME, VARIANT_ID, HELP, DETAILS, __FILE__)

#define HK_DECLARE_DEMO_VARIANT_NO_REG(DEMO_CLASS, DEMO_TYPE, VARIANT_NAME, VARIANT_ID, HELP, DETAILS) \
	static hkDemo* HK_CALL hkCreate ## DEMO_CLASS ## VARIANT_ID(hkDemoEnvironment* env) { return new DEMO_CLASS(env); } \
	hkDemoEntryRegister hkDemoEntryRegister##DEMO_CLASS##VARIANT_ID						\
		(&hkCreate##DEMO_CLASS##VARIANT_ID, DEMO_TYPE, __FILE__, VARIANT_ID, VARIANT_NAME, HELP, DETAILS, false)


// If your variant want to use a struct to identify the variant, you can declare a list of variants with this
// call.
//      - VARIANT_STRUCT is the type of structure holding the variant information
//      - VARIANT_TABLE is an array of VARIANT_STRUCT
// See the pyramid demo to see how this is done


#define HK_DECLARE_DEMO_VARIANT_USING_STRUCT_RENAME(DEMO_CLASS, DEMO_TYPE, VARIANT_STRUCT, VARIANT_TABLE, HELP, DEMO_NAME)					\
	static hkDemo* HK_CALL hkCreate ## DEMO_CLASS(hkDemoEnvironment* env) { return new DEMO_CLASS(env); }							\
	hkDemoEntryRegister hkDemoEntryRegisterTable##DEMO_CLASS[ sizeof(VARIANT_TABLE)/sizeof(VARIANT_STRUCT) ];	\
	hkDemoEntryRegister hkDemoEntryRegister##DEMO_CLASS												\
	(&hkCreate##DEMO_CLASS, DEMO_TYPE, DEMO_NAME, &hkDemoEntryRegisterTable##DEMO_CLASS[0], sizeof(VARIANT_TABLE)/sizeof(VARIANT_STRUCT), sizeof(VARIANT_STRUCT), &VARIANT_TABLE[0].m_name, HELP, &VARIANT_TABLE[0].m_details )

#define HK_DECLARE_DEMO_VARIANT_USING_STRUCT(DEMO_CLASS, DEMO_TYPE, VARIANT_STRUCT, VARIANT_TABLE, HELP) HK_DECLARE_DEMO_VARIANT_USING_STRUCT_RENAME( DEMO_CLASS, DEMO_TYPE, VARIANT_STRUCT, VARIANT_TABLE, HELP, __FILE__ )


#endif // HK_DEMOFRAMEWORK_DEMODATABASE_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20080925)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2008
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
