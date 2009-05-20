/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_NULL
#define HK_GRAPHICS_NULL

#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/Font/hkgFont.h>
#include <Graphics/Common/Texture/hkgTexture.h>
#include <Graphics/Common/Texture/hkgCubemapTexture.h>
#include <Graphics/Common/Camera/hkgCamera.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/DisplayObject/hkgInstancedDisplayObject.h>
#include <Graphics/Common/DisplayObject/hkgParticleDisplayObject.h>
#include <Graphics/Common/Geometry/FaceSet/hkgFaceSet.h>
#include <Graphics/Common/Shader/hkgShader.h>
#include <Graphics/Common/Geometry/BlendMatrixSet/hkgBlendMatrixSet.h>
#include <Graphics/Common/Geometry/VertexSet/hkgVertexSet.h>
#include <Graphics/Common/DisplayContext/hkgDisplayContext.h>
#include <Graphics/Common/Light/hkgLight.h>
#include <Graphics/Common/DisplayWorld/hkgDisplayWorld.h>
#include <Graphics/Common/Material/hkgMaterialManager.h>
#include <Graphics/Common/Light/hkgLightManager.h>
#include <Graphics/Common/Geometry/hkgMaterialFaceSet.h>
#include <Graphics/Common/Shader/hkgShaderContext.h>
#include <Graphics/Common/Texture/SkyBox/hkgSkyBox.h>

// [HCL-306] 
// When you ran the null renderer in the past, it would allocate no 
// space for buffers (verts, texture, etc). This causes demos such
// as cloth, that share the graphics buffers and copy into them,
// problems as there will be too many null access checks and so on.
// If we just allocate space in the buffers then the null renderer
// will always work.
#define HK_GRAPHICS_NULL_RENDERER_ALLOC_ENABLED 1

class hkgWindowNull : public hkgWindow
{
	public:

		static hkgWindow* HK_CALL createWindowNull()
		{
			return new hkgWindowNull;
		}

		bool clearBuffers() { return true; }
		bool swapBuffers() { return true; }
		int peekMessages(bool) { return HKG_WINDOW_MSG_CONTINUE; }
};

class hkgDisplayContextNull : public hkgDisplayContext
{
	protected:

		hkgDisplayContextNull(hkgWindow* owner)
			: hkgDisplayContext(owner)
		{
		}

	public:

		static hkgDisplayContext* HK_CALL createDisplayContextNull(hkgWindow* owner)
		{
			return new hkgDisplayContextNull(owner);
		}

		void setLightingState(bool) {}
		void setTexture2DState(bool) {}
		void setBlendState(bool) {}
		void setDepthReadState(bool) {}
		void setDepthWriteState(bool) {}
		void setCullFaceState(bool) {}
		void setWireframeState(bool) {}
		void setLightState(int,bool) {}
		void setDepthBias(float offset){}
		void pushMatrix() {}
		void popMatrix() {}
		void multMatrix(const float*) {}
		void beginGroup(unsigned short) {}
		void setCurrentColor3( const float* ) {}
		void setCurrentColor4( const float* ) {}
		void setCurrentColorPacked(unsigned int) {}
		void setCurrentTextureCoord( const float* ) {}
		void setCurrentNormal( const float* ) {}
		void setCurrentPosition( const float* ) {}
		void endGroup(bool useCurrentShaders = false) {}
		void flush(void) {}

};

class hkgViewportNull : public hkgViewport
{
	protected:

		hkgViewportNull(hkgWindow* owner)
			: hkgViewport(owner)
		{
		}

	public:

		static hkgViewport* HK_CALL createViewportNull(hkgWindow* owner)
		{
			return new hkgViewportNull(owner);
		}
};


class hkgCameraNull : public hkgCamera
{
	protected:

		hkgCameraNull()
			: hkgCamera(hkgCamera::DEFAULT_FROM, hkgCamera::DEFAULT_TO, hkgCamera::DEFAULT_UP, hkgCamera::DEFAULT_FOV)
		{
		}

	public:

		static hkgCamera* HK_CALL createCameraNull()
		{
			return new hkgCameraNull();
		}

		void setAsCurrent(class hkgDisplayContext *) {}
};

class hkgFontNull : public hkgFont
{
	protected:

		hkgFontNull()
		{
		}

	public:

		static hkgFont* HK_CALL createFontNull()
		{
			return new hkgFontNull();
		}

		void render(class hkgDisplayContext *,const char *,float,float,float []) {}

};

class hkgTextureNull : public hkgTexture
{
	protected:

