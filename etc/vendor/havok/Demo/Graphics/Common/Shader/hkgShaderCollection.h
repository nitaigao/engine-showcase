/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_SHADER_COLLECTION
#define HK_GRAPHICS_SHADER_COLLECTION

#include <Graphics/Common/Shader/hkgShader.h>

extern const hkClass hkgShaderCollectionClass;

/// Initial support for shaders / graphics hardware programs.
class hkgShaderCollection : public hkgReferencedObject
{
public:

	struct ShaderSet
	{
		hkgShader* vertexShader;
		hkgShader* pixelShader;
	};

	static hkgShaderCollection* (HK_CALL* create)();
		/// Default create function, use ::create instead.
	inline static hkgShaderCollection* defaultCreateInternal();

	inline void addShaderGrouping( hkgShader* vshader, hkgShader* pshader );

	inline void removeShader( hkgShader* shader);

		/// Given a context and the current held set of shaders, return the best fit.
	bool matchStyle( hkgDisplayContext* context, ShaderSet& shaderSet, HKG_MATERIAL_VERTEX_HINT hints );

		/// Handy funcs to find a shader to reuse say
	hkgShader* findPixelShaderByStyle( HKG_SHADER_RENDER_STYLE exactMatch ) const;
	hkgShader* findVertexShaderByStyle( HKG_SHADER_RENDER_STYLE exactMatch ) const;

	int getNumShaderSets() const;
	ShaderSet& getShaderSet(int i);
	const ShaderSet& getShaderSet(int i) const;

	inline void removeShaderSet( int i );

    // implement hkReferencedObject
    virtual const hkClass* getClassType() const { return &hkgShaderCollectionClass; }
    virtual void calcContentStatistics(hkStatisticsCollector* collector, const hkClass* cls) const;

protected:

	inline hkgShaderCollection();
	virtual ~hkgShaderCollection();

	hkArray< ShaderSet > m_shaders;
};

#include <Graphics/Common/Shader/hkgShaderCollection.inl>

#endif //HK_GRAPHICS_SHADER_COLLECTION

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
