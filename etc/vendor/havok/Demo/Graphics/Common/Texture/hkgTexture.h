/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_TEXTURE_H
#define HK_GRAPHICS_TEXTURE_H

#include <Graphics/Common/DisplayContext/hkgDisplayContext.h>
#include <Graphics/Common/Texture/hkgTextureDefines.h>

class hkIstream;

extern const hkClass hkgTextureClass;

/// The graphics library texture class. The class knows how to load itself from different
/// file formats across multiple platforms, can handle alpha channels and palettes, but most
/// importantly it can swizzle and load itself into hardware using the realize(), bind() and free()
/// calls.
class hkgTexture : public hkgReferencedObject
{
		friend class hkgDisplayContext;

	public:

			/// Create a blank, platform specific, texture object in the current context as given.
		static hkgTexture* (HK_CALL* create)(hkgDisplayContext* context);

			/// Load a texture from file. Will parse the filename to find the file type,
			/// and use an Ifstream to load it from file before passing the stream on to
			/// the specific loadFrom*() functions.
			/// It can load PNG, uncompessed TGA, or BMP(only under Win32, and will try the
			/// HINSTANCE resources too if it is of BMP type). The hinstance is only required
			/// in Win32 and then only if you want the resource check.
		virtual bool loadFromFile(const char* filename, void * hinstance = HK_NULL);

			/// Load a DDS using D3DX or DevIL or whatever. Not supported on most platforms.
		virtual bool loadFromDDS(hkIstream& s) { return false; }

			/// Load a PNG using libpng, which is built into this library, It will support
			/// most common PNG types, but compressed 32 bit with no palette is recommended.
		bool loadFromPNG(hkIstream& s);

			/// Load an uncompressed 24bit or 32bit Targa file.
		bool loadFromTGA(hkIstream& s);

			/// Load a Windows BMP. As this method will use Windows functions, it is
			/// only implemented on Windows at the moment. In future a library such as FreeImage
			/// may be used for true cross platform support, but PNG is supported on all
			/// platforms as is and is the recommend format.
		bool loadFromBMP(hkIstream& s);

			/// Load from a Windows Device Independent Bitmap (DIB). Only will work in Windows
			/// and will assume the void* is a BITMAPINFO*
		bool loadFromDIB(void* win32DIB);

			/// Load a built in texture. Built-in means that the library has all the texture
			/// information already for the image. These are all either programmatic (such as the
			/// checker pattern) or stored in a binary resource, as a compressed PNG (the logo, pointer
			/// and default font).
			/// The 4 current built-ins are:
			///   HKG_TEXTURE_HAVOKLOGO
			///   HKG_TEXTURE_FONT
			///   HKG_TEXTURE_POINTER
			///   HKG_TEXTURE_CHECKER
		bool loadBuiltIn( HKG_TEXTURE_DEFAULTS d );

			/// Load a texture from memory address. This is currently the only way to specify 3D textures.
			/// You must provide the x,y,z dimensions and the number of components per texel.
			/// If z=0, a 2D texture will be generated. y=z=0 is not supported (aka 1D textures).
		bool loadFromPointer(hkUint32 width, hkUint32 height, hkUint32 depth, hkUint16 nComp, hkUint8* data);

			/// Save to file (will parse extension for type)
		bool saveToFile(const char* filename);
		bool saveToRAW(hkOstream& s); // raw rgba values (photoshop etc can read them if you know the width, height, rgb/rgba). Use .raw,
		bool saveToPNG(hkOstream& s);
		bool saveToTGA(hkOstream& s);

			/// Has the texture got any pixel data yet? It is a simple check for m_data != HK_NULL.
		inline bool hasData() const;

			/// Will wipe and deallocate all held data. This is not the same as free() which
			/// frees platform specific information. This clear() will remove all the data and
			/// clut data.
		virtual void clear();

			/// Get the const pointer to the internal data array. May be HK_NULL if it has yet to be allocated.
			/// You can work out the dimensions by the width, height and bpp.
		inline unsigned char* getDataPointer(int mipLevel = 0) const;

			/// Get the const pointer to the Color Lookup Table. This is not fully implemented on all platforms yet so it
			/// is safest to stick with a HK_NULL clut for the time being.
		inline const unsigned char* getClutPointer() const;

			/// Get the pointer to the internal data array. May be HK_NULL if it has yet to be allocated.
			/// You can work out the dimensions by the width, height and bpp. Once a texture has been realized(),
			/// the structure may change (swizzled for instance on GameCube) to reduce memory overheads.
		inline unsigned char* getDataPointer();

