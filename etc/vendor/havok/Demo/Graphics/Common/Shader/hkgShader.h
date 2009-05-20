/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_SHADER
#define HK_GRAPHICS_SHADER

#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Graphics/Common/Shader/hkgShaderDefines.h>
#include <Graphics/Common/hkgObject.h>

extern const hkClass hkgShaderClass;

/// Initial support for shaders / graphics hardware programs.
class hkgShader : public hkgReferencedObject
{
public:

	struct ShaderInput
	{
		HKG_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( ShaderInput );

		// one of these will be 0:
		HKG_SHADER_MAT_INPUT_CONSTANT matrixSemantic;
		HKG_SHADER_INPUT_CONSTANT normalSemantic;

		int components; // 4x4 == 16, 3x4 == 12, etc
		int offsetInQuadWords; // offset from start of constants register bank to the input var to set (in 128bit values (1 dx9 constant == 4 floats == 1 vector4))
	};

public:

	static hkgShader* (HK_CALL* createVertexShader)(hkgDisplayContext* context);
	static hkgShader* (HK_CALL* createPixelShader)(hkgDisplayContext* context);

		/// Create a blank, platform specific, pixel or vertex shader object in the current context as given.
	static hkgShader* HK_CALL create(HKG_SHADER_TYPE type, hkgDisplayContext* context)
	{
		if ( type == HKG_VERTEX_SHADER) return createVertexShader(context);
		else return createPixelShader(context);
	}

		/// before you realize() a dx8 shader, you will need to specify the filename
		/// so that the realize can find the vsh with the constant defs in it
		/// on all other implementations this does nothing
	virtual void setShaderFileName(const char* fname) {}

	virtual const char* getDefaultFileNameExtension() { return HK_NULL; }
	
		/// Create the platform specific data for the shader. Will compile the shader from the given file.
		/// Indicate the general abilities of the shader by given a 'style' (num lights etc)
	virtual bool realize(hkIstream& istream, HKG_SHADER_RENDER_STYLE style);

	virtual bool realizeCompile(hkIstream& file, const char* entryPoint, HKG_SHADER_RENDER_STYLE style, const char* includePath = HK_NULL, const char** defines = HK_NULL /* each null terminated char will be defined as 1. End with a null string.*/ );

		/// Compile from file. This is best with the filename as it can then do local includes
		/// automatically.
	virtual bool realizeCompileFromFile(const char* filename, const char* entryPoint, HKG_SHADER_RENDER_STYLE style, const char* includePath = HK_NULL /*null == use filename*/, const char** defines = HK_NULL /* each null terminated char will be defined as 1. End with a null string.*/);

		/// Bind the data for the shader, set it as the current active shader program
	virtual bool bind();

			///
	virtual void uploadConstants();

		/// Unbind from the shader.
	virtual bool unbind();

		/// Free the platform specific resources etc used by the shader. Call realize again
		/// if you want to use the shader after this.
	virtual void free();

		/// -1 == after whatever was added last
	virtual void addNormalInput( HKG_SHADER_INPUT_CONSTANT semantic, int components, int indexInQuadWords = -1 );
	virtual void addMatInput( HKG_SHADER_INPUT_CONSTANT semantic, int components, int indexInQuadWords = -1 );

	virtual void addInput( const ShaderInput& si );

	inline HKG_SHADER_MAT_INPUT_CONSTANT getMatInputMask() const;
	inline HKG_SHADER_INPUT_CONSTANT getInputMask() const;

	inline const hkgArray<ShaderInput>& getInputOrder() const;

		/// set input i to the value pointed at by v
	virtual void setFloatInputIndex( int i, const float* v) = 0;

		/// set the input for the i-th of the given semantic to the value pointed at by v
	inline void setFloatInput( HKG_SHADER_INPUT_CONSTANT semantic, int i, const float* v);
	inline void setMatFloatInput( HKG_SHADER_MAT_INPUT_CONSTANT semantic, int i, const float* v);

	virtual void setFloatInputByName( const char* n, const float* v );

	inline HKG_SHADER_RENDER_STYLE getStyle() const;

	/// Callback that happens when a shader is bound, to allow the
	/// user code to set any extra constants that may be required that
	/// aren't automatic in HKG.
	void (HK_CALL* m_userBindCallback)(hkgShader* shader);

	inline static HKG_SHADER_RENDER_STYLE getStyleForMaterialHint( HKG_MATERIAL_VERTEX_HINT hint );

    // implement hkReferencedObject
    virtual const hkClass* getClassType() const { return &hkgShaderClass; }
    virtual void calcContentStatistics(hkStatisticsCollector* collector, const hkClass* cls) const {}

protected:

	inline hkgShader();
	virtual ~hkgShader() {}

	hkgArray<ShaderInput> m_inputOrder;
	HKG_SHADER_INPUT_CONSTANT m_allNormalInputs; // a quick bit mask to identify if the shader requires specific info without having to iterate the inputs
	HKG_SHADER_MAT_INPUT_CONSTANT m_allMatrixInputs; // a quick bit mask to identify if the shader requires specific info without having to iterate the inputs
	HKG_SHADER_RENDER_STYLE m_style;

	struct WellKnownNames
	{
		WellKnownNames() { }
		WellKnownNames(HKG_SHADER_INPUT_CONSTANT c, HKG_SHADER_MAT_INPUT_CONSTANT mc, const char* n, int es) : matConstant(mc), normalConstant(c), name(n), expectedSize(es) {}
		HKG_SHADER_MAT_INPUT_CONSTANT matConstant;
		HKG_SHADER_INPUT_CONSTANT normalConstant;
		const char* name;
		int expectedSize; // in floats
	};
	static WellKnownNames s_wellKnownNames[]; // terminated by (0,0)
};

#include <Graphics/Common/Shader/hkgShader.inl>

#endif //_SHADER

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
