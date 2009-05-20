/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/Physics/Api/Collide/RayCasting/RayCastAgainstProxy/RaycastAgainstProxyDemo.h>

#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Base/System/Io/IStream/hkIStream.h>

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCaster.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/Collector/RayCollector/hkpClosestRayHitCollector.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/Query/hkpRayHitCollector.h>
#include <Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h>

#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Dynamics/Phantom/hkpAabbPhantom.h>
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>

#include <Common/Serialize/Packfile/Xml/hkXmlPackfileReader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>
#include <Common/Serialize/Version/hkVersionRegistry.h>
#include <Common/Serialize/Version/hkVersionUtil.h>

#include <Physics/Utilities/Serialize/hkpPhysicsData.h>
#include <Physics/Utilities/VisualDebugger/Viewer/hkpShapeDisplayBuilder.h>

#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Common/Visualize/hkDebugDisplay.h>
#include <Common/Visualize/Shape/hkDisplayGeometry.h>

class hkProxyPhantom : public hkpAabbPhantom
{
protected:

	const hkpShape*	m_highResolutionShape;
	hkTransform		m_currentTransform;

	/// Update the position of the AABB in the broadphase from the current transform.
	void updateAabbFromCurrentTransform ( void)
	{
		hkAabb aabb;
		m_highResolutionShape->getAabb(m_currentTransform, 0.1f, aabb);
		setAabb(aabb);
	}

  public:

	  /// Override one of the user hkPhantomTypes. This is the value that will be returned
	  /// by hkProxyPhantom::getType().
	static const int HK_PHANTOM_PROXY = HK_PHANTOM_USER0;

	/// hkpShape* highResolutionShape - The high resolution shape for raycasting that is
	/// not in the world.
	/// hkTransform& transform - The initial transform of the shape in world-space.
	/// collisionFilterInfo - The collision filter info for this shape.
	hkProxyPhantom ( const hkpShape* highResolutionShape, hkTransform& transform, hkUint32 collisionFilterInfo = 0 )
		: hkpAabbPhantom(hkAabb(hkVector4::getZero(), hkVector4::getZero()), collisionFilterInfo)
		, m_highResolutionShape(highResolutionShape)
		, m_currentTransform(transform)
	{
		m_highResolutionShape->addReference();
		updateAabbFromCurrentTransform();
	}

	virtual ~hkProxyPhantom ( void )
	{
		m_highResolutionShape->removeReference();
	}

	/// Returns hkProxyPhantom::HK_PHANTOM_PROXY
	/// This allows our raycast function to identify phantoms of this type.
	virtual hkpPhantomType getType( void )  const
	{
		return hkpPhantomType(HK_PHANTOM_PROXY);
	}

	/// Get the current transform of the shape
	const hkTransform& getTransform ( void ) const
	{
		return m_currentTransform;
	}

	void castRayWithCollector( const hkpShapeRayCastInput& input, hkpRayHitCollector& collector ) const
	{
		// Create a null cdBody. If the shape was
		// in the world one would exist.
		hkMotionState* ms = HK_NULL;
		hkpCdBody* parent = HK_NULL;
		hkpCdBody body(parent, ms);

		// Transform the shape into the correct space
		body.setShape(m_highResolutionShape, HK_INVALID_SHAPE_KEY);
		body.setTransform(&getTransform());
		m_highResolutionShape->castRayWithCollector(input, body, collector);
	}

	/// Set the transform of the shape (and update the phantom).
	/// This will be called to update the position of the shape in the world.
	void setTransform ( const hkTransform& newTransform )
	{
		HK_TIME_CODE_BLOCK("hkProxyPhantom::setTransform", this);
		m_currentTransform = newTransform;
		updateAabbFromCurrentTransform();
	}

	const hkpShape* getShape( void ) const
	{
		return m_highResolutionShape;
	}

};