			/// Get the pointer to the Color Lookup Table. This is not fully implemented on all platforms yet so it
			/// is safest to stick with a HK_NULL clut for the time being.
		inline unsigned char* getClutPointer();

			/// Support for direct access to the platform texture (as m_data
			/// is just the generic (unswizzled format etc)
			/// If the format is swizzled you will need to know the platform
			/// swizzle format, this is not generically supported at this time.
		virtual HKG_TEXTURE_PIXEL_FORMAT getPixelFormat() const;

			// usually m_width*bitsPerPixel, but can be different
		virtual int getRowByteLength(int mipLevel = 0);

			// use getPixelFormat to figure out data type returned.
			// unlock when you have finished altering the data.
			// locking with 'all write' is faster if you do so
			// as drivers can allocate a new resource internally
			// Best to have no current texture when locking a texture
			// so use hkgDisplayContext->setSoleCurrentTexture(HK_NULL) first.
			// If texture was realized not as dynamic, may not be lockable (will return NULL)
		virtual unsigned char* lock(int mipLevel = 0, bool willWriteAllPixels = false);
		virtual void unlock(int mipLevel = 0);

			/// Allocate the data (texture surface) with the given width and height. with optional alpha channel and a clut.
			/// You can optional supply a data pointer to copy the image from. This source must be at least
			/// the same size as the destination. A lot of platforms restrict width and height to power of 2 in size,
			/// with max size of 2048x2048. This is naturally too big for the PlayStation(R)2 for instance that has
			/// limited resources for textures.
		unsigned char* allocateSurface( unsigned int width, unsigned int height, bool alpha, bool usesClut, const unsigned char* sourceData = HK_NULL );

			/// Allocate the Color Lookup Table (CLUT) with numEntries and optional alpha channel. You can
			/// supply a source CLUT to copy the data from, but it must be the same dimension (or higher) than
			/// this one. A lot of platforms require a 256 entry CLUT.
		unsigned char* allocateClut( unsigned int numEntries, bool alpha, const unsigned char* sourceData = HK_NULL );

			/// Get the width, in pixels, of the texture. Usually a power of 2.
		inline unsigned int getWidth() const;

			/// Get the height, in pixels, of the texture. Usually a power of 2.
		inline unsigned int getHeight() const;

			/// Get the bits per pixel of the image (8,24,32).
		inline unsigned int getBPP() const;

			/// Is the texture transparent? This would imply that it has an alpha channel and values
			/// in that channel are less than max alpha (< 255)
		inline bool isTransparent() const;

			/// Does the texture have an alpha channel? This does not imply that the alpha channel
			/// has an particular values though.
		inline bool hasAlphaChannel() const;

			/// Do you want it to autogenerate mip map levels
		inline void setAutoMipMaps(bool on);

		/// Do you want it to compress the texture to DXT (only supported on some platforms)
		inline void setAutoCompression(bool on);


		inline bool isLockable() const;

			/// Texture wrap mode
		inline HKG_TEXTURE_UVMODE getTextureWrapModeU() const;
		inline HKG_TEXTURE_UVMODE getTextureWrapModeV() const;
		inline HKG_TEXTURE_UVMODE getTextureWrapModeW() const;
		inline void setTextureWrapModeU(HKG_TEXTURE_UVMODE m);
		inline void setTextureWrapModeV(HKG_TEXTURE_UVMODE m);
		inline void setTextureWrapModeW(HKG_TEXTURE_UVMODE m);

			/// Add an alpha channel to the image. This will have to reallocate the data if there is not alpha
			/// in the image already (if there is, the function just returns). You can init the alpha
			/// to the given value if desired.
		void addAlphaChannel(unsigned char value = 0xff);

		void colorReplace(unsigned int from, unsigned int to );

			/// Add a computed alpha channel to the image based on source luminance.
			/// This will have to reallocate the data if there is not alpha
			/// in the image already (if there is, the function just returns).
			/// Handy for simple bitmap based alpha overlays.
			/// Alpha = sum(r,g,b), so black == fully transparent.
			/// invert for white == fully transparent
		void addComputedAlphaChannel(bool invert = false);

			/// Handy for some normal maps (invert green sometimes)
		void invertChannel(bool red, bool green, bool blue, bool alpha);

			/// Flip Y (height). TGA assumed to be opposite storage to HKG (which is OGL style, 0,0 in lower left)
		void flipY();

			/// change the RGB values (leave alpha) to this value)
			/// handy for computed alpha channels and then setting the rgb to pure white after the alpha is set
		void setRGB(unsigned char r, unsigned char g, unsigned char b);

