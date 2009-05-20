/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/ShowCase/Gdc2005/Gdc2005Demo.h>
#include <Demos/ShowCase/Gdc2005/Components/GdcProxyListener.h>

// Proxy
#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h>

#include <Physics/ConstraintSolver/Simplex/hkpSimplexSolver.h>

#include <Physics/Collide/Query/Collector/PointCollector/hkpAllCdPointCollector.h>
#include <Demos/DemoCommon/Utilities/GameUtils/GameUtils.h>
#include <Common/Visualize/hkDebugDisplay.h>

// HKG
#include <Graphics/Common/DisplayWorld/hkgDisplayWorld.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>
#include <Graphics/Bridge/DisplayHandler/hkgDisplayHandler.h>
#include <Common/Visualize/Shape/hkDisplayCapsule.h>

const hkVector4 UP(0,1,0);

GdcCharacterListener::GdcCharacterListener(const hkVector4& up, const Gdc2005DemoOptions* options)
	: m_up(up),
	m_onMovingSurface(false),
	m_options(options)
{}

// Don't push objects
void GdcCharacterListener::objectInteractionCallback(hkpCharacterProxy* proxy, const hkpCharacterObjectInteractionEvent& input, hkpCharacterObjectInteractionResult& output )
{
	output.m_objectImpulse.mul4( m_options->m_Proxy.m_strength );
}

// In this demo the motion of the character proxy is controlled explicitly by the animation system.
// The feedback between the character proxy and the animation system prevents the character proxy from moving
// unless the animation system has told it to. In this listener we explicitly override this by adding additional
// velocity for some moving obecjts (the crates in this demo). We add the lateral velocity of the obejct straight in to the controller.
void GdcCharacterListener::processConstraintsCallback( const hkArray<hkpRootCdPoint>& manifold, hkpSimplexSolverInput& input )
{
	// We go through the manifold and add velocity to the character proxy as required.
	for (int i=0; i < manifold.getSize(); i++)
	{
		hkpSurfaceConstraintInfo& surface = input.m_constraints[i];

		hkpRigidBody* body = hkGetRigidBody(manifold[i].m_rootCollidableB);
		bool movingBody = (body && (body->getMotionType() != hkpMotion::MOTION_FIXED) && (body->getMotionType() != hkpMotion::MOTION_KEYFRAMED));

		if (movingBody)
		{
			hkVector4 surfaceVel;
			surfaceVel = body->getLinearVelocity();

			// Work out surface velocity in plane perpendicular to up
			surfaceVel.addMul4( -surfaceVel.dot3(m_up), m_up );

			m_onMovingSurface = surface.m_plane.dot3(m_up) > 0.95f;
			if (m_onMovingSurface)
			{
				// Add the velocity to the character directly
				input.m_velocity.addMul4(m_options->m_Proxy.m_friction, surfaceVel);
			}
		}
		else
		{
			m_onMovingSurface = false;
		}

	}
}


