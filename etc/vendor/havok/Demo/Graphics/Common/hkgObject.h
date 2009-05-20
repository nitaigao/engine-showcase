/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_OBJECT
#define HK_GRAPHICS_OBJECT

#include <Graphics/Common/hkgMemory.h>

/// This is the base class for most of the objects in the graphics library.
/// It implements reference counting and also is memory managed by hkBase.
class hkgReferencedObject : public hkReferencedObject
{
	public:

		HKG_DECLARE_CLASS_ALLOCATOR(  );


		hkgReferencedObject() {}
			/// Not allowed call delete directly on a referenced object.
			/// Call release instead. This will be made explicit in future.
		inline virtual ~hkgReferencedObject();

			/// Increment the reference count.
		inline void reference();
			
			/// Decrement the reference count. If the reference count is < 1 then	
			/// the object will be deleted automatically via the virtual destructor above.
		inline void release();  
};

#include <Graphics/Common/hkgObject.inl>

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