class hkProxyRayCaster : public hkpWorldRayCaster
{
  protected:
	virtual	hkReal addBroadPhaseHandle( const hkpBroadPhaseHandle* broadPhaseHandle, int castIndex )
	{
		HK_TIME_CODE_BLOCK("hkProxyRayCaster ::addBroadPhaseHandle", HK_NULL);

		const hkpCollidable* col = static_cast<hkpCollidable*>( static_cast<const hkpTypedBroadPhaseHandle*>(broadPhaseHandle)->getOwner() );		
		hkpRayHitCollector* collector = hkAddByteOffset( m_collectorBase, m_collectorStriding * castIndex );
		
		/// If the handle points to a proxy phantom, pass the raycast
		/// query on to the phantom.
		if (hkpPhantom* phantom = hkGetPhantom(col))
		{
			if (phantom->getType() == hkProxyPhantom::HK_PHANTOM_PROXY)
			{
				const hkProxyPhantom* pf = static_cast<hkProxyPhantom*>(phantom);
				const hkTransform& trans = pf->getTransform();
				const hkpWorldRayCastInput& input = m_input[castIndex];

				m_shapeInput.m_from._setTransformedInversePos( trans, m_input->m_from);
				m_shapeInput.m_to.  _setTransformedInversePos( trans, input.m_to);
				m_shapeInput.m_filterInfo = input.m_filterInfo;

				pf->castRayWithCollector(m_shapeInput, *collector);
			}

			return collector->m_earlyOutHitFraction;
		}
		else
		{
			/// Otherwise fall back on the default hkpWorldRayCaster behavior.
			return hkpWorldRayCaster::addBroadPhaseHandle(broadPhaseHandle, castIndex);
		}
	}
};


RaycastAgainstProxy::RaycastAgainstProxy(hkDemoEnvironment* env)
:	hkDefaultPhysicsDemo(env)
{
	//
	// Setup the camera.
	//
	{
		hkVector4 from( 7.0f, 6.0f, 17.0f);
		hkVector4 to  ( 1.6f, 1.2f,  9.3f);
		hkVector4 up  ( 0.0f, 1.0f,  0.0f);
		setupDefaultCameras(env, from, to, up);
	}

	//
	// Create the world.
	//
	{
		hkpWorldCinfo info;
		m_world = new hkpWorld(info);
		m_world->lock();
		setupGraphics();
	}

	m_time = 0;
	createBodies();
	m_world->unlock();

	m_angle = 0.0f;
}

RaycastAgainstProxy::~RaycastAgainstProxy()
{
	m_env->m_displayHandler->removeGeometry(m_displayID, 0, 0);

	hkReferencedObject::lockAll();
	m_world->markForWrite();
	m_world->removeReference();
	m_world = HK_NULL;

	hkReferencedObject::unlockAll();
	m_pf->removeReference();
	m_packFileData->removeReference();
}

