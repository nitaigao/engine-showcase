/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

hkgShader::hkgShader() 
: m_userBindCallback(HK_NULL), m_allNormalInputs(0), m_allMatrixInputs(0), m_style(0)
{
}

HKG_SHADER_INPUT_CONSTANT hkgShader::getInputMask() const
{
	return m_allNormalInputs;
}


HKG_SHADER_MAT_INPUT_CONSTANT hkgShader::getMatInputMask() const
{
	return m_allMatrixInputs;
}


const hkgArray<hkgShader::ShaderInput>& hkgShader::getInputOrder() const
{
	return m_inputOrder;
}

void hkgShader::setFloatInput(HKG_SHADER_INPUT_CONSTANT semantic, int i, const float* v)
{
	if (m_allNormalInputs & semantic) // there somewhere (one of them anyway)
	{
		int sofar = -1;
		int ni = m_inputOrder.getSize();
		for (int j=0; j < ni; ++j)
		{
			if (m_inputOrder[j].normalSemantic == semantic)
			{
				if ( (++sofar) == i )
				{
					setFloatInputIndex(j, v);
					return; // done
				}
			}
		}
	}
}



void hkgShader::setMatFloatInput(HKG_SHADER_MAT_INPUT_CONSTANT semantic, int i, const float* v)
{
	if (m_allMatrixInputs & semantic) // there somewhere (one of them anyway)
	{
		int sofar = -1;
		int ni = m_inputOrder.getSize();
		for (int j=0; j < ni; ++j)
		{
			if (m_inputOrder[j].matrixSemantic == semantic)
			{
				if ( (++sofar) == i )
				{
					setFloatInputIndex(j, v);
					return; // done
				}
			}
		}
	}
}

inline HKG_SHADER_RENDER_STYLE hkgShader::getStyle() const
{
	return m_style;
}

inline HKG_SHADER_RENDER_STYLE hkgShader::getStyleForMaterialHint( HKG_MATERIAL_VERTEX_HINT hint )
{
	HKG_SHADER_RENDER_STYLE s = HKG_SHADER_RENDER_NOSTYLE;

	if (hint & HKG_MATERIAL_VERTEX_HINT_VCOLOR)
		s |= HKG_SHADER_RENDER_VERTEX_COLOR;

	if (hint & HKG_MATERIAL_VERTEX_HINT_TANGENTS)
		s |= HKG_SHADER_RENDER_TANGENTS;

	if (hint & HKG_MATERIAL_VERTEX_HINT_BLENDING)
		s |= HKG_SHADER_RENDER_BLENDING;

	return s;
}

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
