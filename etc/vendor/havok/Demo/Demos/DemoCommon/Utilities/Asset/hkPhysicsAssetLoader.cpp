/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#include <Demos/demos.h>
#include <Common/Base/hkBase.h>

#include <Demos/DemoCommon/Utilities/Asset/hkPhysicsAssetLoader.h>

#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>
#include <Physics/Utilities/Serialize/hkpDisplayBindingData.h>
#include <Physics/Utilities/VisualDebugger/Viewer/Collide/hkpShapeDisplayViewer.h>
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Graphics/Bridge/SceneData/hkgAssetConverter.h>

#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Common/DisplayWorld/hkgDisplayWorld.h>


hkRigidBodyDemoAsset* hkRigidBodyDemoAsset::clone() const
{
	hkRigidBodyDemoAsset* newAsset = new hkRigidBodyDemoAsset();
	newAsset->m_rigidBody = m_rigidBody->clone();
	newAsset->m_displayObject = m_displayObject->copy( HKG_DISPLAY_OBJECT_SHARE_GEOM, HK_NULL );
	newAsset->m_rigidBody->editProperty(HK_PROPERTY_DISPLAY_PTR, newAsset->m_displayObject);
	return newAsset;
}

hkRigidBodyDemoAsset::~hkRigidBodyDemoAsset()
{
	if (m_rigidBody)
	{
		// Deallocate the unlocked properties
		m_rigidBody->clearAndDeallocateProperties();
		m_rigidBody->removeReference();
	}
	if (m_displayObject)
	{
		m_displayObject->removeReference();
	}
}


hkPhysicsSystemDemoAsset* hkPhysicsSystemDemoAsset::clone() const
{
	hkPhysicsSystemDemoAsset* newAsset = new hkPhysicsSystemDemoAsset();
	newAsset->m_physicsSystem = m_physicsSystem->clone();

	for (int i=0; i<m_rigidBodyAssets.getSize(); ++i)
	{
		hkRigidBodyDemoAsset* rba = new hkRigidBodyDemoAsset();
		rba->m_displayObject = m_rigidBodyAssets[i]->m_displayObject->copy(HKG_DISPLAY_OBJECT_SHARE_GEOM, HK_NULL);
		
		// Find which new rigid body in the new system referred to this display object
		void* ptr = m_rigidBodyAssets[i]->m_rigidBody->getProperty(HK_PROPERTY_DISPLAY_PTR).getPtr();
		const hkArray<hkpRigidBody*>& rbs = newAsset->m_physicsSystem->getRigidBodies();
		for (int j=0; j<rbs.getSize(); ++j)
		{
			if( rbs[j]->getProperty(HK_PROPERTY_DISPLAY_PTR).getPtr() == ptr )
			{
				rba->m_rigidBody = rbs[j];
				rba->m_rigidBody->editProperty(HK_PROPERTY_DISPLAY_PTR, rba->m_displayObject);
			}
		}

		newAsset->m_rigidBodyAssets.pushBack(rba);
		rba->removeReference();
	}

	return newAsset;
}

hkPhysicsSystemDemoAsset::~hkPhysicsSystemDemoAsset()
{
	if (m_physicsSystem)
	{
		m_physicsSystem->removeReference();
	}
	for (int i=0; i < m_rigidBodyAssets.getSize(); ++i)
	{
		if (m_rigidBodyAssets[i])
		{
			m_rigidBodyAssets[i]->removeReference();
		}
	}
}


/// Convert from rb binding to runtime hkRigidBodyDemoAsset
static hkRigidBodyDemoAsset* HK_CALL _convertToRuntimeAsset( const hkpRigidBodyDisplayBinding* asset, hkgDisplayContext* graphicsContext )
{
	hkRigidBodyDemoAsset* newAsset = new hkRigidBodyDemoAsset;
	newAsset->m_rigidBody = asset->m_rigidBody;
	newAsset->m_displayObject = HK_NULL;

	// Need a display object to convert
	if (asset->m_displayObject)
	{
		//XXX Materials not shared as we don't persist the cache
		hkgAssetConverter::MaterialCache mcache;
		hkgArray<hkgAssetConverter::Mapping> vcache;
		hkgAssetConverter::MeshConvertInput input;

		input.m_context = graphicsContext;
		input.m_skinVertexBufferCache = &vcache;
		input.m_materialCache = &mcache;
		input.m_meshCache = HK_NULL;
		hkgAssetConverter::MeshConvertOutput output;

		const hkTransform& worldTransform = asset->m_rigidBody->getTransform();
		input.m_worldTransform.set( worldTransform );

		// these materials will be deleted by the cache so we don't need to track them
		newAsset->m_displayObject = hkgAssetConverter::convertMesh(asset->m_displayObject,input, output);

		// as we currently load an asset at a time, there is no instancing / sharing of geoms.
		// so we can bake the offset trans into the display version.
		if (! asset->m_rigidBodyFromDisplayObjectTransform.isApproximatelyEqual(hkMatrix4::getIdentity(), 0.001f) )
		{
			const float* t = reinterpret_cast<const float*>( &(asset->m_rigidBodyFromDisplayObjectTransform) );
			for (int gi = 0; gi < newAsset->m_displayObject->getNumGeometry(); ++gi)
			{
				newAsset->m_displayObject->getGeometry(gi)->transform(t);
			}
		}

		if (newAsset->m_displayObject) // then set the property in the rb
		{
			// Unlock the loaded properties - they will be deallocated by newAsset's destructor
			newAsset->m_rigidBody->unlockPropertiesFromLoadedObject();
			newAsset->m_rigidBody->addProperty(HK_PROPERTY_DISPLAY_PTR, hkpPropertyValue((void*)newAsset->m_displayObject) );
			newAsset->m_displayObject->setUserPointer( hkgDisplayObject::USER_POINTER_RIGID_BODY, asset->m_rigidBody); // so we can pick it
		}
	}

	return newAsset;
}


