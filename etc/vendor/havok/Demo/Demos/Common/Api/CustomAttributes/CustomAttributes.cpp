/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Demos/DemoCommon/DemoFramework/hkDefaultDemo.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Demos/Common/Api/CustomAttributes/ExampleAttributes.h>

// "External" data referenced by ClassWithAttributes.m_name.IconData
const char IconData_Identifier_Data[] = { 1,2,3,4 };
extern const ExampleRawByteData IconData_Identifier = { IconData_Identifier_Data, HK_COUNT_OF(IconData_Identifier_Data) };


template <typename AttrType, typename Object>
AttrType* hkExtractAttribute(Object* objPtr, const char* attrName, const char* attrType)
{
	if( objPtr )
	{
		if( const hkVariant* v = objPtr->getAttribute(attrName) )
		{
			HK_ASSERT( 0x0, hkString::strCmp( v->m_class->getName(), attrType ) == 0 );
			return static_cast<AttrType*>(v->m_object); \
		}
	}
	return HK_NULL;
}
#define HK_EXTRACT_ATTRIBUTE(AttrType, varName, objPtr, attrName) \
	AttrType* varName = hkExtractAttribute<AttrType>(objPtr, attrName, #AttrType)

class CustomAttributesDemo : public hkDefaultDemo
{
	public:

		CustomAttributesDemo(hkDemoEnvironment* env) : hkDefaultDemo(env) {}

		Result stepDemo()
		{
			extern const hkClass ClassWithAttributesClass;
			const hkClass* k = &ClassWithAttributesClass;

			// longhand attribute access
			if( const hkVariant* v = k->getAttribute("Example.GuiLayoutOrder") )
			{
				ExampleIntListAttribute* attr = static_cast<ExampleIntListAttribute*>(v->m_object);
				attr;
			}
			if( const hkVariant* v = k->getMemberByName("name")->getAttribute("Example.IconData") )
			{
				ExampleRawByteData* attr = static_cast<ExampleRawByteData*>(v->m_object);
				attr;
			}

			// shorthand for the above: puts the attribute into the supplied
			// variable if it exists otherwise sets it to HK_NULL. 
			HK_EXTRACT_ATTRIBUTE(ExampleIntListAttribute, intList, k, "Example.GuiLayoutOrder");
			if( intList )
			{
				int y = intList->m_items[0] + intList->m_items[1];
				++y;
			}

			// The value of this expression can be embedded inside a conditional.
			if( HK_EXTRACT_ATTRIBUTE(ExampleRawByteData, rawData, k->getMemberByName("name"), "Example.IconData") )
			{
				int x = rawData->m_dataSize;
				x += 1;
			}
			return DEMO_OK;
		}
};


#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO(CustomAttributesDemo, HK_DEMO_TYPE_PRIME, "Custom metadata attributes", HK_NULL);

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
