/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/ShowCase/Gdc2005/Gdc2005Demo.h>

// Debug graphics
#include <Common/Visualize/hkDebugDisplay.h>

#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>
#include <Animation/Animation/Rig/hkaPose.h>

#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Common/SceneData/Scene/hkxScene.h>
#include <Common/SceneData/Camera/hkxCamera.h>
#include <Graphics/Common/Window/hkgWindow.h>
#include <Graphics/Common/DisplayObject/hkgDisplayObject.h>

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCaster.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>
#include <Physics/Collide/Query/Collector/RayCollector/hkpClosestRayHitCollector.h>

/*
** CAMERA Component
*/

// doSkinning() just renders (skins) the given pose
void Gdc2005Demo::doCamera (const hkaPose& thePose)
{
	// update camera
	{
		hkVector4 pos;
		const hkInt16 headIdx = hkaSkeletonUtils::findBoneWithName( *m_animatedSkeleton->getSkeleton(), "root");
		pos.setTransformedPos( m_currentTransform, thePose.getBoneModelSpace(headIdx).m_translation );

		if (m_options.m_Display.m_cameraMode == hkGdcDisplayOptions::CLOSEST)
		{
			findCamera( pos );
			fixedCamera( pos );
		}
		else if (m_options.m_Display.m_cameraMode == hkGdcDisplayOptions::TRACKING)
		{
			otsCamera( pos );
		}

	}
}

void Gdc2005Demo::findCamera(const hkVector4& target)
{
	// Choose the closest camera that can see the player
	hkReal error = HK_REAL_MAX;

	int best = 0;

	for (int i=0; i < m_numCameras; i++)
	{
		hkxCamera* c = m_cameras[i];

		hkpWorldRayCastInput raycastIn;
		hkpClosestRayHitCollector rayCollector;

		raycastIn.m_from = c->m_from;
		raycastIn.m_to = m_currentTransform.m_translation;

		hkVector4 cameraDir;
		cameraDir.setSub4( c->m_focus, c->m_from );
		cameraDir.fastNormalize3();

		hkVector4 targetDir;
		targetDir.setSub4( target, c->m_from );
		targetDir.fastNormalize3();

		hkReal cosAng = targetDir.dot3( cameraDir );

		// Check that the camera has the right fov.
		if ( hkMath::cos(c->m_fov) > cosAng)
			continue;

		// cast the ray into the landscape
		m_world->castRay( raycastIn, rayCollector );

		const hkBool didHit = rayCollector.hasHit();
		const hkpWorldRayCastOutput& raycastOut = rayCollector.getHit();


		if (!didHit || (raycastOut.m_hitFraction > 0.85f))
		{
			targetDir.setSub4( target, c->m_focus );

			if (targetDir.lengthSquared3() < error)
			{
				error = targetDir.lengthSquared3();
				best = i;
			}
		}
	}


	if (m_currentCamera != best)
	{
		if (m_lastCamera == best)
		{
			m_haveBetterCamera += m_timestep;
			// Decide if we should switch to best
			if (m_haveBetterCamera > 0.5f)
			{
				m_haveBetterCamera = 0.0f;
				m_currentCamera = best;
			}
		}
		else
		{
			m_haveBetterCamera = 0.0f;
		}
	}

	m_lastCamera = best;
}

void Gdc2005Demo::fixedCamera(const hkVector4& target ) const
{
	hkxCamera& desiredCamera = *m_cameras[m_currentCamera];
	hkgCamera* currentCamera = m_env->m_window->getCurrentViewport()->getCamera();

	hkReal cameraGain = 0.02f;

	const hkVector4 worldUpWS(0,1,0);

	// Camera Handling
	{
		hkReal fov;
		hkVector4 from, to, up;
		currentCamera->getFrom((float*)&from);
		currentCamera->getTo((float*)&to);
		currentCamera->getUp((float*)&up);
		fov = currentCamera->getFOV();

		// Interpolate position
		from.setInterpolate4( from, desiredCamera.m_from, cameraGain);

		// Interpolate target
		to.setInterpolate4( to, target, cameraGain);

		// Interpolate uo
		up.setInterpolate4( up, desiredCamera.m_up, cameraGain);
		if (up.dot3(worldUpWS) < 0)
		{
			up.setNeg4( up );
		}

		// Interpolate fov
		fov += ((desiredCamera.m_fov * 180.0f / HK_REAL_PI) - fov) * cameraGain;

		hkgWindow* w = m_env->m_window;
		for(int i = 0; i < w->getNumViewports(); ++i)
		{
			hkgCamera* c = w->getViewport(i)->getCamera();

			// set up camera
			c->setFrom(&from(0));
			c->setTo(&to(0));
			c->setUp(&up(0));
			c->setFOV(fov);
			c->orthogonalize();
			c->computeModelView();
			c->computeProjection();
		}
	}
}

void Gdc2005Demo::otsCamera(const hkVector4& target) const
{
	hkVector4 dampedTarget;

	const hkVector4 worldUpWS(0,1,0);

	hkgWindow* w = m_env->m_window;
	for(int i = 0; i < w->getNumViewports(); ++i)
	{
		hkgCamera* c = w->getViewport(i)->getCamera();

		hkVector4 to;
		c->getTo((float*)&to);
		hkVector4 diff;
		diff.setSub4(target, to);
		to.addMul4(m_options.m_Display.m_cameraGain, diff);

		// set up camera
		c->setTo((const float*)&to);
		c->setUp((const float*)&worldUpWS);
		c->orthogonalize();
		c->computeModelView();
		c->computeProjection();
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