/// Load all rigid bodies with graphics bindings
int HK_CALL hkPhysicsAssetLoader::loadAllRigidBodyDemoAssets(const char* assetPath, hkLoader* loader, hkgDisplayContext* graphicsContext, hkArray<hkRigidBodyDemoAsset*>& assets )
{
	// File must have hkRootLevelContainer as the top level container
	hkRootLevelContainer* data = loader->load(assetPath);
	if( data )
	{
		// Assets are stored in a hkpDisplayBindingData variant
		hkpDisplayBindingData* bindingData = static_cast<hkpDisplayBindingData*>( data->findObjectByType( hkpDisplayBindingDataClass.getName() ) );
		if ( bindingData )
		{
			const hkArray<hkpRigidBodyDisplayBinding*>& rbbs = bindingData->m_rigidBodyBindings;
			for(int i=0; i<rbbs.getSize(); ++i)
			{
				hkRigidBodyDemoAsset* asset = _convertToRuntimeAsset( rbbs[i], graphicsContext );
				if(asset)
				{
					assets.pushBack(asset);
				}
			}
		}
	}

	return assets.getSize();
}

/// Load a single rigid body with a graphics binding
hkRigidBodyDemoAsset* HK_CALL hkPhysicsAssetLoader::loadRigidBodyDemoAsset(const char* assetPath, const char* assetName, hkLoader* loader, hkgDisplayContext* graphicsContext )
{
	hkRigidBodyDemoAsset* demoAsset = HK_NULL;

	// File must have hkRootLevelContainer as the top level container
	hkRootLevelContainer* data = loader->load(assetPath);
	if( data )
	{
		// Assets are stored in a hkpDisplayBindingData variant
		hkpDisplayBindingData* bindingData = static_cast<hkpDisplayBindingData*>( data->findObjectByType( hkpDisplayBindingDataClass.getName() ) );
		if ( bindingData )
		{
			const hkArray<hkpRigidBodyDisplayBinding*>& rbbs = bindingData->m_rigidBodyBindings;
			for(int i=0; i<rbbs.getSize(); ++i)
			{
				if( hkString::strCasecmp( assetName, rbbs[i]->m_rigidBody->getName() ) == 0 )
				{
					demoAsset = _convertToRuntimeAsset( rbbs[i], graphicsContext );
					break;
				}
			}
		}
	}

	// returns the ref
	return demoAsset;
}

/// Load a complex system with graphics bindings
hkPhysicsSystemDemoAsset* HK_CALL hkPhysicsAssetLoader::loadPhysicsSystemDemoAsset(const char* assetPath, hkLoader* loader, hkgDisplayContext* graphicsContext)
{
	hkPhysicsSystemDemoAsset* demoAsset = HK_NULL;

	// File must have hkRootLevelContainer as the top level container
	hkRootLevelContainer* data = loader->load(assetPath);
	if( data )
	{
		// Assets are stored in a hkpDisplayBindingData variant
		hkpDisplayBindingData* bindingData = static_cast<hkpDisplayBindingData*>( data->findObjectByType( hkpDisplayBindingDataClass.getName() ) );
		if ( bindingData && bindingData->m_physicsSystemBindings.getSize()>0 )
		{
			// Create rb assets from the first system found
			hkArray< hkRigidBodyDemoAsset* > rbAssets;
			hkpPhysicsSystemDisplayBinding* asset = bindingData->m_physicsSystemBindings[0];
			for (int i=0; i < asset->m_bindings.getSize(); ++i)
			{
				hkRigidBodyDemoAsset* rb = _convertToRuntimeAsset( asset->m_bindings[i], graphicsContext );
				if (rb)
				{
					rbAssets.pushBack(rb);
				}
			}

			// Only make one if we have something to put in it
			if (asset->m_system || (rbAssets.getSize() > 0) )
			{
				demoAsset = new hkPhysicsSystemDemoAsset;
				demoAsset->m_physicsSystem = asset->m_system;
				demoAsset->m_rigidBodyAssets = rbAssets; // passes on the held reference
			}
		}
	}

	// returns the ref
	return demoAsset;
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
