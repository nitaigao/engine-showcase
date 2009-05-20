/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_DISPLAY_CONTEXT_H
#define HK_GRAPHICS_DISPLAY_CONTEXT_H

#include <Common/Base/Types/hkBaseTypes.h>
#include <Graphics/Common/hkgObject.h>
#include <Graphics/Common/DisplayContext/hkgDisplayContextDefines.h>

// We include the window header so that we can use it for 
// m_owner inlined (must be after the above defs)

class hkgViewport;
class hkgTexture;
class hkgVertexSet;
class hkgShader;
class hkgShaderContext;
class hkgShaderCollection;
class hkgMaterial;
class hkgWindow;
class hkgBlendMatrixSet;

#define HKG_CHECK_CONTEXT_LOCK_STATUS(ctx) HK_ASSERT2(0x4354260, ctx->isLocked(), "You are making graphics calls without locking the display context") 

class hkGpuInterface; // NV_P2P  XXXXX HACK!

/// The display context. One of the core classes in the graphics library. It represents the rendering state of
/// graphics hardware / underlying API, so that the library can work with any API even if the API stores no state 
/// itself, like the pure DirectX H/W abstraction layer on the Xbox. The display context not only stores the state 
/// but is a virtual abstraction so that direct state changes can be made on all platforms and so we can do 
/// immediate mode rendering through this class as well.
/// It exposes a limited number of render state options, representing the lowest common denominator required to
/// produce sufficient graphical apps.
/// You will see that the user never creates a context (there is no public constructor), as there is only one per 
/// piece of hardware / API context and it is created as needed.

class hkgDisplayContext : public hkgReferencedObject
{
		friend class hkgWindow;

	public:

		static int s_maxNumLights;

			/// This retrieves the maximum number of lights allowed at once on the
			/// current platform.
		static int getMaxNumLights() { return s_maxNumLights; }

			/// Get the current vertex stream options. Most platforms have a concept of a current
			/// source for each vertex component or set of components, and sometimes it is desirable
			/// to disable all of them, or to selectively disable parts.
		inline HKG_VERTEX_OPTIONS getVertexOptions() const;

			/// Set the current vertex stream options. Most platforms have a concept of a current
			/// source for each vertex component or set of components, and sometimes it is desirable
			/// to disable all of them, or to selectively disable parts.
		inline void setVertexOptions( HKG_VERTEX_OPTIONS o );

			/// Set the current viewport. The viewport is hook though which the context can figure
			/// out what portion of the framebuffer it is using and what state is desired for that view
			/// This method just sets the internal current viewport to point to v. The viewport is not 
			/// referenced by the context (does not inc ref count).
		inline void				setCurrentViewport(hkgViewport*	v);

			/// Get the current viewport. The viewport is hook though which the context can figure
			/// out what portion of the framebuffer it is using and what state is desired for that view
			/// The viewport is not referenced by the context (does not inc ref count).
		inline hkgViewport*		getCurrentViewport() const;
		
			/// Get the current active 2D texture. The texture is referenced by the context (incs ref count).
			/// This texture will be used when texturing is enabled.
		inline hkgTexture*		getCurrentTexture(int stage) const;

		inline hkgMaterial*		getCurrentMaterial() const;
		inline HKG_MATERIAL_VERTEX_HINT getCurrentMaterialHints() const;

			/// Get the context from which shaders will draw their context information (input 'constants')
		inline hkgShaderContext* getShaderContext() const;

			/// Get the current active vertex shader.
		inline hkgShader*		getCurrentVertexShader() const;

			/// Get the current active pixel shader.
		inline hkgShader*		getCurrentPixelShader() const;

		virtual hkgShaderCollection* getCurrentGlobalShaderCollection( ) const;

			/// Get the current active set of blend matrices.
		inline hkgBlendMatrixSet* getCurrentBlendMatrices() const;

			/// Get the current active set of vertices.
		inline hkgVertexSet* getCurrentVertexSet() const;

			/// Set the current active 2D texture. This context currently only supports a single active 
			/// texture at any one time. The texture is referenced by the context (incs ref count).
			/// This texture will be used when texturing is enabled.
		virtual void		    setCurrentTexture(int stage, hkgTexture* t, HKG_TEXTURE_MODE mode); // will bind to it aswell

		virtual void 			setCurrentSoleTexture(hkgTexture* t, HKG_TEXTURE_MODE mode); // will bind to it aswell, and unbind all other stages (if any)


