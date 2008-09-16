/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/
#ifndef __RenderSystemCapabilities__
#define __RenderSystemCapabilities__ 1

// Precompiler options
#include "OgrePrerequisites.h"
#include "OgreString.h"

namespace Ogre {

    /// Enum describing the different hardware capabilities we want to check for
    enum Capabilities
    {
        //RSC_MULTITEXTURE          = 0x00000001,
        /// Supporta generating mipmaps in hardware
        RSC_AUTOMIPMAP              = 0x00000002,
        RSC_BLENDING                = 0x00000004,
        /// Supports anisotropic texture filtering
        RSC_ANISOTROPY              = 0x00000008,
        /// Supports fixed-function DOT3 texture blend
        RSC_DOT3                    = 0x00000010,
        /// Supports cube mapping
        RSC_CUBEMAPPING             = 0x00000020,
        /// Supports hardware stencil buffer
        RSC_HWSTENCIL               = 0x00000040,
        /// Supports hardware vertex and index buffers
        RSC_VBO                     = 0x00000080,
        /// Supports vertex programs (vertex shaders)
		RSC_VERTEX_PROGRAM          = 0x00000200,
        /// Supports fragment programs (pixel shaders)
		RSC_FRAGMENT_PROGRAM        = 0x00000400,
        /// Supports compressed textures
		RSC_TEXTURE_COMPRESSION     = 0x00000800,
        /// Supports compressed textures in the DXT/ST3C formats
		RSC_TEXTURE_COMPRESSION_DXT = 0x00001000,
        /// Supports compressed textures in the VTC format
		RSC_TEXTURE_COMPRESSION_VTC = 0x00002000,
        /// Supports performing a scissor test to exclude areas of the screen
        RSC_SCISSOR_TEST            = 0x00004000,
        /// Supports separate stencil updates for both front and back faces
        RSC_TWO_SIDED_STENCIL       = 0x00008000,
        /// Supports wrapping the stencil value at the range extremeties
        RSC_STENCIL_WRAP            = 0x00010000,
        /// Supports hardware occlusion queries
        RSC_HWOCCLUSION				= 0x00020000,
        /// Supports user clipping planes
        RSC_USER_CLIP_PLANES		= 0x00040000,
		/// Supports the VET_UBYTE4 vertex element type
		RSC_VERTEX_FORMAT_UBYTE4	= 0x00080000,
		/// Supports infinite far plane projection
		RSC_INFINITE_FAR_PLANE      = 0x00100000,
        /// Supports hardware render-to-texture (bigger than framebuffer)
        RSC_HWRENDER_TO_TEXTURE     = 0x00200000,
        /// Supports float textures and render targets
        RSC_TEXTURE_FLOAT           = 0x00400000,
        /// Supports non-power of two textures
        RSC_NON_POWER_OF_2_TEXTURES = 0x00800000,
		/// Supports 3d (volume) textures
		RSC_TEXTURE_3D				= 0x01000000,
		/// Supports basic point sprite rendering
		RSC_POINT_SPRITES		    = 0x02000000,
		/// Supports extra point parameters (minsize, maxsize, attenuation)
		RSC_POINT_EXTENDED_PARAMETERS = 0x04000000,
		/// Supports vertex texture fetch
		RSC_VERTEX_TEXTURE_FETCH = 0x08000000, 
		/// Supports mipmap LOD biasing
		RSC_MIPMAP_LOD_BIAS = 0x10000000 

    };