			/// Get the void* user data that can be used to tag the texture with game or conversion information.
		inline hkUlong getUserData() const;

			/// Set the void* user data that can be used to tag the texture with game or conversion information.
		inline void setUserData(hkUlong d);

			/// Platform specific: Realize the texture to the hardware, On PlayStation(R)2 this will just use
			/// the texture in m_data directly, but on GameCube and DirectX platforms (PC & Xbox) the
			/// textures are swizzled to a specific form and so a copy is created. On GameCube (only) this
			/// will result in the actual m_data being swizzled to reduce memory overhead of keeping
			/// the unswizzled version as well. On PC and Xbox, DirectX manages this extra overhead, and on
			/// PlayStation(R)2 there is no overhead.
			/// You can only lock() textures that you create as dynamic
		virtual bool realize(bool dynamic = false);

			/// Free all platform specific resources. If you free a texture, m_data will still be valid
			/// on all platforms except GameCube where the m_data pointer will become HK_NULL as it is
			/// swizzled anyway, so can't be re-realized with special treatment, This will change in future
			/// to be more like the other platforms. Note that you still need to release() to free this object.
		virtual void free();

		inline HKG_TEXTURE_TYPE getMapType() const;

		inline void setCoordGen( HKG_TEXTURE_COORD_GEN g );
		inline HKG_TEXTURE_COORD_GEN getCoordGen( ) const;

		inline void setFilterMode( HKG_TEXTURE_FILTER f );
		inline HKG_TEXTURE_FILTER getFilterMode( ) const;

		inline int getNumMipLevels( ) const;

		int getTotalSurfaceByteSize() const;
		int getSurfaceByteSize(int mipLevel) const;

        // implement hkReferencedObject
        virtual const hkClass* getClassType() const { return &hkgTextureClass; }
        virtual void calcContentStatistics(hkStatisticsCollector* collector, const hkClass* cls) const { }

	protected:

		inline hkgTexture(hkgDisplayContext* owner);
		virtual ~hkgTexture();

		hkgDisplayContext* m_owner;

			/// Takes the m_data, allocates enough space for it and its
			/// sub levels, and then does a box filter down
		void buildMipmaps();

			/// Bind the texture to a given stage. This is called bu the context only as it
			/// knows what stages are active etc and what is currently bound to them.
			/// Enable that texture stage as well.
		virtual void bind(int stage, HKG_TEXTURE_MODE mode) const;
			// remove the texture from the give stage / disable that stage. Normally sets that stage to NULL.
		virtual void unbind(int stage) const;

		// call loadFromFile instead
		bool loadFromBMP(const char* filename, void * hinstance);

		struct DDSLoadInfo
		{
			bool compressed;
			bool swap;
			bool palette;
			unsigned int divSize;
			unsigned int blockBytes;
			HKG_DDS_FORMAT_TYPE internalFormat;
			HKG_DDS_FORMAT_TYPE externalFormat;
			HKG_DDS_FORMAT_TYPE type;
		};

		bool loadDDSFromMemory( unsigned char* f, DDSLoadInfo& info );

		bool m_hasAlpha;		// has an alpha channel
		bool m_isTransparent;  // some of the alpha values are non max values.
		bool m_bIsLockble; // some platforms, unless the texture is dynamic, can't be lock()ed
		bool m_wantMipMaps;  // do you want the texture to auto gen mip map levels (if supported)
		bool m_wantCompression;  // do you want the texture to be compressed to DXT format (if supported)

		int  m_mipLevels; // <= 1  == none

		unsigned char* m_clut; // 256 entry clut. Will have be 3 or 4 bytes depending on m_hasAlpha;
		unsigned char* m_data; // 8 bit clut, 24 bit  rgb or 32 bit rgba, may have several mips (contiguous)

		unsigned int m_width;  // in pixels
		unsigned int m_height; // in pixels
		unsigned int m_depth;  // in pixels
		unsigned short m_bpp;  // bits per pixel;

		HKG_TEXTURE_TYPE m_mapType;
		HKG_TEXTURE_UVMODE m_uWrapMode;
		HKG_TEXTURE_UVMODE m_vWrapMode;
		HKG_TEXTURE_UVMODE m_wWrapMode;
		HKG_TEXTURE_COORD_GEN m_coordGen;
		HKG_TEXTURE_FILTER m_filterMode;

		hkUlong m_userData; // +default(0)

};

#include <Graphics/Common/Texture/hkgTexture.inl>

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
