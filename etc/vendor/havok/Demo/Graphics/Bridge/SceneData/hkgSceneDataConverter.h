/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_BRIDGE2_SCENEDATA_CONV_H
#define HK_GRAPHICS_BRIDGE2_SCENEDATA_CONV_H

#include <Graphics/Common/hkgObject.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObjectDefines.h>
#include <Common/Base/Container/Array/hkObjectArray.h>
#include <Common/SceneData/Mesh/hkxMeshSectionUtil.h>
#include <Common/SceneData/Graph/hkxNode.h>

#include <Graphics/Bridge/SceneData/hkgAssetConverter.h>

class hkSerializerSession;
class hkgDisplayWorld;
class hkgDisplayContext;
class hkgShaderLib;
class hkgBlendMatrixSet;
class hkgVertexBufferWrapper;
class hkgVertexSet;
class hkxMesh;

class hkgSceneDataFilterInterface
{
	public:

		enum NodeStatus
		{
			NODE_OK = 0,
			REJECT_NODE_NO_RECURSE,
			REJECT_NODE_AND_RECURSE
		};

	virtual ~hkgSceneDataFilterInterface () {}
	virtual NodeStatus testNode( const hkxNode* node ) const = 0;
};

class hkgDefaultSceneDataFilter : public hkgReferencedObject, public hkgSceneDataFilterInterface
{
public:

	virtual ~hkgDefaultSceneDataFilter() {} 

	// by calling this it will assume you want to disallow all others not listed
	// str not copied. Keep alive while filter active.
	void allow(const char* str ) 
	{ 
		m_onlyAllow.pushBack(str);
	}

	// by calling this it will assume you want to allow all others not listed
	// str not copied. Keep alive while filter active.
	void disallow(const char* str)
	{
		m_onlyDisallow.pushBack(str);
	}

	void clear()
	{
		m_onlyAllow.setSize(0);
		m_onlyDisallow.setSize(0);
	}

	// Handy function to swap between a sub set to allow to its inverse
	void invert()
	{
		m_onlyAllow.swap( m_onlyDisallow );
	}

	virtual NodeStatus testNode( const hkxNode* node  ) const
	{
		if (node->m_name)
		{
			if (m_onlyAllow.getSize() > 0)
			{
				for (int ai=0; ai < m_onlyAllow.getSize(); ++ai)
				{
					if (hkString::strCmp(m_onlyAllow[ai], node->m_name) == 0)
						return NODE_OK;
				}
				return REJECT_NODE_AND_RECURSE;
			}
			else if (m_onlyDisallow.getSize() > 0)
			{
				for (int dai=0; dai < m_onlyDisallow.getSize(); ++dai)
				{
					if (hkString::strCmp(m_onlyDisallow[dai], node->m_name) == 0)
						return REJECT_NODE_AND_RECURSE;
				}
			}
		}
		return NODE_OK;
	}

	hkArray<const char*> m_onlyAllow;
	hkArray<const char*> m_onlyDisallow;
};



class hkgSceneDataConverter : public hkgReferencedObject
{
public:



		/// Make a converter for a given world and context. Will reference both.
	hkgSceneDataConverter( hkgDisplayWorld* w, hkgDisplayContext* ctx );

		/// dtor
	~hkgSceneDataConverter();

	inline void setAllowMeshSharing( hkBool on );

		/// Normally you only convert a material once. 
		/// But sometimes you might want to add shaders to just once instance, etc
		/// so you can change the material creation behaviour here.
	inline void setAllowMaterialSharing( hkBool on );

		/// Normally you only convert a texture once. 
		/// But sometimes you might want to change the surface of just once instance, etc
	inline void setAllowTextureSharing( hkBool on );

		/// Normally the converter, if shaders are supported by the current h/w,  
		/// imports all textures and tangents etc. If you9 are running on non shader h/w
		/// but still want that support, or if you are running with not enough shader 
		/// support and thus want to force the extra textures off, this is the weay to do it.
	inline void setAllowExtraTextures( hkBool on );

		/// Normally textures are mipmapped.
		/// But sometimes you might want to save memory
	inline void setAllowTextureMipmap( hkBool on );

		/// Normally textures are not compressed (unless read in as such).
		/// But sometimes you might want to save memory
	inline void setAllowTextureCompression( hkBool on );

