/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKG_SYSTEM_H
#define HKG_SYSTEM_H

/// The graphics subsystem class.
class hkgSystem
{
	public:

			/// Ensure hkBaseSystem::init has been called first.
		static void HK_CALL init(const char* renderer=0);
		static void HK_CALL quit();

		enum HKG_RENDERER
		{
			HKG_RENDERER_NONE = 0, // Not initialized yet
			HKG_RENDERER_NULL, // Null Display : does everything except actually send anything to the cards etc.
			HKG_RENDERER_OGL,  // PC, Linux, Mac etc, uses whatever it likes in OpenGL
			HKG_RENDERER_OGLS,  // PC, OpenGL but only using shaders (the PS3 renderer on PC)
			HKG_RENDERER_DX8,  // PC (Xbox is DX8 too, but reported as Console)
			HKG_RENDERER_DX9,  // PC uses whatever it likes in DX9
			HKG_RENDERER_DX9S,  // PC, DX9 Shaders only (the Xbox360 renderer on PC) 
			HKG_RENDERER_DX10,  // Vista, DX10 (which is always shaders only)
			HKG_RENDERER_CONSOLE // PS2, XBOX, GameCube etc
		};

		// The API being used
		enum HKG_RENDERER_SUBSYSTEM_API
		{
			HKG_RENDERER_SUBSYSTEM_NONE = 0, 
			HKG_RENDERER_SUBSYSTEM_NULL, // No subsystem 
			HKG_RENDERER_SUBSYSTEM_DIRECTX8,// Xbox, PC 
			HKG_RENDERER_SUBSYSTEM_DIRECTX9,// Xbox360, PC
			HKG_RENDERER_SUBSYSTEM_DIRECTX10,// PC (Vista)
			HKG_RENDERER_SUBSYSTEM_OPENGL, // PC, PS3, Unix, Mac
			HKG_RENDERER_SUBSYSTEM_OTHER // PS2, GameCube, Wii, PSP
		};


		static hkEnum<HKG_RENDERER, hkUint8> g_RendererType;

		static const char* HK_CALL getRendererString();
		static HKG_RENDERER_SUBSYSTEM_API HK_CALL getRendererSubSystemAPI();


	private:

		hkgSystem();
};	

#endif // HKG_SYSTEM_H

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
