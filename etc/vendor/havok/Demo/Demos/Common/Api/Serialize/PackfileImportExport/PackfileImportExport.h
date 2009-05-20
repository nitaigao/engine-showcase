/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_DEMO_SYMBOL_EXPORT_H
#define HK_DEMO_SYMBOL_EXPORT_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>
#include <Demos/DemoCommon/Utilities/Asset/ResourceLinker.h>
#include <Common/Serialize/Resource/hkResource.h>

extern const hkClass PackfileImportExportReferencedObjectArrayClass;

class PackfileImportExport : public hkDefaultPhysicsDemo
{
	public:

		struct ReferencedObjectArray
		{
			HK_DECLARE_REFLECTION();
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO, PackfileImportExport::ReferencedObjectArray );
			ReferencedObjectArray() { }
			ReferencedObjectArray(hkFinishLoadedObjectFlag f) : m_array(f) { }
			hkArray<const hkReferencedObject*> m_array;
		};

		struct Options
		{
			HK_DECLARE_REFLECTION();

			enum FileOrder
			{
				FILE_ORDER_SHAPES_THEN_BODIES = 0,
				FILE_ORDER_BODIES_THEN_SHAPES = 1
			};
			hkEnum<FileOrder,hkInt8> m_loadOrder;

			enum FileFormat
			{
				FILE_FORMAT_XML = 0,
				FILE_FORMAT_BINARY = 1
			};

			hkEnum<FileFormat,hkInt8> m_shapesFormat;
			hkEnum<FileFormat,hkInt8> m_bodiesFormat;
			int m_reload;
		};
		DEMO_OPTIONS_DECLARE(Options);

		PackfileImportExport( hkDemoEnvironment* env);
		~PackfileImportExport();
		Result stepDemo();

		void exportData();
		void cleanup();
		void setup();

			// A handle to the original physics data object.
		class hkpPhysicsData* m_physicsData;

			// 
		ResourceLinker m_linker;
};


#endif //HK_DEMO_SYMBOL_EXPORT_H

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
