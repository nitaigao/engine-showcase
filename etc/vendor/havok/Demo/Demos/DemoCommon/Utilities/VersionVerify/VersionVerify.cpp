/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Common/Serialize/Util/hkVersionCheckingUtils.h>
#include <Common/Serialize/Version/hkVersionRegistry.h>
#include <Common/Serialize/Util/hkStaticClassNameRegistry.h>
#include <Common/Base/System/Io/Writer/hkStreamWriter.h>
#include <Common/Compat/hkHavokAllClasses.h>
#include <Common/Compat/hkHavokAllClassUpdates.h>
 
struct VersionVerifyVariant
{
	const char*		m_name;
	bool			m_verbose;
	const char*		m_details;
};

static const VersionVerifyVariant g_variants[] =
{
	{ "Verify versioning",				false, "Verify versioning entries"},
	{ "Verify versioning (verbose)",	true, "Verify versioning entries and summarize changes"}
};

struct VersionVerify : public hkDefaultDemo
{
	VersionVerify(hkDemoEnvironment* env) : hkDefaultDemo(env), m_done(false) { }

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

hkDemo::Result VersionVerify::stepDemo()
{
	hkReferencedObject::lockAll();
	if( m_done == false )
	{
		const hkClassNameRegistry* oldClassReg;
		const hkClassNameRegistry* newClassReg;
		m_done = true;
		TeeStream teeStream;
		teeStream.add( hkcout.getStreamWriter() );
		hkOstream disk("VersionVerify.txt");
		teeStream.add( disk.getStreamWriter() );
		hkOstream demoOutput(&teeStream);

		hkError::getInstance().setEnabled(0x786cb087, false);
#		define HK_CLASS_UPDATE_INFO(OLD,NEW) \
		demoOutput << "***** " #OLD " -> " #NEW "*****\n"; \
		oldClassReg = hkVersionRegistry::getInstance().getClassNameRegistry(hkHavok##OLD##Classes::VersionString); \
		HK_ASSERT(0, oldClassReg); \
		newClassReg = hkVersionRegistry::getInstance().getClassNameRegistry(hkHavok##NEW##Classes::VersionString); \
		HK_ASSERT(0, newClassReg); \
		hkVersionCheckingUtils::verifyUpdateDescription( demoOutput, *oldClassReg, *newClassReg, hkCompat_hk##OLD##_hk##NEW ::hkVersionUpdateDescription, g_variants[m_variantId].m_verbose ? hkVersionCheckingUtils::VERBOSE : hkVersionCheckingUtils::NONE );
#		include <Common/Compat/hkCompatVersions.h>
#		undef HK_CLASS_UPDATE_INFO

		hkError::getInstance().setEnabled(0x786cb087, true);
		demoOutput << "*****    DONE    *****\n";
	}
	hkReferencedObject::unlockAll();
	return DEMO_OK;
}

namespace 
{
	struct NullStream : public hkStreamWriter
	{
		int write(const void* buf, int nbytes)	{ return nbytes; }
		hkBool isOk() const { return true; }
	};

}

hkResult CheckVersioning()
{
	NullStream dummy;
	hkOstream output(&dummy); 
	const hkClassNameRegistry* oldClassReg;
	const hkClassNameRegistry* newClassReg;

	hkError::getInstance().setEnabled(0x786cb087, false);
	int result = 0; // Success

#	define HK_CLASS_UPDATE_INFO(OLD,NEW) \
	oldClassReg = hkVersionRegistry::getInstance().getClassNameRegistry(hkHavok##OLD##Classes::VersionString); \
	HK_ASSERT(0, oldClassReg); \
	newClassReg = hkVersionRegistry::getInstance().getClassNameRegistry(hkHavok##NEW##Classes::VersionString); \
	HK_ASSERT(0, newClassReg); \
	result |= hkVersionCheckingUtils::verifyUpdateDescription( output, *oldClassReg, *newClassReg, hkCompat_hk##OLD##_hk##NEW ::hkVersionUpdateDescription, hkVersionCheckingUtils::IGNORE_REMOVED );
#	include <Common/Compat/hkCompatVersions.h>
#	undef HK_CLASS_UPDATE_INFO

	return (hkResult)result;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( VersionVerify, HK_DEMO_TYPE_OTHER, VersionVerifyVariant, g_variants, "Verify versioning entries" );


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