		/// When creating a texture, turn on anisotropic filtering.
	inline void setAllowTextureAnisotropicFilter( hkBool on );

		/// Normally you would only convert materials as used by materials.
		/// As such, you let the convert create the materials when it creates
		/// each mesh. If you disable this, all materials will be created through convert()
		/// and the meshes will not recurse to create materials but just search the mat list.
	inline void setMeshMaterialCreation( hkBool on );

		/// If you want to hardware skin (or DX or OGL skin) 
		/// using HKG, the converter needs to pass the indices and weights 
		/// to HKG and to setup appropriate blend matrices holders.
	inline void setAllowHardwareSkinning( hkBool on );

		/// If you want to have the hkxMaterials search the Shader Library
	inline void setShaderLibraryEnabled( hkBool on );

		/// You can undo all the above settings with this call, to revert to the original defaults.
		/// See related .inl file for the current defaults.
	inline void revertToDefaultSettings();

		/// You can add paths to be searched in case non-embedded textures are not found in the stored
		/// absolute location.
	inline void addTextureSearchPath(const char* path);

		/// Convert any known types from the list.
		/// Will cast to the correct types and call the sub convert methods
		/// as required. The mapping of converted objects will be stored for 
		/// reference that you can then purge when you have finished with it
	void convert( const class hkxScene* scene, 
		hkgAssetConverter::ConvertMask convMask = hkgAssetConverter::CONVERT_ALL, 
		bool addObjectsToWorld = true, hkgSceneDataFilterInterface* nodeFilter = HK_NULL);

		/// A more selective convert, without having to know the mesh, and with more control over 
		/// replication. Returns the new (or reused) object if successful; If you disable the object cache, a new object will 
		/// be created each time.
		/// Note that is node based, so that copies etc have transform / name etc
	hkgDisplayObject* convertMesh( const class hkxScene* scene, const char* meshNodeName, bool disableObjectCache, HKG_DISPLAY_OBJECT_COPY_FLAGS copyFlags = HKG_DISPLAY_OBJECT_SHARE_GEOM,  hkgAssetConverter::ConvertMask convMask = hkgAssetConverter::CONVERT_ALL, bool addObjectsToWorld = true, hkgDisplayObject** sourceObjectUsed = HK_NULL ); // will find the hkxMesh and call the folloeing func
	
	hkgDisplayObject* convertMesh( const class hkxScene* scene, const hkxNode* nodeToConvert, bool disableObjectCache, HKG_DISPLAY_OBJECT_COPY_FLAGS copyFlags = HKG_DISPLAY_OBJECT_SHARE_GEOM,  hkgAssetConverter::ConvertMask convMask = hkgAssetConverter::CONVERT_ALL, bool addObjectsToWorld = true, hkgDisplayObject** sourceObjectUsed = HK_NULL );

		/// Called by the main convert function, but you can call it directly of you like.
	int convertMesh( const class hkxNode* node, const class hkxMesh* mesh, const hkMatrix4& worldTransform, hkBool enableLighting, hkBool& wasSkin, hkgAssetConverter::ConvertMask convMask = hkgAssetConverter::CONVERT_ALL, bool addObjectsToWorld = true);
		
		/// Called by the main convert function, but you can call it directly of you like.
	int convertLight( const class hkxLight* light, bool addLightsToWorld = true );

		/// Called by the main convert function, but you can call it directly of you like.
	int convertMaterial( const class hkxMaterial* material, bool diffuseOnly = false );
	
		/// Called by the main convert function, but you can call it directly of you like.
	int convertInplaceTexture( const class hkxTextureInplace* texture );

		/// Called by the main convert function, but you can call it directly of you like.
	int convertTextureFile( const class hkxTextureFile* texture );

		/// Called by the main convert function, but you can call it directly of you like.
	int convertCamera( const class hkxCamera* camera, const char* name );

		/// Called by the main convert function, but you can call it directly of you like.
	void convertNode( const class hkxNode* node, const hkMatrix4& worldTransform, 
		hkgAssetConverter::ConvertMask convMask = hkgAssetConverter::CONVERT_ALL, 
		bool addObjectsToWorld = true, hkgSceneDataFilterInterface* nodeFilter = HK_NULL );

