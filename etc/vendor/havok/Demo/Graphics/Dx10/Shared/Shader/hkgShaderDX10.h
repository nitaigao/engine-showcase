/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_SHADER_DX10_H
#define HK_GRAPHICS_SHADER_DX10_H

#include <Graphics/Common/Shader/hkgShader.h>
#include <Graphics/Dx10/Shared/DisplayContext/hkgDisplayContextDX10.h>

class hkgShaderDX10 : public hkgShader
{
public:

		/// Direct access to the device etc
	inline ID3D10Device* getDevice() const;

	inline ID3D10Blob* getInputSignature() const;
	
	inline ID3D10Blob* getByteCode() const;
	
	virtual void setFloatInputIndex( int i, const float* v);

	virtual void setFloatInputByName( const char* n, const float* v );

	virtual void uploadConstants();

	virtual const char* getDefaultFileNameExtension() { return ".hlsl"; }

protected:		
	
	void setupInputSignature(void* shaderByteCode, hkUint32 bufSize);
	void setupRefelection(void* shaderByteCode, hkUint32 bufSize);
	
	void setupConstantBuffers();
	void updateConstantBuffers();

	inline hkgShaderDX10(hkgDisplayContext* context); 
	virtual ~hkgShaderDX10();

	hkgDisplayContextDX10*  m_context;
	ID3D10Blob*				m_inputSignature; // for VS and GS shaders, taken from the byte code ptr
	ID3D10Blob*				m_byteCode;      
	ID3D10ShaderReflection* m_shaderReflection;
	hkArray<ID3D10Buffer*>	m_constantBuffers;
	hkArray<char*>			m_constantBufferData;
	hkArray<hkUint32>		m_constantBufferSize;
	hkArray<hkUint32>		m_constantBufferUpdates;

	struct Variable
	{
		hkUint32 constantBufferIndex;
		D3D10_SHADER_VARIABLE_DESC desc;
	};
	hkArray<Variable> m_wellKnownVariables;

};

#include <Graphics/Dx10/Shared/Shader/hkgShaderDX10.inl>

#endif // HK_GRAPHICS_SHADER_DX10_H

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