			/// Set the current material. Can be null (will unbind current if so)
			/// Will no bind if is already the current material
		virtual void		    setCurrentMaterial(class hkgMaterial* m, HKG_MATERIAL_VERTEX_HINT hints = HKG_MATERIAL_VERTEX_HINT_NONE ); // will bind to it aswell

			/// Set the current active shader. 
		virtual void		    setCurrentVertexShader(hkgShader* s); // will bind to it aswell

		/// Set the current active shader. 
		virtual void		    setCurrentPixelShader(hkgShader* s); // will bind to it aswell

		void					setCurrentGlobalShaderCollection( hkgShaderCollection* globalShaders );

			/// Set the current active blend matrix set
		virtual void		    setCurrentBlendMatrices(hkgBlendMatrixSet* bm); // will bind to it aswell

			/// Set the current active blend matrix set
		virtual void		    setCurrentVertexSet(hkgVertexSet* vs); // will bind (setStreams) to it aswell

			/// A context has a window that owns it. Each window has a context at the moment, so there
			/// is a one to one mapping from window to context.
		inline const hkgWindow*	getOwner() const;

			/// A context has a window that owns it. Each window has a context at the moment, so there
			/// is a one to one mapping from window to context.
		inline hkgWindow*		getOwner();

			/// Get the current render state.The returned value is a bitfield of enabled options.
		inline HKG_ENABLED_STATE  getEnabledState() const;	
		
			/// Enable/Disable per vertex lighting..
		virtual void setLightingState(bool on) = 0;

			/// Enable/Disable 2D texture mapping, using the current texture as the map. 1D & 3D textures 
			/// not supported yet.
		virtual void setTexture2DState(bool on) = 0;

			/// Enable/Disable alpha blending. If blending is enabled, a (src alpha, 1-src alpha) style 
			/// blend is enabled, which is a common transparent surface rendering mode. Modes such as 
			/// additive blending are not yet supported through this interface.
		virtual void setBlendState(bool on) = 0;

			/// Enable/Disable whether we read the Z buffer when doing the pixel updates. If z read is 
			/// turned off, we bypass the z buffer test. If z read is on, the z test is update on 
			/// 'less than'. Typically for say transparent objects, we render solids first with z read 
			/// and write on, then render the transparents with z read on, but z write off.
		virtual void setDepthReadState(bool on) = 0;

			/// Enable/Disable whether we write to the Z buffer when doing the pixel updates. If z write
			/// is turned off, even if the pixel passed the z test during the z read, we bypass the z buffer
			/// update of the z value and update the color only. Typically for say transparent objects, we 
			/// render solids first with z read and write on, then render the transparents with z read on, 
			/// but z write off.
		virtual void setDepthWriteState(bool on) = 0;

			/// Set the depth bias for the Z buffer when doing the pixel updates. This will offset the
			/// depth by the biased amount while rendering, allowing you to render the same geometry twice
			/// but over the preceding one without turning depth read/write off. This is useful for shadows
			/// or when drawing a wireframe over the actual mesh.
		virtual void setDepthBias(float val) = 0;

			/// Turn on or off backface culling. Although it may seem obvious the backface culling 
			/// would improve speed, it is really system dependant as it may cause blocks in the 
			/// triangle pipeline, especially on PlayStation(R)2 where the culling is done in the VU and has to break up
			/// the triangle strips for instance if culling is enabled. 	
		virtual void setCullFaceState(bool on ) = 0;

			/// Turn on or off rendering in a wireframe mode. On GameCube this is done a linestrips so 
			/// is slower compared to other platforms that have native support for this mode.
		virtual void setWireframeState(bool on) = 0;
		
			/// Enable or disable the given light number. It assumes that if you are enabling a given light
			/// that you have set them up using the hkgLight class. This is merely a state change.
		virtual void setLightState(int light, bool on) = 0;

			/// Alter if needed the current state to match the given desired state. This is used to 
			/// make sure that the context matches your desired rendering view, without having to query 
			/// each state in turn.
		void matchState( HKG_ENABLED_STATE desiredState, 
						HKG_CULLFACE_MODE desiredCullfaceMode = HKG_CULLFACE_CW,
						HKG_BLEND_MODE desiredBlendMode = HKG_BLEND_MODULATE );
		