		hkgTextureNull(hkgDisplayContext* context)
			: hkgTexture(context)
		{
		}

	public:


		static hkgTexture* HK_CALL createTextureNull(hkgDisplayContext* context)
		{
			return new hkgTextureNull(context);
		}
};

class hkgCubemapTextureNull : public hkgCubemapTexture
{
protected:

	hkgCubemapTextureNull(hkgDisplayContext* context)
		: hkgCubemapTexture(context)
	{
	}

public:


	static hkgCubemapTexture* HK_CALL createCubemapTextureNull(hkgDisplayContext* context)
	{
		return new hkgCubemapTextureNull(context);
	}
};


class hkgShaderNull : public hkgShader
{
	protected:

		hkgShaderNull()
		{
		}

	public:

		static hkgShader* HK_CALL createShaderNull(hkgDisplayContext* context)
		{
			return new hkgShaderNull();
		}

		virtual void setFloatInputIndex( int i, const float* v) { }
};

class hkgBlendMatrixSetNull : public hkgBlendMatrixSet
{

	protected:

		hkgBlendMatrixSetNull()
		{
		}

	public:

		static hkgBlendMatrixSet* HK_CALL createBlendMatrixSetNull(hkgDisplayContext* context)
		{
			return new hkgBlendMatrixSetNull();
		}

		virtual int getNumMatrices() const { return 0; }

		virtual void addMatrix(const float* m, int referenceID) { }

		virtual void setMatrix(int i, const float* m) { }

		virtual bool bind() { return false; }

		virtual void clear() { }
};

class hkgMaterialNull : public hkgMaterial
{
	protected:

		hkgMaterialNull()
		{
		}

	public:

		static hkgMaterial* HK_CALL createMaterialNull()
		{
			return new hkgMaterialNull;
		}
};

class hkgLightNull : public hkgLight
{
	protected:

		hkgLightNull()
		{
		}

	public:

		static hkgLight* HK_CALL createLightNull()
		{
			return new hkgLightNull;
		}

		void updateState(class hkgDisplayContext *) {}
};

class hkgVertexSetNull : public hkgVertexSet
{
	protected:

		hkgVertexSetNull(hkgDisplayContext* context)
			: hkgVertexSet( context), m_localBuffer(HK_NULL)
		{
		}

		virtual ~hkgVertexSetNull();

		void setNumVerts(int, HKG_VERTEX_FORMAT vertexFormat, bool dynamic = false);

		unsigned char *lock(HKG_LOCK_FLAG,int,int);

		void unlock(int,int);

		void setStreams();
		
	public:

		static hkgVertexSet* HK_CALL createVertexSetNull(hkgDisplayContext* context)
		{
			return new hkgVertexSetNull( context);
		}

		hkUint8* m_localBuffer;
};

class hkgShaderContextNull : public hkgShaderContext
{
	public:
		hkgShaderContextNull(hkgDisplayContext* owner) 
			: hkgShaderContext(owner)
		{
		}
		
};

class hkgInstancedDisplayObjectNull : public hkgInstancedDisplayObject
{
	public:		

		hkgInstancedDisplayObjectNull() 
			: hkgInstancedDisplayObject()
		{
		}

		virtual void setMaxNumObjects( int numObjects, bool allocateBuffers = true ) { }
		virtual void setTransform( const float* transform, int objectIdx ) { }
		virtual const float* getTransform( int objectIdx ) const { return m_trans; }
		virtual void setRenderMode( HKG_INSTANCED_DISPLAY_MODE m ) { m_renderMode = m; }
		static hkgInstancedDisplayObject* HK_CALL createInstancedDisplayObjectNull( hkgDisplayContext* context)
		{
			return new hkgInstancedDisplayObjectNull();
		}
};


class hkgParticleDisplayObjectNull : public hkgParticleDisplayObject
{
	public:		

		virtual void setMaxNumParticles( int numParticles, bool allocateBuffers = true ) { }
		virtual void setPosition( const float* position, int objectIdx ) { }
		virtual const float* getPosition( int objectIdx ) const;
		virtual void setVelocity( const float* vel, int objectIdx ) { }
		virtual const float* getVelocity( int objectIdx ) const;
		virtual void setUserData( const float* data, int objectIdx ) { }
		virtual const float* getUserData( int objectIdx ) const;
	
		static hkgParticleDisplayObject* HK_CALL createParticleDisplayObjectNull( hkgDisplayContext* context)
		{
			return new hkgParticleDisplayObjectNull();
		}
};

#endif // HK_GRAPHICS_NULL

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
