/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef DEMO_RESOURCE_LINKER_H
#define DEMO_RESOURCE_LINKER_H

#include <Common/Base/Container/PointerMap/hkPointerMap.h>
#include <Common/Base/Container/StringMap/hkStringMap.h>
#include <Common/Serialize/Util/hkPointerMultiMap.h>

class hkResource;

// Simple class to link resource by putting all identifiers into a global namespace.
class ResourceLinker
{
	public:

			// Destroy all resources in use.
		~ResourceLinker();

			// Destroy and remove reference to all resources.
		void reset();

			// Unload a single resource, sets pointers to objects in
			// this resource to null.
		//void unload( hkResource* resource ); // todo

			// Add a new resource. Resolves imports/exports against existing resources.
		void add( hkResource* resource );

	//protected:

			// Bag of resources
		hkPointerMap<hkResource*, int> m_resources;

			// id -> object
		hkStringMap<void*> m_resolved;

			// id -> pending index in backMap
		hkStringMap<int> m_dangling;

			//
		struct Reference
		{
			void** loc;
			hkResource* resource;
		};

			// object -> list of things which reference it.
		hkPointerMultiMap<void*, Reference> m_backMap;
};

#endif // DEMO_RESOURCE_LINKER_H

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