			/// This method makes the underlying hardware state match that assumed by this context. This
			/// Is required at startup to get the hardware in a set state, and also after the state of
			/// the hardware becomes undetermined (for instance after D3D resets once it loses context).
			/// The method simply calls 'set state' on each controlled state with the context idea of which
			/// is enabled.
		void syncState(); 
		
			/// Two modes, CW and CCW. Specify false as second arg if you want CCW (in a right hand system)
			/// culling.
		inline HKG_CULLFACE_MODE getCullfaceMode() const;

			/// Two modes, CW and CCW. Specify false as second arg if you want CCW (in a right hand system)
			/// culling. Will set the mode and call setCullFaceState() to reflect the change if culling is on.
		inline void setCullfaceMode( HKG_CULLFACE_MODE m );

			/// Additive or Modulated blend mode.
		inline HKG_BLEND_MODE getBlendMode() const;

			/// Additive or Modulated blend mode.
		inline void setBlendMode( HKG_BLEND_MODE m );

			/// How color is set
		inline HKG_COLOR_MODE getColorMode() const;

			/// How color is set
		inline void setColorMode( HKG_COLOR_MODE c );

			/// What type of debugging we would like to do
		inline HKG_DEBUG_FLAGS getDebugFlags() const;

		inline void setDebugFlags( HKG_DEBUG_FLAGS c );

			/// What type of rendering are we doing
		inline HKG_RENDER_PASS_INFO getRenderPassInfo() const;

		inline void setRenderPassInfo( HKG_RENDER_PASS_INFO p );

			/// What, if any, stage is locked externally (with a shadow map for instance)
		inline HKG_TEXTURE_STAGE_LOCK_MODE getTextureStageLockMode() const;

		inline void setTextureStageLockMode( HKG_TEXTURE_STAGE_LOCK_MODE l );


		// Immediate mode rendering

			/// Pop matrix will take the current modelview matrix off the stack and restore
			/// the previous matrix as the current matrix.
		virtual void popMatrix() = 0;

			/// Push matrix will place the current modelview matrix on the stack. It does not need
			/// to alter the current matrix.
		virtual void pushMatrix() = 0;

			/// Multiply the current modelview matrix by m. Matrices in this graphics lib are OpenGL style,
			/// so are column major, so item(row, col) is m[row + col*4].
		virtual void multMatrix( const float* m) = 0;

			/// Begin a set of immediate mode rendering primitives. Do not alter state while inside a group.
			/// This is quite similar to glBegin( ), where HKG_IMM_GROUP defines how the items will be interpreted.
			/// Call endGroup() once all vertices have been defined for the group.
		virtual void beginGroup(HKG_IMM_GROUP type) = 0;

			/// Set the current color, RGB as floats 0.0-->1.0. Alpha will be set as 1.0
		virtual void  setCurrentColor3( const float* c) = 0;

			/// Set the current color, RGBA as floats 0.0-->1.0
		virtual void  setCurrentColor4( const float* c) = 0;
		
			/// Set the current color in packed form: [ARGB] == 0xAARRGGBB
		virtual void  setCurrentColorPacked( unsigned int c) = 0; 

			/// Set the current texture coordinate uv[0] = u, uv[1] = v.
			/// These can be <0 or >1, and the clamp mode is 'repeat' at the moment.
			/// Be careful on some platforms very high u or v values can cause artifacts
			/// such as on PlayStation(R)2 with u or v in the 100s.
		virtual void  setCurrentTextureCoord(const float* uv) = 0;

			/// Set the current normal to be used in per vertex lighting. If lighting is disabled
			/// then there is not much point in specifying a normal, but a default normal will be
			/// assumed.
		virtual void  setCurrentNormal(const float* n) = 0;

			/// Set the vertex position. This call will define the vertex, so all 'current' attributes 
			/// for the vertex will be used as of this point, so all color,normal,etc. must be set before 
			/// this call to affect this vertex.
		virtual void  setCurrentPosition(const float* p) = 0;

			/// End the group. This will batch all the vertices together that where specified after the last
			/// beginGroup(). There is no guarantee that the vertices will appear in the buffer straight away
			/// until flush is called. Make sure you specify enough vertices to satisfy the type of primitive
			/// you are rendering.
		virtual void endGroup(bool useCurrentShaders = false) = 0;

			/// Flush the pipeline of commands to the hardware. After this call it can be assumed that all
			/// outstanding rendering has been completed.
		virtual void flush() = 0;

			// Handy debug routines. Optionally set the state to match (and restore) a default state (no lighting etc).
		void drawUnitAxis(bool setState = true);