    /** singleton class for storing the capabilities of the graphics card. 
        @remarks
            This class stores the capabilities of the graphics card.  This
            information is set by the individual render systems.
    */
    class _OgreExport RenderSystemCapabilities 
    {
        private:
            /// The number of world matricies available
            ushort mNumWorldMatrices;
            /// The number of texture units available
            ushort mNumTextureUnits;
            /// The stencil buffer bit depth
            ushort mStencilBufferBitDepth;
            /// The number of matrices available for hardware blending
            ushort mNumVertexBlendMatrices;
            /// Stores the capabilities flags.
            int mCapabilities;
            /// The best vertex program that this card / rendersystem supports
            String mMaxVertexProgramVersion;
            /// The best fragment program that this card / rendersystem supports
            String mMaxFragmentProgramVersion;
            /// The number of floating-point constants vertex programs support
            ushort mVertexProgramConstantFloatCount;           
            /// The number of integer constants vertex programs support
            ushort mVertexProgramConstantIntCount;           
            /// The number of boolean constants vertex programs support
            ushort mVertexProgramConstantBoolCount;           
            /// The number of floating-point constants fragment programs support
            ushort mFragmentProgramConstantFloatCount;           
            /// The number of integer constants fragment programs support
            ushort mFragmentProgramConstantIntCount;           
            /// The number of boolean constants fragment programs support
            ushort mFragmentProgramConstantBoolCount;
			/// The number of simultaneous render targets supported
			ushort mNumMultiRenderTargets;
			/// The maximum point size
			Real mMaxPointSize;
			/// Are non-POW2 textures feature-limited?
			bool mNonPOW2TexturesLimited;
			/// The number of vertex texture units supported
			ushort mNumVertexTextureUnits;
			/// Are vertex texture units shared with fragment processor?
			bool mVertexTextureUnitsShared;

    	public:	
            RenderSystemCapabilities ();
            ~RenderSystemCapabilities ();

            void setNumWorldMatricies(ushort num)
            {
                mNumWorldMatrices = num;
            }

            void setNumTextureUnits(ushort num)
            {
                mNumTextureUnits = num;
            }

            void setStencilBufferBitDepth(ushort num)
            {
                mStencilBufferBitDepth = num;
            }

            void setNumVertexBlendMatrices(ushort num)
            {
                mNumVertexBlendMatrices = num;
            }

			/// The number of simultaneous render targets supported
			void setNumMultiRenderTargets(ushort num)
			{
				mNumMultiRenderTargets = num;
			}

            ushort getNumWorldMatricies(void) const
            { 
                return mNumWorldMatrices;
            }

            /** Returns the number of texture units the current output hardware
                supports.

                For use in rendering, this determines how many texture units the
                are available for multitexturing (i.e. rendering multiple 
                textures in a single pass). Where a Material has multiple 
                texture layers, it will try to use multitexturing where 
                available, and where it is not available, will perform multipass
                rendering to achieve the same effect. This property only applies
				to the fixed-function pipeline, the number available to the 
				programmable pipeline depends on the shader model in use.
            */
            ushort getNumTextureUnits(void) const
            {
                return mNumTextureUnits;
            }

            /** Determines the bit depth of the hardware accelerated stencil 
                buffer, if supported.
                @remarks
                    If hardware stencilling is not supported, the software will
                    provide an 8-bit software stencil.
            */
            ushort getStencilBufferBitDepth(void) const
            {
                return mStencilBufferBitDepth;
            }

            /** Returns the number of matrices available to hardware vertex 
                blending for this rendering system. */
            ushort numVertexBlendMatrices(void) const
            {
                return mNumVertexBlendMatrices;
            }

			/// The number of simultaneous render targets supported
			ushort numMultiRenderTargets(void) const
			{
				return mNumMultiRenderTargets;
			}

            /** Adds a capability flag to mCapabilities
            */
            void setCapability(const Capabilities c) 
            { 
                mCapabilities |= c;
            }

