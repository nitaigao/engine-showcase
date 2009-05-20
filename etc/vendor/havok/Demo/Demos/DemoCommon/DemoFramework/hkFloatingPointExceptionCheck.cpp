/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/DemoCommon/DemoFramework/hkDemo.h>
#include <Demos/demos.h>

#include <Demos/DemoCommon/DemoFramework/hkDemo.h>
#include <Common/Base/Config/hkConfigSimd.h>
#include <Demos/DemoCommon/DemoFramework/hkFloatingPointExceptionCheck.h>

#if defined(HK_PLATFORM_PC) || defined(HK_PLATFORM_XBOX) || defined(HK_PLATFORM_XBOX360)
	#include <float.h>
#endif

#	define CHECK_FPU_EXCEPTIONS

#if defined(CHECK_FPU_EXCEPTIONS)
	static hkBool g_checkFPU = false;
	static unsigned int g_checkFPUStack[255];
	static int  g_checkFPUStackIndex = 0;
#endif

int hkGetFPUState()
{
#if defined(CHECK_FPU_EXCEPTIONS)
		#if (HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED)
			return _MM_GET_EXCEPTION_STATE( );
		#else
			return _controlfp( 0, 0 );
		#endif
#else
	return 0;
#endif
}

void hkSetFPUState(int cw, bool reset = false)
{
#if defined(CHECK_FPU_EXCEPTIONS)
#		if (HK_CONFIG_SIMD == HK_CONFIG_SIMD_ENABLED)
			_MM_SET_EXCEPTION_STATE( cw );
#		else
			if (!reset)
			{
				_controlfp( cw, MCW_EM );
			}
			else
			{
				_controlfp( cw, 0xfffff );
			}
#		endif
#endif
}


void HK_CALL hkPushFPUState(unsigned int state, hkBool addToCurrentState)
{
#if defined(CHECK_FPU_EXCEPTIONS)
	// Get the current state
	int cw = hkGetFPUState();

	// Push the current state onto the stack
	HK_ASSERT2(0x6c9871d4, g_checkFPUStackIndex < 255, "FPU check overflow");
	g_checkFPUStack[g_checkFPUStackIndex] = cw;
	g_checkFPUStackIndex++;
	
	// Set the control word.
	if (addToCurrentState)
	{
		// Set the current state preserving the current state
		cw &=~( state );
	}
	else
	{
		// overwrite current state with new state
		cw = state;
	}
	hkSetFPUState(cw);
#endif
}

void HK_CALL hkPopFPUState()
{
#if defined(CHECK_FPU_EXCEPTIONS)
	g_checkFPUStackIndex--;
	HK_ASSERT2(0x7511c51f, g_checkFPUStackIndex >= 0, "FPU check underflow");
	int cw = g_checkFPUStack[g_checkFPUStackIndex];
	
	// set the state back
	hkSetFPUState( cw );
#endif
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
