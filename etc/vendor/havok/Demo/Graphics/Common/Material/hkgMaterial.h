/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_MATERIAL_H
#define HK_GRAPHICS_MATERIAL_H

#include <Common/Base/Types/hkBaseTypes.h>
#include <Graphics/Common/Math/hkgMath.h>
#include <Graphics/Common/Texture/hkgTexture.h>
#include <Graphics/Common/Shader/hkgShaderCollection.h>
#include <Graphics/Common/Material/hkgMaterialDefines.h>

extern const hkClass hkgMaterialClass;

/// A materail has both color (diffuse and specular, although specular is not used at the moment) and
/// texture (which may be null).
class hkgMaterial : public hkgReferencedObject
{
		friend class hkgMaterialManager;

	public:
			/// Create a platform specific material.
		static hkgMaterial* (HK_CALL *create)();

			/// Set the texture for the material. Will reference count the texture. This may be set to HK_NULL.
		inline void setTexture(int i, hkgTexture* t);
		inline void addTexture(hkgTexture* t);

			/// Get the const pointer to the texture for the material. This may be HK_NULL.
		inline const hkgTexture* getTexture(int stage) const;

			/// Get the pointer to the texture for the material. This may be HK_NULL.
		inline hkgTexture* getTexture(int stage);

			/// Get the texture mode used by the given texture for this object
		inline HKG_TEXTURE_MODE getTextureMode(int stage) const;

		inline int getNumTextures() const;


			/// Modulate or Decal the texture stage blend mode.
		inline void setTextureMode( int stage, HKG_TEXTURE_MODE m );

			/// Set the shader for the material. Will reference count the shader. This may be set to HK_NULL.
		inline void setShaderCollection(hkgShaderCollection* s);

			/// Get the const pointer to the shader for the material. This may be HK_NULL.
		inline const hkgShaderCollection* getShaderCollection() const;

			/// Get the pointer to the shader for the material. This may be HK_NULL.
		inline hkgShaderCollection* getShaderCollection();

			/// Set the diffuse color for material. The values are in the range 0-->1 and the alpha
			/// value defaults to 1.0, setting it to less than that will cause the material
			/// to be translucent.
		inline void setDiffuseColor( float r, float g, float b, float a = 1.0f );

			/// Get the 4 color values (RGBA) for the material.
		inline void getDiffuseColor( float& r, float& g, float& b, float &a ) const;
		inline const float* getDiffuseColorPtr() const;

			/// Set the specular color components for the material. Alpha is not a specular part.
		inline void setSpecularColor( float r, float g, float b );

			/// Get the specular color components for the material. Alpha is not a specular part.
		inline void getSpecularColor( float& r, float& g, float& b ) const;
		inline const float* getSpecularColorPtr() const;

			/// Set the specular power exponent for the material.
		inline void setSpecularPower( float p );

			/// Get the specular power exponent for the material.
		inline float getSpecularPower() const;

			/// Get the void* user data, which can be used for tagging (with a source material pointer for instance).
		inline hkUlong getUserData() const;

			/// Set the void* user data, which can be used for tagging (with a source material pointer for instance).
		inline void setUserData(hkUlong t);

			// Call realize() in all referenced textures
		inline void realizeTextures();

			/// Bind this material as the current material (including texture and shader) in the given context. This may change state.
		virtual void bind(hkgDisplayContext* context, HKG_MATERIAL_VERTEX_HINT hints ) const;

			/// Unbind the current material. Notice that this is static as it is just a state change and
			/// will set the material back to a null state (ie, texture stage 0 to NULL, disable textures)
		static void unbind(hkgDisplayContext* context);

			/// Is the material transparent? A material is transparent if the texture is transparent
			/// or the diffuse color has non 1.0 alpha.
		inline bool isTransparent() const;

			/// Get the source for the material's transparency so that we can figure out such things as when
			/// the textures are disabled, is the material still transparent: ie, is the transparent source
			/// the diffuse color?
		inline HKG_MATERIAL_TRANS_SOURCE getTransparentSource() const;

			/// Print the material information to the given stream. It will assume the stream is text based.
		void print(hkOstream& stream) const;

			/// Is this material the same as the given other material? It will check for colors with tolerance
			/// of each other and texture pointers being the same.
		inline bool operator==( const hkgMaterial& other ) const;

			/// copy operator
		inline hkgMaterial& operator=( const hkgMaterial& other );

			/// don't modify default materials.
		inline bool isDefaultMaterial() const {return m_isDefaultMaterial; }

        // implement hkReferencedObject
        virtual const hkClass* getClassType() const { return &hkgMaterialClass; }
        virtual void calcContentStatistics(hkStatisticsCollector* collector, const hkClass* cls) const;

	protected:

		inline hkgMaterial();
		virtual ~hkgMaterial();

		hkInplaceArray<hkgTexture*,1> m_textures;
		HKG_TEXTURE_MODE m_textureMode[HKG_MAX_TEXTURE_STAGES];

		hkgShaderCollection* m_shaders; // transforms and can light if it wants to

		bool	m_isTransparent; // either the diffuse is transparent or the texture has alpha.
		bool	m_isDefaultMaterial;
		float	m_diffuse[4]; // includes alpha in last.

		float	m_specular[3];
		float	m_specularPower;

		hkUlong	m_userData; // +default(0)
};

#include <Graphics/Common/Material/hkgMaterial.inl>

#endif //  HK_GRAPHICS_MATERIAL_H

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
