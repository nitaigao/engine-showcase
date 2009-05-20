/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef DEMO_PHYSICS_ASSET_MGT_UTIL_H
#define DEMO_PHYSICS_ASSET_MGT_UTIL_H

#include <Common/Base/hkBase.h>
#include <Physics/Utilities/Serialize/hkpDisplayBindingData.h>

class hkLoader;
class hkpRigidBody;
class hkgDisplayObject;
class hkpPhysicsSystem;


class hkRigidBodyDemoAsset : public hkReferencedObject
{
	public:

		hkRigidBodyDemoAsset() : m_rigidBody(HK_NULL), m_displayObject(HK_NULL) { }
		
			/// Will release the two (assumed) references
		virtual ~hkRigidBodyDemoAsset();

			/// Shallow copy of the rb and the display (will share as much as possible)
		hkRigidBodyDemoAsset* clone() const;

		hkpRigidBody* m_rigidBody;
		hkgDisplayObject* m_displayObject;
};

class hkPhysicsSystemDemoAsset : public hkReferencedObject
{
	public: 

		hkPhysicsSystemDemoAsset() : m_physicsSystem(HK_NULL) { }

			// Will release the (assumed) references
		virtual ~hkPhysicsSystemDemoAsset();

			/// Shallow copy of the rb assets (will share as much as possible) and the system
		hkPhysicsSystemDemoAsset* clone() const;
		
		hkpPhysicsSystem* m_physicsSystem;
		hkArray< hkRigidBodyDemoAsset* > m_rigidBodyAssets;
};


// This class provides DemoAsset loading functions
class hkPhysicsAssetLoader
{
public:
	/// Load an array of rigid bodies with a graphics bindings
	static int HK_CALL loadAllRigidBodyDemoAssets(const char* assetPath, hkLoader* loader, hkgDisplayContext* graphicsContext, hkArray<hkRigidBodyDemoAsset*>& assets );

	/// Load a single rigid body with a simple demo graphics binding (from a collection)
	static hkRigidBodyDemoAsset* HK_CALL loadRigidBodyDemoAsset(const char* assetPath, const char* assetName, hkLoader* loader, hkgDisplayContext* graphicsContext);
	
	/// Load a complex system with demo graphics bindings (loads only the first system found)
	static hkPhysicsSystemDemoAsset* HK_CALL loadPhysicsSystemDemoAsset(const char* assetPath, hkLoader* loader, hkgDisplayContext* graphicsContext);
};

#endif // DEMO_PHYSICS_ASSET_MGT_UTIL_H

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
