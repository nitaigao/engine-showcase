/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Common/Api/Serialize/SerializeWithoutMetadata/SerializeWithoutMetadata.h>

// Serialize includes
#include <Common/Base/System/Io/IStream/hkIStream.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileReader.h>
#include <Common/Serialize/Packfile/Binary/hkBinaryPackfileWriter.h>
#include <Common/Serialize/Packfile/Xml/hkXmlPackfileReader.h>
#include <Common/Serialize/Packfile/Xml/hkXmlPackfileWriter.h>

#include <Physics/Utilities/Serialize/hkpPhysicsData.h>

SerializeWithoutMetadata::SerializeWithoutMetadata( hkDemoEnvironment* env) 
	: hkDefaultPhysicsDemo(env),
	m_packfileData( HK_NULL )
{
	// Files to save to and load from.
	hkString fileNameSphereShape("Resources/Common/withoutMetaDataShapeShape");
	hkString fileNameRigidBody("Resources/Common/withoutMetaDataRigidBody");
	hkString fileNameWorld("Resources/Common/withoutMetaDataWorld");

	//
	// File extension based on demo variant (binary or xml).
	//
	hkString fileExtension;
	{
		if ( m_variantId == 0 )
		{
			fileExtension = ".hkx";
		}
		else
		{
			fileExtension = ".xml";
		}
	
		fileNameSphereShape += fileExtension;
		fileNameRigidBody += fileExtension;
		fileNameWorld += fileExtension;
	}

	//
	// Set up the camera.
	//
	{
		hkVector4 from( 10.0f, 10.0f, 10.0f );
		hkVector4 to( 0.0f, 0.0f, 0.0f );
		hkVector4 up( 0.0f, 0.0f, 1.0f );
		setupDefaultCameras( env, from, to, up );
	}

	// Create a sphere shape.
	hkpSphereShape* sphereShape = new hkpSphereShape( 1.0f );

	// Save sphere shape without metadata to file.
	saveWithoutMetadata( sphereShape, hkpSphereShapeClass, &fileNameSphereShape );

	// No longer need created shape (as saved version can be re-loaded) so delete original.
	{
		sphereShape->removeReference();
		sphereShape = HK_NULL;
	}

	//
	// Load sphere shape from file.
	//
	{
		sphereShape = static_cast<hkpSphereShape*>( loadWithoutMetadata( hkpSphereShapeClass, &fileNameSphereShape ) );
		HK_ASSERT2(0xa6451543, sphereShape != HK_NULL, "Could not load root level object (sphere shape)." );
	}

	//
	// Create a rigid body using loaded shape.
	//
	hkpRigidBody* rigidBody = HK_NULL;
	{
		hkpRigidBodyCinfo rigidBodyCinfo;
		rigidBodyCinfo.m_shape = sphereShape;
		rigidBodyCinfo.m_motionType = hkpMotion::MOTION_FIXED;
		rigidBody = new hkpRigidBody( rigidBodyCinfo );
	}

	// Save rigid body without metadata.
	saveWithoutMetadata( rigidBody, hkpRigidBodyClass, &fileNameRigidBody );

	//
	// No longer need created rigid body (as saved version can be re-loaded) so delete original.
	//
	{
		rigidBody->removeReference();
		rigidBody = HK_NULL;
		
		sphereShape = HK_NULL;
	}

	//
	// Load saved rigid body.
	//
	{
		rigidBody = static_cast<hkpRigidBody*>( loadWithoutMetadata( hkpRigidBodyClass, &fileNameRigidBody ) );
		HK_ASSERT2(0xa6451543, rigidBody != HK_NULL, "Could not load root level object (rigid body)." );
	}

	//
	// Create hkpWorld.
	//
	{
		hkpWorldCinfo info;
		m_world = new hkpWorld( info );
 		m_world->lock();
	}

	// Add rigid body to hkpWorld.
	m_world->addEntity( rigidBody );

	//
	// Save hkpWorld without metadata - since hkpWorld cannot be serialized directly, save the hkpPhysicsData "equivalent".
	//
	{
		hkpPhysicsData physicsDataToSave;
		physicsDataToSave.populateFromWorld( m_world );

		saveWithoutMetadata( &physicsDataToSave, hkpPhysicsDataClass, &fileNameWorld );
	}

	//
	// No longer need created world (as saved version can be re-loaded) so delete original.
	//
	{
		m_world->unlock();
		m_world->markForWrite();
		m_world->removeReference();
		m_world = HK_NULL;
	}

	//
	// Load saved hkpPhysicsData.
	//
	{
		hkpPhysicsData* physicsDataToLoad;

		physicsDataToLoad = static_cast<hkpPhysicsData*>( loadWithoutMetadata( hkpPhysicsDataClass, &fileNameWorld ) );
		HK_ASSERT2(0xa6451543, physicsDataToLoad != HK_NULL, "Could not load root level object (physics data)." );

		m_world = physicsDataToLoad->createWorld();
	}

	// Set up graphics (so the loaded rigid body can be seen in the world).
	setupGraphics();
}

