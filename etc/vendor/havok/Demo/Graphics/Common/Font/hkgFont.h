/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_FONT_H
#define HK_GRAPHICS_FONT_H

#include <Common/Base/Types/hkBaseTypes.h>
#include <Graphics/Common/Texture/hkgTexture.h>
#include <Graphics/Common/Font/hkgFontDefines.h>

/// A simple 2D texture based font that assumes an orthographic projection (but can be used with any projection).
/// Can handle any texture of a given format, as outlined in the setTexture() method description.
class hkgFont  : public hkgReferencedObject
{
	public:
			/// Create a platform specific font object.
		static hkgFont* (HK_CALL *create)();

			/// Set the texture to be used by the font.
			/// The texture can be changed anytime, for different render() calls for instance.
			/// It will reference count the texture if not null. 
			/// The texture should have 8 rows of 16 chars per row as follows, evenly spaced:
			/// <code>
			///   user
			///   user
			///    !"#$%$'()*+,-./
			///	  0123456789:;<=>?
			///	  @ABCDEFGHIJKLMNO
			///	  PQRSTUVWXYZ[\]^_
			///	  'abcdefghijklmno
			///   pqrstuvwxyz{|}~
			/// </code>
			/// This is a common enough texture format, produced by such programs as Bitmap Font Builder by Thom Wetzel 
			/// which came about from the NeHe OpenGL example font code that assumed that format (less the .ini of course)

		void setTexture(hkgTexture* t, hkgFontType tp = HKG_FONT_TYPE_SINGLE);

			/// Get the pointer to the current texture for the font. It may be HK_NULL.
		inline const hkgTexture* getTexture() const;
		inline hkgTexture* getTexture();

			/// Setup the draw state for the font. This will adjust the render state of the
			/// context to match that expected of the font, such as alpha and depth modes.
			/// Note that this is a static call and can be called anytime, but render() calls
			/// for text should follow this call or else the render state may be altered.
		static void		HK_CALL setDrawState( hkgDisplayContext* context );

			/// Render the given text to the current context's viewport.
			/// It will assume that the camera projection is in an orthographic 
			/// projection already (but of course can be any projection) and that 
			/// the world matrix is the identity. It will always enable textures even 
			/// if the current viewport in the context  requests otherwise. Assumes 
			/// that the draw state is correct (will not check for speed reasons so you can fool it if you wish)
			/// The color is RGBA as 4 floats of 0-->1 range.
		virtual void	render( hkgDisplayContext* context, const char* text, float x, float y, float color[4]) = 0;

			/// Resolve the upper right and lower left texture coordinates for a given 
			/// character. You can then use these coordinates to texture any polygon if
			/// you wish to use the texture outside of this class.
		void		resolve( char c, float& u0, float& v0, float& u1, float& v1);
		void		setFontSet(int set); 

			/// Set the character width, in pixels on the screen. The default is correct for characters
			/// of 8 pixels high and with the default texture size. Works out as about 10 pixels wide
			/// to avoid distortion.
		inline void		setCharWidth(float w);

			/// Set the character height, in pixels on the screen. The default is 8 pixels high 
		inline void		setCharHeight(float h);

			/// Get the width of the characters on the screen, in pixels.
		inline float		getCharWidth() const;
			
			/// Get the height of the characters on the screen, in pixels.
		inline float		getCharHeight() const;
	
	protected:

		hkgFont();
		virtual ~hkgFont();

		hkgTexture*		m_texture;
		hkgFontType		m_fontType;
		int				m_fontSet;

		float			m_charWidth;
		float			m_charHeight; 
};

#include <Graphics/Common/Font/hkgFont.inl>

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