void Gdc2005Demo::initCharacterController()
{

	//
	//	Create a character proxy object
	//
	hkpCapsuleShape* characterShape;
	{
		// Construct a shape

		const hkReal totalHeight = m_options.m_Proxy.m_height;
		const hkReal radius = m_options.m_Proxy.m_radius;
		const hkReal capsulePoint = totalHeight*0.5f - radius;

		hkVector4 vertexA(0, capsulePoint * 2 + radius, 0);
		hkVector4 vertexB(0, radius, 0);

		// Create a capsule to represent the character standing
		characterShape = new hkpCapsuleShape(vertexA, vertexB, radius);

		// Construct a Shape Phantom
		hkpSimpleShapePhantom* phantom = new hkpSimpleShapePhantom( characterShape, hkTransform::getIdentity(), hkpGroupFilter::calcFilterInfo (LAYER_PROXY,0) );
		characterShape->removeReference();

		// Add the phantom to the world
		m_world->addPhantom( phantom );

		// Construct a character proxy
		hkpCharacterProxyCinfo cpci;
		cpci.m_position.set(10, 10, 5);
		cpci.m_staticFriction = 0.0f;
		cpci.m_dynamicFriction = 1.0f;
		cpci.m_up.setNeg4( m_world->getGravity() );
		cpci.m_up.normalize3();
		cpci.m_userPlanes = 4;
		cpci.m_maxSlope = HK_REAL_PI / 3;

		cpci.m_shapePhantom = phantom;
		m_characterProxy = new hkpCharacterProxy( cpci );
		phantom->removeReference();
	}

	//
	// Add in a custom listener
	//
	{
		m_proxyListener = new GdcCharacterListener( UP, &m_options);
		m_characterProxy->addCharacterProxyListener(m_proxyListener);
	}

	// Grab the display
	{
		hkDisplayCapsule* displayCapsule = new hkDisplayCapsule( characterShape->getVertex(1), characterShape->getVertex(0), characterShape->getRadius() );

		hkArray<hkDisplayGeometry*> geoms; geoms.pushBack(displayCapsule);
		hkUlong id = (hkUlong)(m_characterProxy->getShapePhantom()->getCollidable());

		m_env->m_displayHandler->addGeometry( geoms, hkTransform::getIdentity(), id, 0, 0);
		displayCapsule->removeReference();

		HK_SET_OBJECT_COLOR(id, hkColor::rgbFromChars( 240, 200, 0, 70));

		m_proxyDisplay = m_env->m_displayHandler->findDisplayObject( id );
		int idx = m_env->m_displayWorld->findDisplayObject( m_proxyDisplay );
		if (idx>=0)
		{
			m_env->m_displayWorld->removeDisplayObject( idx );
		}
		m_proxyVisible = false;
	}

	m_timeUnsupported = 0.0f;
}

void Gdc2005Demo::updateProxyDisplay(hkReal radius, hkReal height)
{
	const hkpCapsuleShape* capsule = (const hkpCapsuleShape*)m_characterProxy->getShapePhantom()->getCollidable()->getShape();
	hkVector4 diff; diff.setSub4( capsule->getVertex(0), capsule->getVertex(1) );
	hkReal currentHeight = hkReal(diff.length3()) + capsule->getRadius() * 2.0f;

	if ((hkMath::fabs(currentHeight - height) < 1e-3f) && (hkMath::fabs(capsule->getRadius() - radius) < 1e-3f))
		return;

	{
		int idx = m_env->m_displayWorld->findDisplayObject(m_proxyDisplay);
		if (idx >=0 )
		{
			m_env->m_displayWorld->removeDisplayObject(idx);
			m_proxyDisplay->release(); // the world ref
		}
	}

	const hkReal capsulePoint = height*0.5f - radius;

	hkVector4 vertexA(0, capsulePoint * 2 + radius, 0);
	hkVector4 vertexB(0, radius, 0);

	hkDisplayCapsule* displayCapsule = new hkDisplayCapsule( vertexA, vertexB, radius );

	hkArray<hkDisplayGeometry*> geoms; geoms.pushBack(displayCapsule);
	hkUlong id = (hkUlong)(m_characterProxy->getShapePhantom()->getCollidable());

	m_env->m_displayHandler->removeGeometry( id ,0, 0);
	m_env->m_displayHandler->addGeometry( geoms, hkTransform::getIdentity(), id, 0, 0);
	displayCapsule->removeReference();

	HK_SET_OBJECT_COLOR(id, hkColor::rgbFromChars( 240, 200, 0, 70));

	m_proxyDisplay = m_env->m_displayHandler->findDisplayObject( id );
	if (!m_proxyVisible)
	{
		int idx = m_env->m_displayWorld->findDisplayObject( m_proxyDisplay );
		if (idx>=0)
		{
			m_env->m_displayWorld->removeDisplayObject( idx );
		}
	}
}

void Gdc2005Demo::toggleProxyVisibility()
{
	m_proxyVisible = !m_proxyVisible;
	if (m_proxyVisible)
	{
		m_env->m_displayWorld->addDisplayObject( m_proxyDisplay );
	}
	else
	{
		int idx = m_env->m_displayWorld->findDisplayObject(m_proxyDisplay);
		if (idx >=0 )
		{
			m_env->m_displayWorld->removeDisplayObject(idx);
			m_proxyDisplay->release(); // the world ref
		}
	}
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
