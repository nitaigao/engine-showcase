/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_BRIDGE_SHADER_LIB_H
#define HK_GRAPHICS_BRIDGE_SHADER_LIB_H

class hkgDisplayContext;
class hkgShaderCollection;
class hkxMaterial;

// The shader files used by this lib have comments near the top in the form:   
 ///:STYLE <vertexentryname> <pixelentryname> L<type><maxnum> T<numtcoords> {SHADOWMAP,DIFFUSEn,REFLECTION,BUMPn,NORMALn,DISPLACEMENTn,SPECULARn,SPECULARANDGLOSSn,OPACITYn,EMISSIVEn,REFRACTION,GLOSSn} {INSTANCED,SKINNED}
// where the texture type is ordered as per enum in hkxMaterial so agreeded final stage slot between hkx and hkg
// and where 'n' is the tcoord channel that will be used for that tex type so we can match exactly
// and where light type is D for Directional, O for Omni, S for Spot, and maxnum is the max num lights used (assumes used numLights constant to render with less).
// When you have a hkxMaterial (self describing) or demo material you would like a shader for
// on the current platform, then you can request one from this lib
// It will cache shaders and resuse etc as requested.
class hkgShaderLib : public hkReferencedObject
{
	public: 

		hkgShaderLib( hkgDisplayContext* ctx );

		virtual ~hkgShaderLib( );

			// Add a search path to find the shaders (.hlsl and .cg files)
			// eg  ./resources/shaders
		void addSearchPath( const char* path );
		void clearSearchPaths();

		enum DisplayObjectFlags
		{
			NO_FLAGS = 0,
			SKINNED = 1, // need shader to support h/w skinning
			INSTANCED = 2 // need shader to support h/w instancing
		};

		enum ExtraTextureHints
		{
			NO_HINTS = 0,
			LIGHTMAP = 1, // one of the diffuse textures is a lightmap
			USE_ALPHA_SHADOWS = 2, // use the shadow shader that has alpha enabled shadow map (slower smap creation)
			USE_DIFFUSE_ONLY = 4 // usually used in 'low fi / fast graphics' versions, so laptops faster etc 
		};

			/// Get a shader collection for the given material
			/// Will normal give back at least 3 sets (2 for shadow map passes, and 1 for no shadows) 
			/// Will return NULL if none found that match
			/// Pass 0 for numLights for unlit shaders
		hkgShaderCollection* findShader( const hkxMaterial* sourceFileMaterial, DisplayObjectFlags objectFlags = NO_FLAGS, ExtraTextureHints textureHints = NO_HINTS, int numLights = 1);

		enum WellKnownShaderType
		{
			DIFFUSE_ONLY_T1,
			DIFFUSE_NORMAL_T1,
			DIFFUSE_NORMAL_SPECULAR_T1, // Most common asset: Diffuse, Normal and Specular maps all share tcoord0
			DIFFUSE_DIFFUSE_NORMAL_SPECULAR_T2, // Second diffuse uses tcoord1, usually a dirt map
			DIFFUSE_LIGHTMAP_NORMAL_SPECULAR_T2, // Most common prelit scene asset: Diffuse texture, Normal map and Specular share tcoord0, LightMap uses tcoord1. Shadow depends on LightMap luminance 
			DIFFUSE_NORMAL_DISPLACEMENT_SPECULAR_T1 // For usual high quality assets: Diffuse texture, normal and specular map for lighting, displacement heightfield for surface detail
		};

		hkgShaderCollection* findShader( WellKnownShaderType t, DisplayObjectFlags objectFlags = NO_FLAGS, ExtraTextureHints textureHints = NO_HINTS, int numLights = 1 );

	public:

		class ShaderMatcher
		{
		public:

			virtual ~ShaderMatcher() {}

			virtual bool styleMatch( const char* shaderFileStyle, hkgShaderLib::DisplayObjectFlags objectFlags, hkgShaderLib::ExtraTextureHints textureHints, int numLights ) const = 0;
		};

	protected:

		hkgShaderCollection* findShaderMatch( const ShaderMatcher& matchFunc, DisplayObjectFlags objectFlags, ExtraTextureHints textureHints, int numLights );

		void createDepthShaders();

		hkgDisplayContext* m_context;

		hkObjectArray< hkString > m_searchPaths;
		hkArray< struct ShaderCacheInfo* > m_shaderCache; // shader pairs already created

		hkgShader* m_toDepthVShader;
		hkgShader* m_toDepthVShaderInstanced;
		hkgShader* m_toDepthVShaderSkinned;
		hkgShader* m_toDepthPShader;
};

#endif

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