SerializeWithoutMetadata::~SerializeWithoutMetadata()
{
	if ( m_world != HK_NULL )
	{
		m_world->markForWrite();
		m_world->removeReference();
		m_world = HK_NULL;
	}

	if ( m_packfileData != HK_NULL )
	{
		m_packfileData->removeReference();
		m_packfileData = HK_NULL;
	}
}

hkResult SerializeWithoutMetadata::saveWithoutMetadata( void* objectToWrite, const hkClass& classForWriteObject, hkString* fileName )
{
	hkOstream outFile( fileName->cString() );
	HK_ASSERT( 0x215d080c, outFile.isOk() );

	//
	// Packfile writer determined based on demo variant (binary or xml).
	//
	hkPackfileWriter* packfileWriter = HK_NULL;

	if ( m_variantId == 0 )
	{
		packfileWriter = new hkBinaryPackfileWriter;
	}
	else
	{
		packfileWriter = new hkXmlPackfileWriter;
	}

	hkStreamWriter* streamWriter = outFile.getStreamWriter();

	hkPackfileWriter::Options packfileWriterOptions;
	// Do not write metadata.
	packfileWriterOptions.m_writeMetaInfo = false;

	packfileWriter->setContents( objectToWrite, classForWriteObject );
	hkResult writeResult = packfileWriter->save( streamWriter, packfileWriterOptions );
	packfileWriter->removeReference();

	return writeResult;
}

void* SerializeWithoutMetadata::loadWithoutMetadata( const hkClass& classForReadObject, hkString* fileName )
{
	hkIstream infile( fileName->cString() );
	HK_ASSERT( 0x215d080c, infile.isOk() );

	//
	// Packfile reader based on demo variant (binary or xml).
	//
	hkPackfileReader* packfileReader = HK_NULL;

	if ( m_variantId == 0 )
	{
		packfileReader = new hkBinaryPackfileReader();
	}
	else
	{
		packfileReader = new hkXmlPackfileReader;
	}

	packfileReader->loadEntireFile( infile.getStreamReader() );

	// Scrub the old loaded data as it is no longer needed.
	if( m_packfileData != HK_NULL )
	{	
		m_packfileData->removeReference();
		m_packfileData = HK_NULL;
	}

	//
	// Get a handle to the memory allocated by the reader, and add a reference to it.
	// This allows us to delete the reader.
	//
	{
		m_packfileData = packfileReader->getPackfileData();
		m_packfileData->addReference();
	}

	// Get the top level object in the file
	void* readObject = packfileReader->getContents( classForReadObject.getName() );
	packfileReader->removeReference();

	return readObject;
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = \
"How to save and load simple objects without metadata.";

HK_DECLARE_DEMO_VARIANT( SerializeWithoutMetadata, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_SERIALIZE | HK_DEMO_TYPE_CRITICAL, "Binary", 0, helpString, helpString );
HK_DECLARE_DEMO_VARIANT( SerializeWithoutMetadata, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_SERIALIZE, "XML", 1, helpString, helpString );

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
