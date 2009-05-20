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

// ik solver
#include <Animation/Animation/Ik/TwoJoints/hkaTwoJointsIkSolver.h>

// for the raycast interface
#include <Animation/Animation/Ik/FootPlacement/hkaFootPlacementIkSolver.h>

#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>
#include <Animation/Animation/Rig/hkaPose.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpAllCdPointCollector.h>
#include <Physics/Collide/Agent/hkpProcessCollisionInput.h>

/*
** REACH Component
**
** The reach component in this demo looks for objects in close proximity by using Havok Physics's calls to getClosestPoints(), as well
** as a raycast to ensure the object is a surface we can land our hands on.
** The TwoJoints IK Solver is used to place the hand on the spot found.
** This component keeps internal state and can be eased in and out. Apart from smoothing out the on/off transition, it also smooths
** movements in the target to avoid flickering artifacts.
**
*/

// initReach() just stores the indices of the different bones involved in each arms reach, as well
// as the location of the "radar points" (around which we detect location to land the hands).
// It also initializes internal state (weights and normals)
void Gdc2005Demo::initReach ()
{
	const hkaSkeleton& skeleton = *m_animatedSkeleton->getSkeleton();

	// Left arm
	m_shoulderIdx [LEFT_ARM] = hkaSkeletonUtils::findBoneWithName( skeleton, "LeftShoulder" );
	m_elbowIdx [LEFT_ARM] = hkaSkeletonUtils::findBoneWithName(skeleton, "LeftElbow");
	m_handIdx [LEFT_ARM] = hkaSkeletonUtils::findBoneWithName(skeleton, "LeftWrist");

	// Right arm
	m_shoulderIdx [RIGHT_ARM] = hkaSkeletonUtils::findBoneWithName( skeleton, "RightShoulder" );
	m_elbowIdx [RIGHT_ARM] = hkaSkeletonUtils::findBoneWithName(skeleton, "RightElbow");
	m_handIdx [RIGHT_ARM] = hkaSkeletonUtils::findBoneWithName(skeleton, "RightWrist");

	// Internal state, used for smoothing
	m_reachStopped [LEFT_ARM] = true;
	m_reachStopped [RIGHT_ARM] = true;
	m_previousReachPointWS[LEFT_ARM].setZero4();
	m_previousReachPointWS[RIGHT_ARM].setZero4();
	m_previousNormalWS[LEFT_ARM].set(0,1,0);
	m_previousNormalWS[RIGHT_ARM].set(0,1,0);
	m_reachWeight[LEFT_ARM]=0.0f;
	m_reachWeight[RIGHT_ARM]=0.0f;

	// We will move the two sensors according to the forward direction, defined by the rotation of this bone
	m_reachReferenceBoneIdx = hkaSkeletonUtils::findBoneWithName(skeleton, "root");

	// Location of the sensors relative to the orientation of the reference bone
	m_radarLocationRS[LEFT_ARM] = hkVector4(0.4f,1.5f,0.20f);
	m_radarLocationRS[RIGHT_ARM] = hkVector4(-0.4f,1.5f,0.20f);

	// The axis around which the elbow rotate. This axis is specific to the model setup. A positive rotation around that axis should extend the arm
	m_elbowAxis[LEFT_ARM] = hkVector4(0,1,0);
	m_elbowAxis[RIGHT_ARM] = hkVector4(0,-1,0);

}