		// Some util funcs
	
	hkgDisplayObject* findFirstDisplayObjectUsingMesh (const hkxMesh* mesh ) const;
			/// Finds the display objects associated with the given mesh (empty if none found)
	void findDisplayObjectsUsingMesh (const hkxMesh* mesh, hkgArray<hkgDisplayObject*>& objs ) const;
	
		/// Updates the position and orientation of the given attachment display object
	void updateAttachment (hkgDisplayObject* displayObject, const hkMatrix4& worldTransform);

		/// Find all nodes starting with given string (say "SHADOWPLANE") and assume they 
		/// represent the frame for a shadow plane, to be added to all lights.
	void convertShadowPlanes( const class hkxNode* node, const char* prefix, const hkMatrix4& curWorldT );

		/// After convert, you may have converted weighted meshes to hardware skins, so 
		/// you will want to setup the mappings for them so they know what matrices to use.
		/// IndexMapping is the same as the Havok animation systems mapping structure in the mesh binding
		/// so you can reinterpret cast from one to another. 
		/// Returns true if successful.
	bool setupHardwareSkin( hkgDisplayContext* context, hkxMesh* skin, hkgAssetConverter::IndexMapping* mappings, hkInt32 numMappings, hkInt16 fullPaletteSize, hkBool allowCulling = false);
	
		/// Update graphics rep of the skin based on the given pallete. Just sets
		/// the blend matrices used in HKG so is only called if doing hardware or DX skinning.
		/// Provide a new object AABB ('local', ie, will assume *skinTransform == world) if the aabb if you are using culling and/or shadows.
	void updateSkin( const hkxMesh* skin, hkArray< hkTransform >& matrixPallete, const hkTransform& skinTransform, const class hkAabb* pLocalAabb = HK_NULL);

		/// As this class stores the mappings on what it converts, at some stage
		/// you will want to delete that mapping and release the reference held to
		/// the mapped objects.
	void purgeHeldMappings();

		/// Similar to the above, but will actually remove all the objects from the world
	void removeCreatedObjectsFromWorld(bool purgeMappings = true);
	void addCreatedObjectsBackIntoWorld(); // will revert the effect of removeCreatedObjectsFromWorld(false);

		// A list of mappings use to find the graphics versions of objects
	hkgArray<hkgAssetConverter::Mapping> m_meshes; // a hkxMesh can map to >1 hkgDisplayObject as multiple copies etc supported
	hkgArray<hkgAssetConverter::NodeMapping> m_meshNodes;
	hkgArray<hkgAssetConverter::Mapping> m_lights;	
	hkgArray<hkgAssetConverter::Mapping> m_cameras;	
	hkgArray<hkgAssetConverter::Mapping> m_vertexBuffers;
	hkgAssetConverter::MaterialCache    m_materialCache;

		// The handles to the display objects that have been added to the scene
	hkgArray<class hkgDisplayObject*> m_addedDisplayObjects;
	hkgArray<class hkgLight*> m_addedLights;
	hkgArray<class hkgMaterial*> m_addedMaterials;
	hkgArray<hkgAssetConverter::SkinPaletteMap* > m_addedSkins; 

	// The context that we will create the objects in
	hkgDisplayContext* m_context;

	// The world that the objects are added to
	hkgDisplayWorld* m_world;

	// The shader library that we will use on
	// platforms that support shaders to match the 
	// materials with a proper shader resource
	hkgShaderLib* m_shaderLibrary;

public:

	hkBool m_bAllowMeshSharing;
	hkBool m_bAllowMaterialSharing;
	hkBool m_bAllowTextureSharing;
	hkBool m_bAllowTextureMipMaps;
	hkBool m_bInvertNormalMapGreenChannel;
	hkBool m_bAllowTextureCompression;
	hkBool m_bAllowTextureAnisotropicFilter;
	hkBool m_bAllowHardwareSkinning;
	hkBool m_bMeshMaterialCreation;
	hkBool m_bEnableShaderLibLookup; // true by default on all SM 2.0 cards/platforms and above. Add search paths to the shader lib in this case.
};

#include <Graphics/Bridge/SceneData/hkgSceneDataConverter.inl>

#endif //HK_GRAPHICS_BRIDGE2_SCENEDATA_CONV_H

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
