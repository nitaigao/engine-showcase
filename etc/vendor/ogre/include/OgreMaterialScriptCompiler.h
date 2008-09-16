/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org

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

#ifndef __MaterialScriptScompiler_H__
#define __MaterialScriptScompiler_H__

#include "OgreCompiler2Pass.h"
#include "OgreTextureUnitState.h"
#include "OgreMaterial.h"
#include "OgreGpuProgram.h"
#include "OgreStringVector.h"

namespace Ogre {

    class _OgreExport MaterialScriptCompiler : public Compiler2Pass
    {

    public:
        MaterialScriptCompiler(void);
        ~MaterialScriptCompiler(void);

        /** gets BNF Grammer for Compositor script.
        */
        virtual const String& getClientBNFGrammer(void) const;

        /** get the name of the BNF grammer.
        */
        virtual const String& getClientGrammerName(void) const;

        /** Compile a material script from a data stream using a specific resource group name.
        @param stream Weak reference to a data stream which is the source of the material script
        @param groupName The name of the resource group that resources which are
			parsed are to become a member of. If this group is loaded or unloaded,
			then the resources discovered in this script will be loaded / unloaded
			with it.
        @param allowOverride if material or gpu program name already exists then use definition
            in the material script being parsed to override the existing instance.  The default
            value is false which causes an exception to be thrown if a duplicate is found.
        */
        void parseScript(DataStreamPtr& stream, const String& groupName, const bool allowOverride = false);

    protected:
	    // Token ID enumeration
	    enum TokenID {
		    // Terminal Tokens section
            ID_UNKOWN = 0,
            // GPU Program
            ID_VERTEX_PROGRAM, ID_FRAGMENT_PROGRAM,

            // material
            ID_CLONE,

            // technique

            // pass

            ID_VERTEXCOLOUR,
            // scene blend
            ID_COLOUR_BLEND, ID_DEST_COLOUR,
            ID_SRC_COLOUR, ID_ONE_MINUS_DEST_COLOUR, ID_ONE_MINUS_SRC_COLOUR,
            ID_DEST_ALPHA, ID_SRC_ALPHA, ID_ONE_MINUS_DEST_ALPHA, ID_ONE_MINUS_SRC_ALPHA,
            // Depth
            ID_ALWAYS_FAIL, ID_ALWAYS_PASS,
            ID_LESS_EQUAL, ID_LESS, ID_EQUAL, ID_NOT_EQUAL, ID_GREATER_EQUAL, ID_GREATER,
            // culling
            ID_CLOCKWISE, ID_ANTICLOCKWISE,
            ID_CULL_BACK, ID_CULL_FRONT,
            // shading
            ID_FLAT, ID_GOURAUD, ID_PHONG,
            // polygon mode
            ID_SOLID, ID_WIREFRAME, ID_POINTS,
            // fog overide
            ID_EXP, ID_EXP2,
            // iteration
            ID_ONCE, ID_ONCE_PER_LIGHT, ID_PER_LIGHT, ID_PER_N_LIGHTS, ID_DIRECTIONAL, ID_SPOT,

            // texture unit state
            // texture
            ID_1D, ID_2D, ID_3D, ID_CUBIC, ID_UNLIMITED, ID_ALPHA,
            // cubic texture
            ID_SEPARATE_UV, ID_COMBINED_UVW,
            // address mode
            ID_WRAP, ID_CLAMP, ID_MIRROR, ID_BORDER,
            // filtering
            ID_BILINEAR, ID_TRILINEAR, ID_ANISOTROPIC,
            // color op
            ID_REPLACE,
            ID_SOURCE1, ID_SOURCE2, ID_MODULATE_X2, ID_MODULATE_X4, ID_ADD_SIGNED,
            ID_ADD_SMOOTH, ID_SUBTRACT, ID_BLEND_DIFFUSE_COLOUR, ID_BLEND_DIFFUSE_ALPHA,
            ID_BLEND_TEXTURE_ALPHA, ID_BLEND_CURRENT_ALPHA, ID_BLEND_MANUAL, ID_DOTPRODUCT,
            ID_SRC_CURRENT, ID_SRC_TEXTURE, ID_SRC_DIFFUSE, ID_SRC_SPECULAR, ID_SRC_MANUAL,

