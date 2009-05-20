/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_MEMORY
#define HK_GRAPHICS_MEMORY
#include <Common/Base/Memory/Memory/hkMemory.h>


	// set this define if you want hkgGraphics to use its on memory system
//#define HKG_USE_PRIVATE_MEMORY

#if defined(HKG_USE_PRIVATE_MEMORY)

	class hkgMemory  
	{
		public:
			static hkMemory& getInstance(){ return *s_instance; }
			static hkMemory* s_instance;
	};


	#define HKG_DECLARE_CLASS_ALLOCATOR_UNCHECKED(TYPE) \
		HK_FORCE_INLINE void* HK_CALL operator new(hk_size_t nbytes)	{ HK_ASSERT_OBJECT_SIZE_OK(nbytes); hkReferencedObject* b = static_cast<hkReferencedObject*>(hkgMemory::getInstance().allocateChunk(static_cast<int>(nbytes),TYPE)); b->m_memSizeAndFlags = static_cast<hkUint16>(nbytes); return b; }	\
		HK_FORCE_INLINE void  HK_CALL operator delete(void* p)			{ hkReferencedObject* b = static_cast<hkReferencedObject*>(p); hkgMemory::getInstance().deallocateChunk(p, b->m_memSizeAndFlags,TYPE); }	\
		HK_FORCE_INLINE void* HK_CALL operator new[](hk_size_t nbytes)	{ hkReferencedObject* b = static_cast<hkReferencedObject*>(hkgMemory::getInstance().allocateChunk(static_cast<int>(nbytes),TYPE)); b->m_memSizeAndFlags = static_cast<hkUint16>(nbytes); return b; }	\
		HK_FORCE_INLINE void  HK_CALL operator delete[](void* p)		{ hkReferencedObject* b = static_cast<hkReferencedObject*>(p); hkgMemory::getInstance().deallocateChunk(p, b->m_memSizeAndFlags,TYPE); }	\
		HK_FORCE_INLINE void* HK_CALL operator new(hk_size_t, void* p)	{ return p; }	\
		HK_FORCE_INLINE void* HK_CALL operator new[](hk_size_t, void* p){ HK_BREAKPOINT(0); return p; }	\
		HK_OPERATOR_DELETE \
	class MustEndWithSemiColon

	#define HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR_BY_SIZE_UNCHECKED(TYPE,CLASS_SIZE) \
		HK_FORCE_INLINE void* HK_CALL operator new(hk_size_t nbytes)	{ HK_ASSERT(0x6c787b7f, nbytes == CLASS_SIZE ); return hkgMemory::getInstance().allocateChunk(static_cast<int>(nbytes),TYPE); }	\
		HK_FORCE_INLINE void  HK_CALL operator delete(void* p)			{ if (p) hkgMemory::getInstance().deallocateChunk(p, CLASS_SIZE, TYPE); }								\
		HK_FORCE_INLINE void* HK_CALL operator new(hk_size_t n, void* p){ HK_ASSERT(0x77bb90a1, n == CLASS_SIZE); return p; } \
		HK_FORCE_INLINE void* HK_CALL operator new[](hk_size_t, void* p){ return p;	} \
		private:			\
		HK_FORCE_INLINE void* HK_CALL operator new[](hk_size_t nbytes)	{ return (void*)1; /*return hkgMemory::getInstance().allocate(static_cast<int>(nbytes),TYPE);*/ }												\
		HK_FORCE_INLINE void  HK_CALL operator delete[](void* p)		{ /*hkgMemory::getInstance().deallocate(p);*/ } \
		HK_OPERATOR_NONVIRTUAL_DELETE \
		public:				\
	class MustEndWithSemiColon

	//
	// In debug, use some compile time trickery to ensure correct allocator is used.
	// Use -Wshadow to catch nonvirtual allocators in virtual classes.
	//
	#if defined(HK_DEBUG)
	#   define HKG_DECLARE_CLASS_ALLOCATOR() \
		HK_FORCE_INLINE const hkReferencedObject* iMustBeDerivedFromReferencedObject() const { return static_cast<const hkReferencedObject*>(this); } \
		HKG_DECLARE_CLASS_ALLOCATOR_UNCHECKED(HK_MEMORY_CLASS_DISPLAY)
	#   define HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(THIS_CLASS) \
		HK_FORCE_INLINE void iShouldNotHaveVtable() const { int m_memSizeAndFlags = 0; m_memSizeAndFlags--; /* if you get this error, you derive from hkReferencedObject! */ } \
		HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR_BY_SIZE_UNCHECKED(HK_MEMORY_CLASS_DISPLAY, sizeof(THIS_CLASS))
	#else
	#   define HKG_DECLARE_CLASS_ALLOCATOR() 						HKG_DECLARE_CLASS_ALLOCATOR_UNCHECKED(HK_MEMORY_CLASS_DISPLAY)
	#   define HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(THIS_CLASS) 	HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR_BY_SIZE_UNCHECKED(HK_MEMORY_CLASS_DISPLAY, sizeof(THIS_CLASS))
	#endif


	#undef HKBASE_HKARRAY_H
	#define hkArrayUtil hkgArrayUtil
	#define hkArray hkgArray
	#define hkInplaceArray hkgInplaceArray
	#define hkInplaceArrayAligned16 hkgInplaceArrayAligned16
	#define hkArrayAllocator hkgMemory::getInstance()
	#include <Common/Base/Container/Array/hkArray.h>
	#undef hkArrayUtil
	#undef hkArray
	#undef hkInplaceArray
	#undef hkInplaceArrayAligned16
	#undef hkArrayAllocator

#else
	#define hkgArray hkArray
	#define hkgInplaceArray hkInplaceArray
	#define hkgInplaceArrayAligned16 hkInplaceArrayAligned16
	#define HKG_DECLARE_CLASS_ALLOCATOR() 						HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DISPLAY)
	#define HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(THIS_CLASS) 	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DISPLAY, THIS_CLASS)
#endif

#endif	//HK_GRAPHICS_MEMORY

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