// doReach() is always called, with reachOn=true when we want to do reaching, reachOn=false otherwise
// This is because is internally handles the easing in and out. It is called once for each arm
void Gdc2005Demo::doReach (hkBool reachOn, WhichArm whichArm, hkaPose& thePose)
{

	const hkVector4 upWS(0,1,0);

	const hkInt16 shoulderIdx = m_shoulderIdx[whichArm];
	const hkInt16 elbowIdx = m_elbowIdx[whichArm];
	const hkInt16 handIdx = m_handIdx[whichArm];

	hkQsTransform modelFromReference = thePose.getBoneModelSpace(m_reachReferenceBoneIdx);
	// Remove all local translations from the reference bone, use only its orientation
	modelFromReference.m_translation.set(0.0f, 0.0f, 0.0f);
	hkQsTransform worldFromReference; worldFromReference.setMul(m_currentTransform, modelFromReference);

	// Look for a place to reach
	hkBool found = false;
	hkVector4 pointWS;
	hkVector4 normalWS;

	// Search for closest point, unless we are switching off reaching
	if (reachOn)
	{
		// The position of our sensor:
		hkVector4 radarPosWS; radarPosWS.setTransformedPos(worldFromReference, m_radarLocationRS[whichArm]);

		// Display it
		if (m_options.m_Display.m_showIkInfo)
		{
			HK_DISPLAY_STAR( radarPosWS, 0.1f, 0xffffffff);
		}

		// Use hkpWorld::getClosestPoints() to look for candidates
		// Use a sphere of 0.05m radius located at the radar point
		hkTransform sphereTransform; sphereTransform.setIdentity();
		sphereTransform.setTranslation(radarPosWS);
		hkpSphereShape sphere(0.05f);

		// Use the ray cast filter layer, so it only landscape and movable environment is detected
		hkpCollidable collidable( &sphere, &sphereTransform );
		collidable.setCollisionFilterInfo(hkpGroupFilter::calcFilterInfo(LAYER_RAYCAST, 0));

		// Collect all possible candidates
		// Use the same filter we use for the world (it already knows which layers collide with which layers)
		hkpAllCdPointCollector collector;
		hkpProcessCollisionInput input =*m_world->getCollisionInput();
		input.m_tolerance = 0.4f;
		input.m_filter = m_world->getCollisionFilter();
		m_world->getClosestPoints( &collidable, input, collector);

		// Sort all the candidates (closest first) so the first good one is the best one
		collector.sortHits();
		hkArray<hkpRootCdPoint>& allHits = collector.getHits();

		// Now, we have a list of closest points (closest objects). Ray cast against them to find out if they
		// have a surface we can lay the hand on.
		for (int hitId = 0; hitId < allHits.getSize(); hitId++)
		{
			hkContactPoint& contactPoint = allHits[hitId].m_contact;

			hkVector4 closest = contactPoint.getPosition();

			// Construct a unit vector that points from the radar towards the detected point in WS
			// This is the "away" vector
			hkVector4 awayWS;
			{
				awayWS.setSub4(closest, radarPosWS);
				awayWS.addMul4(-awayWS.dot3(upWS), upWS);
				const hkReal length = awayWS.length3();
				if (length>1e-5f)
				{
					awayWS.mul4(1.0f/length);
				}
				else
				{
					const hkVector4 forwardMS(0.0f,0.0f,1.0f);
					awayWS.setRotatedDir(m_currentTransform.getRotation(), forwardMS);
				}
			}

			// The closest point is usually at the edge. Move it a little away (10cm)
			closest.addMul4(0.10f, awayWS);

			// Do a raycast above it. We reuse the raycast interface object that we use for the foot placement
			const hkReal upDistance = 0.3f;
			const hkReal downDistance = 0.3f;
			const hkReal totalLength = upDistance + downDistance;
			hkVector4 startWS; startWS.setAddMul4(closest,upWS, upDistance);
			hkVector4 endWS; endWS.setAddMul4(closest,upWS, -downDistance);
			hkReal hitFraction;
			const hkBool properHit = m_raycastInterface->castRay(startWS, endWS, hitFraction, normalWS);

			if (m_options.m_Display.m_showIkInfo)
			{
				HK_DISPLAY_LINE( startWS, endWS, 0x00f8f8f8 );
			}

			// Nothing found, check the next candidate
			if (!properHit)
			{
				continue;
			}

			// The surface is not (roughly) horizontal, check the next candidate
			if (normalWS.dot3(upWS)<0.8f)
			{
				continue;
			}

			// Good, we have a good candidate. Check where the landing point is
			pointWS.setAddMul4(startWS, upWS, -totalLength * hitFraction);

			// If we are dealing with a box (the big crates), move the point closer since to account for the radius
			{
				const bool isABox = allHits[hitId].m_rootCollidableB->getShape()->getType() == HK_SHAPE_CONVEX_TRANSLATE;

				// Box : move it back closer
				if (isABox)
				{
					pointWS.addMul4(-0.1f, awayWS);
				}
			}

			// Finally, reject points too far away (the arm won't reach)
			hkVector4 vDistance;vDistance.setSub4(pointWS, radarPosWS);
			const hkReal distanceSqr = vDistance.lengthSquared3();
			if (distanceSqr > 0.16f)
			{
				continue;
			}

			// If we reached here, we have a found a place where to lay our hand, don't look for more candidates
			found = true;
			break;

		}

		// Did we find a place to put the hand on?
		if (found)
		{
			if (m_options.m_Display.m_showIkInfo)
			{
				HK_DISPLAY_STAR(pointWS, 0.5f, 0xffffff00);
			}

			// Move the point a little up to account for the width of the hand
			pointWS.addMul4(0.03f, upWS);

			// Interpolate the point and the normal with the previous ones, unless it's the first time of course
			if (m_reachWeight[whichArm]>0.0f)
			{
				// Use the gain specified by the user
				const hkReal moveGain = m_options.m_IK.m_handIkMoveGain;
				pointWS.setInterpolate4(m_previousReachPointWS[whichArm], pointWS, moveGain);
				normalWS.setInterpolate4(m_previousNormalWS[whichArm], normalWS, moveGain);
				normalWS.normalize3();
			}
			// Store them to interpolate the next frame
			m_previousReachPointWS[whichArm] = pointWS;
			m_previousNormalWS[whichArm] = normalWS;

		}
	}

	// Do gain on the weight for the reaching
	if (found)
	{
		// If reach is on : go towards 1.0f (ease in), otherwise towards 0.0f (ease off)
		const hkReal desiredWeight = (reachOn) ? 1.0f : 0.0f;
		const hkReal diffWeight = desiredWeight - m_reachWeight[whichArm];
		const hkReal reachGain = m_options.m_IK.m_handIkReachGain;
		m_reachWeight[whichArm] += diffWeight * reachGain;
	}
	else
	{
		// If we didn't find anything, go towards 0.0f (ease off)
		const hkReal leaveGain = m_options.m_IK.m_handIkLeaveGain;
		m_reachWeight[whichArm] *= (1.0f - leaveGain);
		pointWS = m_previousReachPointWS[whichArm];
		normalWS = m_previousNormalWS[whichArm];
	}

	// If weight 0 or very small, switch off completely and do nothing
	if (m_reachWeight[whichArm] < 0.01f)
	{
		m_reachWeight[whichArm] = 0.0f;
	}
	else
	{
		// Fix the position, use the Two Joints IK solver
		{
			hkVector4 pointMS; pointMS.setTransformedInversePos(m_currentTransform, pointWS);

			hkaTwoJointsIkSolver::Setup setup;

			setup.m_firstJointIdx =shoulderIdx;
			setup.m_secondJointIdx = elbowIdx;
			setup.m_endBoneIdx = handIdx;
			setup.m_hingeAxisLS = m_elbowAxis[whichArm];
			setup.m_firstJointIkGain = m_reachWeight[whichArm];
			setup.m_secondJointIkGain = m_reachWeight[whichArm];
			setup.m_endJointIkGain = m_reachWeight[whichArm];
			setup.m_endTargetMS = pointMS;

			hkaTwoJointsIkSolver::solve(setup, thePose);
		}

		// Now, rotate the wrist, so the palm follows the surface
		{
			const hkVector4 upHandLocalSpace (0,1,0); // In this rig the hand Y axis points up (when the palm faces down)
			const hkQuaternion currentHandRotation = thePose.getBoneModelSpace(handIdx).getRotation();
			hkVector4 currentUpMS; currentUpMS.setRotatedDir(currentHandRotation, upHandLocalSpace);
			hkVector4 desiredUpMS; desiredUpMS.setRotatedInverseDir(m_currentTransform.getRotation(), normalWS);

			{
				hkVector4 currentUpWS; currentUpWS.setRotatedDir(m_currentTransform.getRotation(), currentUpMS);
				hkVector4 desiredUpWS; desiredUpWS.setRotatedDir(m_currentTransform.getRotation(), desiredUpMS);

			}

			// Calculate the shortest rotation that matches both axis
			hkQuaternion extraRotation; extraRotation.setIdentity();
			{
				// Look for the shortest rotation that would bring the foot to the right orientation
				// We scale that rotation (its angle) by the weight of the reach IK
				const hkReal dotProd = currentUpMS.dot3(desiredUpMS);

				if ( (dotProd - 1.0f) < - HK_REAL_EPSILON)
				{
					const hkReal rotationAngle = hkMath::acos(dotProd);

					hkVector4 rotationAxis;
					rotationAxis.setCross(currentUpMS, desiredUpMS);

					if (rotationAxis.length3()>1e-14f)
					{
						rotationAxis.normalize3();
						extraRotation.setAxisAngle(rotationAxis, rotationAngle*m_reachWeight[whichArm]);
					}
				}
			}

			// Apply the extra rotation
			hkQuaternion newRotationMS; newRotationMS.setMul(extraRotation, currentHandRotation);
			newRotationMS.normalize();

			// Set the new rotation to the hand in the pose
			thePose.accessBoneModelSpace(handIdx, hkaPose::PROPAGATE).setRotation(newRotationMS);
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
