/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// Havok TweakDemo

//
// Includes
//
#include <Demos/demos.h>

#include <Demos/DemoCommon/Utilities/Tweaker/Tweak.h>
#include <Demos/DemoCommon/Utilities/GameUtils/TweakerUtils.h>

#include <Demos/DemoCommon/DemoFramework/hkTextDisplay.h>
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Graphics/Common/Input/Pad/hkgPad.h>

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkClassMember.h>
#include <Graphics/Common/Font/hkgFont.h>

class TweakDemo : public hkDemo
{
	public:

		TweakDemo(hkDemoEnvironment* env);
		~TweakDemo();

		Result stepDemo();
		Result stepVisualDebugger(){ return DEMO_OK; }
		bool visualDebuggerEnabled() { return false; }
		void makeFakeInput() {}

		hkDemoEnvironment* m_env;
		
		hkContainer m_options;

		hkString m_selected;
};

//
// Constructor
//

TweakDemo::TweakDemo(hkDemoEnvironment* env)
	:	hkDemo(env)
{
	m_selected = "/";
}

TweakDemo::~TweakDemo()
{
}

extern const hkClass TweakMeClass;

hkDemo::Result TweakDemo::stepDemo()
{
	extern const hkClass hkContainerClass;
	const hkClass& klass = hkContainerClass;
	hkgPad* pad = m_env->m_gamePad;

	if (pad->wasButtonPressed(HKG_PAD_DPAD_DOWN))
	{
		m_selected = TweakerUtils::getNextSiblingPath(m_selected, &m_options, klass);
	}

	if (pad->wasButtonPressed(HKG_PAD_DPAD_UP))
	{
		m_selected = TweakerUtils::getPrevSiblingPath(m_selected, &m_options, klass);
	}

	if (pad->wasButtonPressed(HKG_PAD_DPAD_LEFT))
	{
		m_selected = TweakerUtils::getParentPath(m_selected, &m_options, klass);
	}

	if (pad->wasButtonPressed(HKG_PAD_DPAD_RIGHT))
	{
		m_selected = TweakerUtils::getChildPath(m_selected, &m_options, klass);
	}

	if (pad->wasButtonPressed(HKG_PAD_BUTTON_1))
	{
		TweakerUtils::tweakData(m_selected, &m_options, klass, -0.1f);
	}

	if (pad->wasButtonPressed(HKG_PAD_BUTTON_2))
	{
		TweakerUtils::tweakData(m_selected, &m_options, klass, 0.1f);
	}

	TweakerUtils::displayData(m_selected, &m_options, klass, *m_env->m_textDisplay, 10, 80);

	m_env->m_textDisplay->outputText( m_selected, 20, 400, 0xffffffff);

	return DEMO_OK;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

HK_DECLARE_DEMO(TweakDemo, HK_DEMO_TYPE_OTHER, "Tweak data interactively.", "Use the DPad to navigate and \221 and \222 to tweak");

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
