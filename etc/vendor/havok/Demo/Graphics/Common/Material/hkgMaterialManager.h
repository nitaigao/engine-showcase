/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_GRAPHICS_MATERIAL_MANAGER
#define HK_GRAPHICS_MATERIAL_MANAGER

#include <Common/Base/Types/hkBaseTypes.h>
#include <Graphics/Common/Material/hkgMaterial.h>

/// The material manager is a container for all the materials used. An application does
/// not have to use the manager, but it could help in texture management in future.
/// It is cross platform and has no platform specific parts at this time.
class hkgMaterialManager : public hkgReferencedObject
{
	public:

			/// Create a hkgMaterialManager. This will return the
			/// same object on all platforms.
		static hkgMaterialManager* (HK_CALL *create)(hkgDisplayContext* context);
			/// Default create function, use ::create instead.
		inline static hkgMaterialManager* defaultCreateInternal(hkgDisplayContext* context);

			/// Is the i-th material textured (does it have a valid texture pointer)?
		inline bool materialIsTextured( int i ) const;

			/// Get the number of materials in this manager.
		inline int getNumMaterials() const;

			/// Get the i-th hkgMaterial in this manager. 'i' must be within range,
		inline hkgMaterial* getMaterial(int i);

			/// Get the const pointer to the i-th hkgMaterial in this manager. 'i' must be within range,
		inline const hkgMaterial* getMaterial(int i) const;

			/// It can be handy to just have some default, untextured, materials that
			/// get setup for use on random objects so at least they are not all the same
			/// color. By default there are 8 colors, which in ARGB format are:
			/// 	0xff7f7f7f
			/// 	0xffff3333
			/// 	0xff33ff33
			/// 	0xff3333ff
			/// 	0xffff33ff
			/// 	0xffffff33
			/// 	0xff33ffff
			/// 	0xffffffff
		inline int getNumDefaultMaterials() const;

			/// Get the const pointer to the i-th default material. 'i' must be within range.
			/// See getNumDefaultMaterials() for that range and also the list of default
			/// colors currently setup.
		inline const hkgMaterial* getDefaultMaterial(int i) const;

			/// Get the i-th default material. 'i' must be within range.
			/// See getNumDefaultMaterials() for that range and also the list of default
			/// colors currently setup.
		inline hkgMaterial* getDefaultMaterial(int i);

			/// Sets a default material
		inline void setDefaultMaterial(hkgMaterial* mat, int i);

			/// Add a material to the manager. This will increment the reference count on the material.
			/// You can add the same material to any number of managers, although there is usual only
			/// one manager.
		inline int addMaterial(hkgMaterial* m);

			/// Take the given materials out of the lists and transfer the reference to the caller.
			/// Does not affect default materials.
		int findMaterial(const hkgMaterial* m);
		void removeMaterial( int index );

			/// Realize all textures in hardware. It will simply call realize() on all material textures
			/// it finds. See hkgTexture::realize() for more information.
		void realizeTextures(hkgDisplayContext* context);

			/// Clear the list of materials (and release the reference to them).
		inline void clearMaterials();

			/// Print the material information to the given stream. It will assume the stream is text based.
		void print(hkOstream& stream) const;

		void lock() const;
		void unlock() const;

	protected:

		inline void clearDefaultMaterials();
		void createDefaultMaterials(hkgDisplayContext* context);

		hkgMaterialManager(hkgDisplayContext* context);
		virtual ~hkgMaterialManager();

		hkgArray<hkgMaterial*> m_materials;
		hkgArray<hkgMaterial*> m_defMaterials;

		mutable class hkCriticalSection*	m_materialLock;
};

#include <Graphics/Common/Material/hkgMaterialManager.inl>

#endif // HK_GRAPHICS_MATERIAL_MANAGER

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
