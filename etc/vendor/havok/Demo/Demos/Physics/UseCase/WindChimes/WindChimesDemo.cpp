/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/UseCase/WindChimes/WindChimesDemo.h>

#include <Physics/Collide/Shape/Convex/Cylinder/hkpCylinderShape.h>
#include <Physics/Dynamics/Constraint/Bilateral/Hinge/hkpHingeConstraintData.h>

#include <Physics/Utilities/Collide/Filter/constraint/hkpConstraintCollisionFilter.h>

#include <Common/Visualize/hkDebugDisplay.h>

#include <Common/Serialize/Util/hkLoader.h>
#include <Demos/DemoCommon/Utilities/Asset/hkAssetManagementUtil.h>
#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>

struct WindChimesVariant
{
	const char*	m_name;
	int m_numChimes;
	int m_numDifferentChimeSizes;
	const char* m_assetName;
	const char* m_details;
};

static const WindChimesVariant g_variants[] =
{
	{ "Circular", 5, 5, "Resources/Physics/Objects/WindChime.hkx", "Wind chimes in a circle" },
	{ "Linear", 8, 4, "Resources/Physics/Objects/WindChime2.hkx", "Wind chimes in a row" }
};

static const char* noteNames[] = {"Do","Re","Mi","Fa","So","La","Ti","Do"};

WindChimesDemo::WindChimesDemo(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env),
	m_loader(HK_NULL)
{
	const WindChimesVariant& variant =  g_variants[m_variantId];

	//
	// Create the sound manager.
	//

	//
	// Set up the camera
	//
	{
		hkVector4 from(6.0f, 0.0f, 3.0f);
		hkVector4 to  (0.0f, 0.0f, 0.0f);
		hkVector4 up  (0.0f, 0.0f, 1.0f);
		setupDefaultCameras( env, from, to, up );
	}

	hkString assetFile = hkAssetManagementUtil::getFilePath(variant.m_assetName);
	m_loader = new hkLoader();
	hkRootLevelContainer* container = m_loader->load( assetFile.cString() );
	HK_ASSERT2(0xaefe9356, container != HK_NULL , "Could not load asset");

	hkpPhysicsData* physics = static_cast<hkpPhysicsData*>( container->findObjectByType( hkpPhysicsDataClass.getName() ) );
	HK_ASSERT2(0x245982ae, physics != HK_NULL, "Could not find physics data in root level object" );
	{
		m_world = new hkpWorld( *physics->getWorldCinfo() );

		m_world->lock();

		hkpAgentRegisterUtil::registerAllAgents(m_world->getCollisionDispatcher());

		setupGraphics();
	}

	m_world->addPhysicsSystem( physics->getPhysicsSystems()[0] );

	// There are 5 pipes in the circular chimes asset, called Chime0, Chime1...etc.
	for( int i = 0; i < variant.m_numChimes; i++ )
	{
		hkString chimeName;
		chimeName.printf( "Chime%d", i );
		hkpRigidBody* chime = physics->findRigidBodyByName( chimeName.cString() );
		HK_ASSERT(0x99862353, chime);

		new WindChimesCollisionListener( chime, i % variant.m_numDifferentChimeSizes );
	}
	m_world->unlock();
}

WindChimesDemo::~WindChimesDemo()
{
	if(m_world != HK_NULL)
	{
		m_world->markForWrite();
		m_world->removeReference();
		m_world = HK_NULL;
	}
	if( m_loader != HK_NULL )
	{
		delete m_loader;
	}
}

// Our WindChimesCollisionListener class inherits from both hkpCollisionListener and hkpEntityListener and so implementations for each
// of the virtual methods in both base classes must be present. However, for the purposes of this demo we are only interested
// in the contactPointAddedCallback(...) method. It is within this that we are able to use the newly acquired contact information.
WindChimesCollisionListener::WindChimesCollisionListener(hkpRigidBody* rigidBody, int noteIndex)
:   m_noteIndex(noteIndex)
{
	rigidBody->addCollisionListener( this );
	rigidBody->addEntityListener( this );
	m_uniqueIdCounter = 0;
}

WindChimesCollisionListener::~WindChimesCollisionListener()
{
}

void WindChimesCollisionListener::contactPointAddedCallback( hkpContactPointAddedEvent& event )
{
	hkReal noteStrength = hkMath::fabs(event.m_projectedVelocity);

	hkString bodyA = static_cast<hkpRigidBody*>(event.m_bodyA->getRootCollidable()->getOwner())->getName();
	hkString bodyB = static_cast<hkpRigidBody*>(event.m_bodyB->getRootCollidable()->getOwner())->getName();

	if( hkString::strCmp(bodyA.cString(),"Gong") == 0 || hkString::strCmp(bodyB.cString(),"Gong") == 0 )
	{
		noteStrength *= 3;
	}

	hkprintf( "%s %f (%s, %s)\n", noteNames[m_noteIndex], noteStrength, bodyA.cString(), bodyB.cString() );

}

void WindChimesCollisionListener::contactPointRemovedCallback( hkpContactPointRemovedEvent& event )
{

}

void WindChimesCollisionListener::contactPointConfirmedCallback( hkpContactPointConfirmedEvent& event )
{

}

void WindChimesCollisionListener::contactProcessCallback( hkpContactProcessEvent& event )
{

}

//
// entityDeletedCallback
//
void WindChimesCollisionListener::entityDeletedCallback( hkpEntity* entity )
{
	// Remove the collision event listener
	entity->removeCollisionListener( this ); 


	// Now that we're removed from our entity, we can remove ourselves, too.
	delete this;
}

void WindChimesCollisionListener::entityRemovedCallback( hkpEntity* entity )
{
	// Do nothing (we'll remove the collision event listener only when the entity is deleted)
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = "WindChimes";

HK_DECLARE_DEMO_VARIANT_USING_STRUCT( WindChimesDemo, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_SERIALIZE, WindChimesVariant, g_variants, helpString ); 



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