		void feedShaders(bool onlyFeedDirty);

			// Owner will not be reference() counted against this object
		static hkgDisplayContext* (*create)(hkgWindow* owner);

		virtual void lock() const;
		bool isLocked() const;
		virtual void unlock() const;

		// For dynamic textures and vertex sets, normally vid reset will cause invalidation in dx renderers etc
		inline void registerTexture(hkgTexture* t) const;
		inline bool isRegisteredTexture(hkgTexture* t) const;
		inline void unregisterTexture(hkgTexture* t) const;
		
		inline void registerVertexSet(hkgVertexSet* vs) const;
		inline bool isRegisteredVertexSet(hkgVertexSet* vs) const;
		inline void unregisterVertexSet(hkgVertexSet* vs) const;


		int getNumCurrentTextures() const;

		virtual void setMaxAnisotropy(int i); // usually 1 to 16 (on platforms that support it and for textures that you enable it on)
		virtual int getMaxAnisotropy() const;

			// For a given scene, bias is the amount (default 0.0001) by which to offset the test by (in the depths when using VSM
			// are from 0..1 where 1 is the furthest, so 0.0001 is 0.01% of the depth of the shadow region, so for big scenes you can make this smaller.
			// The epsilon is controls the variance, so that it does not go to 0, so is just a small number, but can be used to alter the
			// influence of the variance (has to be positive though).
		virtual void setShadowVsmBias( float bias, float epsilon );
		virtual void getShadowVsmBias( float& bias, float& epsilon );

//#ifdef NV_P2P
        void setGpuInterface(hkGpuInterface *pGpu) { m_gpuInterface = pGpu; }
        hkGpuInterface* getGpuInterface() { return m_gpuInterface; }
//#endif

	protected:

		hkgDisplayContext(hkgWindow* owner); 
		virtual ~hkgDisplayContext();
		
		HKG_ENABLED_STATE   m_currentState;
		HKG_VERTEX_OPTIONS  m_vertexOptions;	
		HKG_CULLFACE_MODE   m_cullfaceMode;
		HKG_BLEND_MODE	    m_blendMode;
		HKG_COLOR_MODE		m_colorMode;
		HKG_DEBUG_FLAGS	    m_debugFlags;
		HKG_RENDER_PASS_INFO m_renderPass;
		HKG_TEXTURE_STAGE_LOCK_MODE m_textureStageLock;
		hkgWindow*		    m_owner;
		hkgViewport*	    m_currentViewport;
		hkgTexture*		    m_currentBoundTextures[HKG_MAX_TEXTURE_STAGES]; // max 8 stages.
		
		//if all object share a material in a group there is no need to rebind to it
		hkgMaterial*		m_currentBoundMaterial;
		HKG_MATERIAL_VERTEX_HINT m_currentBoundMaterialHints;

			// Shader support
		mutable hkgShaderContext*   m_shaderContext;
		hkgShader*		    m_currentBoundVertexShader;
		hkgShader*		    m_currentBoundPixelShader;
		hkgShaderCollection* m_globalShaderCollection;

		hkgBlendMatrixSet*  m_currentBlendMatrices;
		hkgVertexSet*		m_currentVertexSet;

		unsigned int		m_maxVerts; // usually (2^16 - 1) == 65,535 verts. The context will split to this 
										// boundary so the user does not need to worry about it.
		int		m_maxAnisotropy;
		float	m_shadowVsmBias;
		float   m_shadowVsmEpsilon;

		mutable hkgArray<hkgTexture*> m_texturePalette;
		mutable hkgArray<hkgVertexSet*> m_vertexSets;

		mutable class hkCriticalSection* m_contextLock;
		static HK_THREAD_LOCAL(int) m_threadLocalLockCount;

//#ifdef NV_P2P
        hkGpuInterface *m_gpuInterface;
//#endif
};

class hkgDisplayContextAutoLock
{
	public:
			/// Ctor
		hkgDisplayContextAutoLock(hkgDisplayContext* context)
			:	m_context(context)
		{
			context->lock();
		}
			/// Dtor
		~hkgDisplayContextAutoLock()
		{
			m_context->unlock();
		}
	private:
		hkgDisplayContext* m_context;
};

#include <Graphics/Common/DisplayContext/hkgDisplayContext.inl>

#endif //HK_GRAPHICS_DISPLAY_CONTEXT_H

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