hkDemo::Result RaycastAgainstProxy::stepDemo()
{
	m_world->lock();

	m_angle += 0.01f;

	hkVector4 axis (0.3f, 0.2f, 0.5f);
	axis.normalize3();

	hkTransform t;
	t.setIdentity();
	t.setRotation(hkQuaternion(axis, m_angle));
	m_pf->setTransform(t);

	// Update the display
	m_env->m_displayHandler->updateGeometry(m_pf->getTransform(),m_displayID,0);

	m_time += m_timestep;

	// The start point of the ray remains fixed in world space with the destination point of the
	// ray being varied in both the X and Y directions. This is achieved with simple
	// sine and cosine functions calls using the current time as the varying parameter:

	hkReal xDir = 6.0f * hkMath::sin(m_time * 0.3f);
	hkReal yDir = 6.0f * hkMath::cos(m_time * 0.3f);
	
	// The start and end points are both specified in World Space as we are using the world castRay() method
	// to fire the ray.

	hkpWorldRayCastInput input;
	input.m_from.set(1.0f, 1.0f, 15.0f);
	input.m_to.setAdd4(input.m_from, hkVector4(xDir, yDir, -30.0f));

	hkpClosestRayHitCollector output;

	hkProxyRayCaster caster;
	caster.castRay(*m_world->getBroadPhase(), input, m_world->getCollisionFilter(), HK_NULL, output);

	// To visualise the raycast we make use of a macro defined in "hkDebugDisplay.h" called HK_DISPLAY_LINE.
	// The macro takes three parameters: a start point, an end point and the line colour.
	// If a hit is found we display a YELLOW line from the raycast start point to the point of intersection and mark that
	// point with a small RED cross. The intersection point is calculated using: startWorld + (result.m_mindist * endWorld).
	//
	// If no hit is found we simply display a GREY line between the raycast start and end points.

	if( output.hasHit() )
	{
		hkVector4 intersectionPointWorld;
		intersectionPointWorld.setInterpolate4( input.m_from, input.m_to, output.getHit().m_hitFraction );
		HK_DISPLAY_LINE( input.m_from, intersectionPointWorld, hkColor::YELLOW);
		HK_DISPLAY_ARROW( intersectionPointWorld, output.getHit().m_normal, hkColor::CYAN);
	}
	else
	{
		// Otherwise draw as GREY
		HK_DISPLAY_LINE(input.m_from, input.m_to, hkColor::rgbFromChars(200, 200, 200));
	}

	m_world->unlock();

	return hkDefaultPhysicsDemo::stepDemo();
}



void RaycastAgainstProxy::createBodies()
{
	const hkpShape* shape;

	// Load the shape from a file
	{
		hkXmlPackfileReader reader;
		static const char filename [] = "Resources/Physics/Objects/Teapot.xml";
		hkIstream infile (filename);
		HK_ASSERT2(0x215d080c, infile.isOk(), "Could not open " << filename);
		reader.loadEntireFile(infile.getStreamReader());
		hkVersionUtil::updateToCurrentVersion( reader, hkVersionRegistry::getInstance() );

		m_packFileData = reader.getPackfileData();
		m_packFileData->addReference();

		hkRootLevelContainer* container = static_cast<hkRootLevelContainer*>( reader.getContents( hkRootLevelContainerClass.getName() ) );
		HK_ASSERT2(0xa6451543, container != HK_NULL, "Could not load root level obejct" );
		hkpPhysicsData* physicsData = static_cast<hkpPhysicsData*>( container->findObjectByType( hkpPhysicsDataClass.getName() ) );
		HK_ASSERT2(0xa6451544, physicsData, "Could not find physics data in root level object" );
		shape = physicsData->getPhysicsSystems()[0]->getRigidBodies()[0]->getCollidableRw()->getShape();
	}
	
	// Build the proxy phantom
	{
		hkTransform t; t.setIdentity();
		m_pf = new hkProxyPhantom(shape, t);
		m_world->addPhantom(m_pf );
	}

	// Add some display geometry so that we can see what we are casting against.
	{
		m_displayID = 1;
		hkArray<hkDisplayGeometry*> geom;
		hkpShapeDisplayBuilder::hkpShapeDisplayBuilderEnvironment env;
		hkpShapeDisplayBuilder builder(env);
		builder.buildShapeDisplay(m_pf->getShape(), m_pf->getTransform(), geom);
		m_env->m_displayHandler->addGeometry(geom, m_pf->getTransform(),m_displayID,0, 0);

		for (int i=0; i<geom.getSize(); i++)
		{
			delete geom[i];
		}
	}
}

#if defined(HK_COMPILER_MWERKS)
#	pragma force_active on
#	pragma fullpath_file on
#endif

static const char helpString[] = "This demo shows how to remove complicated shapes from "
"the broadphase, and replace them with an AABB phantom instead.";

HK_DECLARE_DEMO(RaycastAgainstProxy, HK_DEMO_TYPE_PRIME | HK_DEMO_TYPE_SERIALIZE, "", helpString);

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