            // env map
            ID_SPHERICAL, ID_PLANAR, ID_CUBIC_REFLECTION, ID_CUBIC_NORMAL,
            // wave transform
            ID_SCROLL_X, ID_SCROLL_Y, ID_SCALE_X, ID_SCALE_Y, ID_SINE, ID_TRIANGLE,
            ID_SQUARE, ID_SAWTOOTH, ID_INVERSE_SAWTOOTH, ID_ROTATE,
			// content type
			ID_NAMED, ID_SHADOW,

            // GPU program references
            // GPU Parameters

            // general
            ID_ON, ID_OFF, ID_TRUE, ID_FALSE, ID_NONE, ID_POINT, ID_LINEAR, ID_ADD, ID_MODULATE, ID_ALPHA_BLEND,
            ID_ONE, ID_ZERO, ID_VERTEX, ID_FRAGMENT,

            // where auto generated tokens start so donot remove
            ID_AUTOTOKENSTART
        };

        /** Enum to identify material sections. */
        enum MaterialScriptSection
        {
            MSS_NONE,
            MSS_MATERIAL,
            MSS_TECHNIQUE,
            MSS_PASS,
            MSS_TEXTUREUNIT,
            MSS_PROGRAM_REF,
		    MSS_PROGRAM,
            MSS_DEFAULT_PARAMETERS,
		    MSS_TEXTURESOURCE
        };
	    /** Struct for holding a program definition which is in progress. */
	    struct MaterialScriptProgramDefinition
	    {
		    String name;
		    GpuProgramType progType;
            String language;
		    String source;
		    String syntax;
            bool supportsSkeletalAnimation;
		    bool supportsMorphAnimation;
		    ushort supportsPoseAnimation; // number of simultaneous poses supported
            bool usesVertexTextureFetch;
			std::vector<std::pair<String, String> > customParameters;
	    };
        /** Struct for holding the script context while parsing. */
        struct MaterialScriptContext
        {
            MaterialScriptSection section;
		    String groupName;
            MaterialPtr material;
            Technique* technique;
            Pass* pass;
            TextureUnitState* textureUnit;
            GpuProgramPtr program; // used when referencing a program, not when defining it
            bool isProgramShadowCaster; // when referencing, are we in context of shadow caster
            bool isVertexProgramShadowReceiver; // when referencing, are we in context of shadow caster
            bool isFragmentProgramShadowReceiver; // when referencing, are we in context of shadow caster
            bool allowOverride; // if true then allow existing materials and gpu programs to be overriden
            GpuProgramParametersSharedPtr programParams;
			ushort numAnimationParametrics;
		    MaterialScriptProgramDefinition* programDef; // this is used while defining a program

		    int techLev,	//Keep track of what tech, pass, and state level we are in
			    passLev,
			    stateLev;
            // container of token que positions for default params that require pass 2 processing
            std::vector<size_t> pendingDefaultParams;

            AliasTextureNamePairList textureAliases;
        };

        MaterialScriptContext mScriptContext;

        typedef void (MaterialScriptCompiler::* MSC_Action)(void);
        typedef std::map<size_t, MSC_Action> TokenActionMap;
        typedef TokenActionMap::iterator TokenActionIterator;
        /** Map of Token value as key to an Action.  An Action converts tokens into
            the final format.
            All instances use the same Token Action Map.
        */
        static TokenActionMap mTokenActionMap;

        /** Execute an Action associated with a token.  Gets called when the compiler finishes tokenizing a
            section of the source that has been parsed.
        **/
        virtual void executeTokenAction(const size_t tokenID);
        /** Get the start position of auto generated token IDs.
        */
        virtual size_t getAutoTokenIDStart() const {return ID_AUTOTOKENSTART;}

