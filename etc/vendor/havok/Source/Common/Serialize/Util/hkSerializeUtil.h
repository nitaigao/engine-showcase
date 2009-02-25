/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SERIALIZE_UTIL_H
#define HK_SERIALIZE_UTIL_H

class hkResource;
class hkStreamReader;

/// hkSerializeUtil utility functions
namespace hkSerializeUtil
{
		/// Load serialized objects from stream and return pointer
		/// to hkResource object.
		/// To access hkResource objects you should call hkResource::getContents()
		/// or hkResource::getContentsWithRegistry() specifying type of top level
		/// object pointer you expect.
		/// hkResource is reference counted and users are responsible to
		/// remove reference when they do not need the hkResource content.
		/// The objects owned by hkResource are not reference counted. 
	hkResource* HK_CALL load( hkStreamReader* sr );

		/// Load serialized objects from a file using name and return pointer
		/// to hkResource object.
		/// See hkSerializeUtil::load( hkStreamReader* sr ) for details.
	hkResource* HK_CALL load( const char* filename );

		/// Load serialized objects from buffer and return pointer
		/// to hkResource object.
		/// See hkSerializeUtil::load( hkStreamReader* sr ) for details.
	hkResource* HK_CALL load( const void* buf, int buflen );
}

#endif // HK_SERIALIZE_UTIL_H


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
