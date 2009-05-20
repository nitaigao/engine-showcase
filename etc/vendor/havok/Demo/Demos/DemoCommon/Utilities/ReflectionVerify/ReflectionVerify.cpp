/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Common/Serialize/Util/hkSerializationCheckingUtils.h>
#include <Common/Base/System/Io/Writer/hkStreamWriter.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.h>
#include <Common/Base/Reflection/Registry/hkClassNameRegistry.h>

struct ReflectionVerify : public hkDefaultDemo
{
	ReflectionVerify(hkDemoEnvironment* env) : hkDefaultDemo(env), m_done(false) { }

	Result stepDemo();
	Result stepVisualDebugger(){ return DEMO_OK; }

	hkBool m_done;
};

namespace
{
	struct TeeStream : public hkStreamWriter
	{
		~TeeStream()
		{
			for(int i = 0; i < m_streams.getSize(); ++i )
			{
				m_streams[i]->removeReference();
			}
		}
		void add(hkStreamWriter* writer)
		{
			m_streams.pushBack(writer);
			writer->addReference();
		}
		int write(const void* buf, int nbytes)
		{
			for(int i = 0; i < m_streams.getSize(); ++i )
			{
				m_streams[i]->write(buf, nbytes);
			}
			return nbytes;
		}
		hkBool isOk() const { return true; }
		hkArray<hkStreamWriter*> m_streams;
	};
}

hkDemo::Result ReflectionVerify::stepDemo()
{
	hkReferencedObject::lockAll();
	if( m_done == false )
	{
		m_done = true;
		TeeStream teeStream;
		teeStream.add( hkcout.getStreamWriter() );
		hkOstream disk("ReflectionVerify.txt");
		teeStream.add( disk.getStreamWriter() );
		hkOstream demoOutput(&teeStream);

		demoOutput << "** Reflection Verify **\n";
		const hkClassNameRegistry* classReg = hkBuiltinTypeRegistry::getInstance().getClassNameRegistry();
		HK_ASSERT(0x3bcd08e1, classReg);
		hkSerializationCheckingUtils::verifyReflection(*classReg, demoOutput);
		demoOutput << "*****    DONE    *****\n";
	}
	hkReferencedObject::unlockAll();
	return DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = "Verify reflection of classes";

HK_DECLARE_DEMO( ReflectionVerify, HK_DEMO_TYPE_OTHER, "Verify reflected classes", helpString );


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