        /** Associate all the lexemes used in a material script with their corresponding tokens and actions.
        **/
        virtual void setupTokenDefinitions(void);
        void addLexemeTokenAction(const String& lexeme, const size_t token, const MSC_Action action = 0);
        /** Associate all the lexemes used in a material script with their corresponding actions and have
            token IDs auto-generated.
        **/
        void addLexemeAction(const String& lexeme, const MSC_Action action) {addLexemeTokenAction(lexeme, 0, action);}

        void logParseError(const String& error);

        // support methods that convert tokens
        ColourValue _parseColourValue(void);
        CompareFunction convertCompareFunction(void);

        // Token Actions which get called when tokens are created during parsing.
        void parseOpenBrace(void);
        void parseCloseBrace(void);
        // material section Actions
        void parseMaterial(void);
        void parseLodDistances(void);
        void parseReceiveShadows(void);
        void parseTransparencyCastsShadows(void);
        void parseSetTextureAlias(void);
        // Technique related actions
        void parseTechnique(void);
        void parseScheme(void);
        void parseLodIndex(void);
        // Pass related Actions
        void parsePass(void);
        void parseAmbient(void);
        void parseDiffuse(void);
        void parseSpecular(void);
        void parseEmissive(void);
        void parseSceneBlend(void);
        SceneBlendFactor convertBlendFactor(void);
        void parseDepthCheck(void);
        void parseDepthWrite(void);
        void parseDepthFunc(void);
        void parseDepthBias(void);
        void parseAlphaRejection(void);
        void parseCullHardware(void);
        void parseCullSoftware(void);
        void parseLighting(void);
        void parseShading(void);
        void parsePolygonMode(void);
        void parseFogOverride(void);
        void parseMaxLights(void);
		void parseStartLight(void);
        void parseIteration(void);
        void parseIterationLightTypes(void);
        void parseColourWrite(void);
        void parsePointSize(void);
        void parsePointSprites(void);
        void parsePointSizeMin(void);
        void parsePointSizeMax(void);
        void parsePointSizeAttenuation(void);
        // Texture Unit related Actions
        void parseTextureUnit(void);
        void parseTextureAlias(void);
        void parseTexture(void);
        void parseAnimTexture(void);
        void parseCubicTexture(void);
        void parseTexCoord(void);
        TextureUnitState::TextureAddressingMode convTexAddressMode(void);
        void parseTexAddressMode(void);
        void parseTexBorderColour(void);
        void parseFiltering(void);
        FilterOptions convertFiltering();
        void parseMaxAnisotropy(void);
        void parseMipMapBias(void);
        void parseColourOp(void);
        void parseColourOpEx(void);
        LayerBlendOperationEx convertBlendOpEx(void);
        LayerBlendSource convertBlendSource(void);
        void parseColourOpMultipassFallback(void);
        void parseAlphaOpEx(void);
        void parseEnvMap(void);
        void parseScroll(void);
        void parseScrollAnim(void);
        void parseRotate(void);
        void parseRotateAnim(void);
        void parseScale(void);
        void parseWaveXform(void);
        void parseTransform(void);
        void parseTextureCustomParameter(void);
    	void parseBindingType(void);
		void parseContentType(void);

        // GPU Program
        void parseGPUProgram(void);
        void parseProgramSource(void);
        void parseProgramSyntax(void);
        void parseProgramCustomParameter(void);
        void parseDefaultParams(void);
        void parseProgramSkeletalAnimation(void);
        void parseProgramMorphAnimation(void);
        void parseProgramPoseAnimation(void);
        void parseProgramVertexTextureFetch(void);
        void parseVertexProgramRef(void);
        void parseFragmentProgramRef(void);
        void parseShadowCasterVertexProgramRef(void);
        void parseShadowReceiverVertexProgramRef(void);
        void parseShadowReceiverFragmentProgramRef(void);
        void parseParamIndexed(void);
        void parseParamIndexedAuto(void);
        void parseParamNamed(void);
        void parseParamNamedAuto(void);
        void processManualProgramParam(bool isNamed, const String commandName, 
			size_t index = 0, const String& paramName = StringUtil::BLANK);
		void processAutoProgramParam(bool isNamed, const String commandName, 
			size_t index, const String& paramName = StringUtil::BLANK);


    	void finishProgramDefinition(void);

    };
}

#endif
