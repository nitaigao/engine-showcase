/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/Asset/ResourceLinker.h>
#include <Common/Serialize/Resource/hkResource.h>

#	define TRACE(A)

ResourceLinker::~ResourceLinker()
{
	reset();
}

void ResourceLinker::reset()
{
	hkPointerMap<hkResource*, int>::Iterator it;
	for( it = m_resources.getIterator(); m_resources.isValid(it); it = m_resources.getNext(it) )
	{
		hkResource* r = m_resources.getKey(it);
		if( r->getReferenceCount() == 1 )
		{
			r->callDestructors();
		}
	}
	for( it = m_resources.getIterator(); m_resources.isValid(it); it = m_resources.getNext(it) )
	{
		m_resources.getKey(it)->removeReference();
	}
	m_resources.clear();
	m_resolved.clear();
	m_dangling.clear();
	m_backMap.clear();
}

void ResourceLinker::add( hkResource* resource )
{
	TRACE(("ADD BUNDLE %s\n", resource->getName() ));
	resource->addReference();
	int resourceId = m_resources.getSize(); //XXX not unique
	m_resources.insert(resource, resourceId);

	hkArray<hkResource::Export> exports;
	hkArray<hkResource::Import> imports;
	resource->getImportsExports( imports, exports );

	for( int i = 0; i < exports.getSize(); ++i )
	{
		TRACE(("\tEXPORT %s\n", exports[i].name));
		HK_ASSERT2( 0, m_resolved.hasKey(exports[i].name) == false, "duplicate symbol" );
		hkStringMap<int>::Iterator it = m_dangling.findKey(exports[i].name);
		if( m_dangling.isValid(it) )
		{
			TRACE(("\tHIT PENDING %s\n", exports[i].name));
			int index = m_dangling.getValue(it);
			m_dangling.remove( it );
			m_backMap.realizePendingKey( exports[i].data, index );
			for( ; index != -1; index = m_backMap.getNextIndex(index) )
			{
				const Reference& ref = m_backMap.getValue(index);
				HK_ASSERT(0x7fe24edd, *ref.loc== HK_NULL );
				*ref.loc = exports[i].data;
			}
		}
		m_resolved.insert( exports[i].name, exports[i].data );
	}
	
	for( int i = 0; i < imports.getSize(); ++i )
	{
		hkResource::Import & import = imports[i];
		Reference ref = { import.location, resource };
		if( void* data = m_resolved.getWithDefault( import.name, HK_NULL ) )
		{
			HK_ASSERT(0, m_dangling.hasKey(import.name)==false);
			TRACE(("\tIMP RESOLVED %s\n", import.name));
			*(import.location) = data;
			m_backMap.insert( data, ref );
		}
		else
		{
			TRACE(("\tIMP PENDING %s\n", import.name));
			int prev = m_dangling.getWithDefault( import.name, -1 );
			m_dangling.insert( import.name,
					m_backMap.addPendingValue( ref, prev ) );
		}
	}
}

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
