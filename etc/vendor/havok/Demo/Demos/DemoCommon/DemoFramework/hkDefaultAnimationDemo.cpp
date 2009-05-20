/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/DemoCommon/DemoFramework/hkDemo.h>

#include <Demos/demos.h>
#include <Demos/DemoCommon/DemoFramework/hkDefaultAnimationDemo.h>

#include <Common/Base/System/Io/FileSystem/hkFileSystem.h>
#include <Common/Base/System/Io/Reader/hkStreamReader.h>
#include <Common/Base/System/Stopwatch/hkStopwatch.h>
#include <Common/Base/Reflection/hkClass.h>

#include <Graphics/Common/hkGraphics.h>
#include <Graphics/Common/Input/Pad/hkgPad.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/Camera/hkgCamera.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/DisplayWorld/hkgDisplayWorld.h>
#include <Graphics/Common/Light/hkgLightManager.h>

#include <Common/Serialize/Util/hkBuiltinTypeRegistry.h>
#include <Common/Base/Reflection/Registry/hkVtableClassRegistry.h>

#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>

#include <Common/Visualize/Process/hkDebugDisplayProcess.h>
#include <Common/Visualize/hkProcess.h>
#include <Common/Visualize/hkProcessRegisterUtil.h>
#include <Common/Visualize/hkVisualDebugger.h>
#include <Common/Visualize/hkProcessFactory.h>


hkDefaultAnimationDemo::hkDefaultAnimationDemo(hkDemoEnvironment* env)
: hkDefaultDemo	(env)
	//m_animationViewersContext(HK_NULL)
{
// eg:
//	m_animationViewersContext= new hkAnimationContext;
//	hkAnimationContext::registerAllAnimationProcesses(); // all physics only ones

	// Most of the default animations hover above the ground, so we don't want shadows
	// until they are reauthored
	forceShadowState(false);

	setupLights(m_env); // after we decide if we want shadows

	// Disable compression & mapping reports
	if(m_env->m_reportingLevel < hkDemoEnvironment::REPORT_INFO )
	{
		hkError::getInstance().setEnabled(0x432434a4, false); // Track analysis report
		hkError::getInstance().setEnabled(0x432434a5, false); // Delta/wavelet constructor report
		hkError::getInstance().setEnabled(0x54e4323e, false); // hkaSkeletonMapperUtils::createMapping report
		hkError::getInstance().setEnabled(0xf0d1e423, false); // 'Could not realize an inplace texture of type PNG.'
		hkError::getInstance().setEnabled(0x36118e94, false); // Spline constructor report
	}
}

hkDefaultAnimationDemo::~hkDefaultAnimationDemo()
{
// eg:
//	if (m_animationViewersContext)
//	{
//		m_animationViewersContext->removeReference();
//		m_animationViewersContext = HK_NULL;
//	}

}

hkDemo::Result hkDefaultAnimationDemo::stepDemo()
{
	// stepDeltaTime(m_timestep);

	return DEMO_OK;
}

void hkDefaultAnimationDemo::setupContexts(hkArray<hkProcessContext*>& contexts)
{

// eg:
//	m_animationViewersContext->addAnim(m_anim);
//	contexts.pushBack( m_animationViewersContext );

	// Add viewers to the demo display.
	// Uncomment these to use them.

	//	m_debugViewerNames.pushBack( hkStatisticsProcess::getName()  );
	m_debugViewerNames.pushBack( hkDebugDisplayProcess::getName() );

	// register all our classes we know about with the vdb for tweaking
	if (m_vdbClassReg)
		m_vdbClassReg->registerList(hkBuiltinTypeRegistry::StaticLinkedTypeInfos, hkBuiltinTypeRegistry::StaticLinkedClasses);
}

hkBool hkDefaultAnimationDemo::objectPicked( const hkgDisplayObject* displayObject, const hkVector4& worldPosition, int geomIndex )
{
	HK_ASSERT(0x65b2643b, m_env->m_displayHandler);

	/* hkgDisplayHandler* dhandler = m_env->m_displayHandler; */

	return false;
}

void hkDefaultAnimationDemo::objectDragged( const hkVector4& newWorldSpacePoint )
{

}

void hkDefaultAnimationDemo::objectReleased()
{

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