            /** Checks for a capability
            */
            bool hasCapability(const Capabilities c) const
            {
                if(mCapabilities & c)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            /// Gets the best low-level vertex program version supported
            const String& getMaxVertexProgramVersion(void) const
            {
                return mMaxVertexProgramVersion;
            }
            /// Gets the best fragment program that this card / rendersystem supports
            const String& getMaxFragmentProgramVersion(void) const
            {
                return mMaxFragmentProgramVersion;
            }
            /// The number of floating-point constants vertex programs support
            ushort getVertexProgramConstantFloatCount(void) const
            {
                return mVertexProgramConstantFloatCount;           
            }
            /// The number of integer constants vertex programs support
            ushort getVertexProgramConstantIntCount(void) const
            {
                return mVertexProgramConstantIntCount;           
            }
            /// The number of boolean constants vertex programs support
            ushort getVertexProgramConstantBoolCount(void) const
            {
                return mVertexProgramConstantBoolCount;           
            }
            /// The number of floating-point constants fragment programs support
            ushort getFragmentProgramConstantFloatCount(void) const
            {
                return mFragmentProgramConstantFloatCount;           
            }
            /// The number of integer constants fragment programs support
            ushort getFragmentProgramConstantIntCount(void) const
            {
                return mFragmentProgramConstantIntCount;           
            }
            /// The number of boolean constants fragment programs support
            ushort getFragmentProgramConstantBoolCount(void) const
            {
                return mFragmentProgramConstantBoolCount;           
            }



            /// sets the best low-level vertex program version supported
            void setMaxVertexProgramVersion(const String& ver)
            {
                mMaxVertexProgramVersion = ver;
            }
            /// sets the best fragment program that this card / rendersystem supports
            void setMaxFragmentProgramVersion(const String& ver)
            {
                mMaxFragmentProgramVersion = ver;
            }
            /// The number of floating-point constants vertex programs support
            void setVertexProgramConstantFloatCount(ushort c)
            {
                mVertexProgramConstantFloatCount = c;           
            }
            /// The number of integer constants vertex programs support
            void setVertexProgramConstantIntCount(ushort c)
            {
                mVertexProgramConstantIntCount = c;           
            }
            /// The number of boolean constants vertex programs support
            void setVertexProgramConstantBoolCount(ushort c)
            {
                mVertexProgramConstantBoolCount = c;           
            }
            /// The number of floating-point constants fragment programs support
            void setFragmentProgramConstantFloatCount(ushort c)
            {
                mFragmentProgramConstantFloatCount = c;           
            }
            /// The number of integer constants fragment programs support
            void setFragmentProgramConstantIntCount(ushort c)
            {
                mFragmentProgramConstantIntCount = c;           
            }
            /// The number of boolean constants fragment programs support
            void setFragmentProgramConstantBoolCount(ushort c)
            {
                mFragmentProgramConstantBoolCount = c;           
            }
			/// Maximum point screen size in pixels
			void setMaxPointSize(Real s)
			{
				mMaxPointSize = s;
			}
			/// Maximum point screen size in pixels
			Real getMaxPointSize(void) const
			{
				return mMaxPointSize;
			}
			/// Non-POW2 textures limited
			void setNonPOW2TexturesLimited(bool l)
			{
				mNonPOW2TexturesLimited = l;
			}
			/** Are non-power of two textures limited in features?
			@remarks
				If the RSC_NON_POWER_OF_2_TEXTURES capability is set, but this
				method returns true, you can use non power of 2 textures only if:
				<ul><li>You load them explicitly with no mip maps</li>
				<li>You don't use DXT texture compression</li>
				<li>You use clamp texture addressing</li></ul>
			*/
			bool getNonPOW2TexturesLimited(void) const
			{
				return mNonPOW2TexturesLimited;
			}

			/// Set the number of vertex texture units supported
			void setNumVertexTextureUnits(ushort n)
			{
				mNumVertexTextureUnits = n;
			}
			/// Get the number of vertex texture units supported
			ushort getNumVertexTextureUnits(void) const
			{
				return mNumVertexTextureUnits;
			}
			/// Set whether the vertex texture units are shared with the fragment processor
			void setVertexTextureUnitsShared(bool shared)
			{
				mVertexTextureUnitsShared = shared;
			}
			/// Get whether the vertex texture units are shared with the fragment processor
			bool getVertexTextureUnitsShared(void) const
			{
				return mVertexTextureUnitsShared;
			}


            /** Write the capabilities to the pass in Log */
            void log(Log* pLog);




    };
}

#endif // __RenderSystemCapabilities__

