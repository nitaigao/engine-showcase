/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_DISPLAY_CONTEXT_OGL_H
#define HK_GRAPHICS_DISPLAY_CONTEXT_OGL_H

#include <Graphics/Common/DisplayContext/hkgDisplayContext.h>

class hkgDisplayContextOGL : public hkgDisplayContext
{
public:

	virtual void setLightingState(bool on);
	virtual void setTexture2DState(bool on);
	virtual void setBlendState(bool on);
	virtual void setDepthReadState(bool on);
	virtual void setDepthWriteState(bool on);
	virtual void setCullFaceState(bool on);
	virtual void setWireframeState(bool on);
	virtual void setDepthBias(float offset){}

	virtual void setLightState(int light, bool on);
	
	virtual void popMatrix();
	virtual void pushMatrix();
	virtual void multMatrix( const float* m);
	virtual void beginGroup(HKG_IMM_GROUP type);
	virtual void  setCurrentColor3(  const float* c);
	virtual void  setCurrentColor4(  const float* c);
	virtual void  setCurrentColorPacked( unsigned int c);
	virtual void  setCurrentTextureCoord( const float* uv);
	virtual void  setCurrentNormal( const float* n);
	virtual void  setCurrentPosition( const float* p);
	virtual void endGroup(bool useCurrentShaders = false);
	virtual void flush();

	// OpenGL supports two sided lighting natively (only platfrom really)
	void setTwoSidedLightingState(bool on);
	inline bool getTwoSidedLightingState() const;

	inline CGcontext getCgContext() const;
	inline CGprofile getCgVertexProfile() const;
	inline CGprofile getCgFragmentProfile() const;

	static hkgDisplayContext* createDisplayContextOGL(hkgWindow* owner) { return new hkgDisplayContextOGL(owner); }

	inline void restartMatrixStack() { m_matrixStack.setSize(0); }
	
protected:

	bool m_twoSidedLighting;
	CGcontext m_cgContext;
	CGprofile m_vertexProfile;
	CGprofile m_fragmentProfile;

	hkArray<float>	m_matrixStack; // world matrix (as modelview has both it + cam in it), handy for shaders

	hkgDisplayContextOGL(hkgWindow* owner); 
	virtual ~hkgDisplayContextOGL();
};

#include <Graphics/Ogl/Shared/DisplayContext/hkgDisplayContextOGL.inl>

#endif //HK_GRAPHICS_DISPLAY_CONTEXT_OGL_H
	

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
