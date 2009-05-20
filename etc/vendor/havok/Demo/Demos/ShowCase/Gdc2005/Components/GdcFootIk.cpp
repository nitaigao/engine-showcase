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

#include <Animation/Animation/Ik/FootPlacement/hkaFootPlacementIkSolver.h>

#include <Physics/Collide/Shape/Convex/ConvexTranslate/hkpConvexTranslateShape.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCaster.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>

#include <Physics/Collide/Query/CastUtil/hkpLinearCastInput.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpSimpleClosestContactCollector.h>
#include <Physics/Collide/Query/Collector/RayCollector/hkpClosestRayHitCollector.h>

// State Machine
#include <Demos/ShowCase/Gdc2005/StateMachine/GdcState.h>
#include <Demos/ShowCase/Gdc2005/StateMachine/GdcStateTypes.h>


#include <Common/Base/Monitor/hkMonitorStream.h>

/*
** FOOT IK component
**
** It is mostly implemented inside the FootPlacement IK solver (hkFootPlacementIkSolveR).
** The code here just initializes and destroys the objects involved.
**
*/


// The Foot Placement IK solver requires an interface to perform raycast. We will provide one using Havok Physics.
class GdcDemoRaycastInterface : public hkReferencedObject, public hkaRaycastInterface
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DEMO );

		GdcDemoRaycastInterface (hkpWorld* world) : m_world(world) {}

		virtual hkBool castRay ( const hkVector4& fromWS, const hkVector4& toWS, hkReal& hitFractionOut, hkVector4& normalWSOut );

	private:
		hkpWorld* m_world;
};


// initFooIk() creates the Foot Placement IK solvers (they are persistent between frames). We need one for each leg.
void Gdc2005Demo::initFootIK ()
{
	// This is the ray cast interface instance we will use at run time with the foot placement solver
	m_raycastInterface = new GdcDemoRaycastInterface (m_world);

	const hkaSkeleton& skeleton = *m_animatedSkeleton->getSkeleton();

	hkaFootPlacementIkSolver::Setup setupData;

	// Here we set up all the setup information required for the foot placement component. Check the Havok Animation User Guide for
	// more information about these parameters.

	// COMMON. Many parameters are common for both legs
	{
		setupData.m_skeleton = &skeleton;
		setupData.m_kneeAxisLS.set(-1,0,0);				// The knee(s) rotate around -X
		setupData.m_footEndLS.set(0.0f, 0.0f, 0.2f);	// The foot has a length of 20cm (on the Z axis)
		setupData.m_worldUpDirectionWS.set(0,1,0);		// Y is up (in the world)
		setupData.m_modelUpDirectionMS.set(0,1,0);		// Y is up (in the model)
		setupData.m_originalGroundHeightMS = 0.0f;		// Our animations have the ground at Y=0
		setupData.m_minAnkleHeightMS = -0.1f;			// We won't allow our ankle to go any lower
		setupData.m_maxAnkleHeightMS = 0.7f;			// We won't allow our ankle to go any higher
		setupData.m_footPlantedAnkleHeightMS = 0.14f;	// At this height (and lower), we'll assume the foot is fully planted
		setupData.m_footRaisedAnkleHeightMS = 0.5f;		// At this height (and higher), we'll asume the foot is fully raised
		setupData.m_cosineMaxKneeAngle = -0.95f;		// Don't let the knee go too close to full extension
		setupData.m_cosineMinKneeAngle = 0.95f;			// Don't let the knee go too close to full contraction
		setupData.m_raycastDistanceUp = 0.5f;			// Rays casted start 50cm above the foot
		setupData.m_raycastDistanceDown = 0.8f;			// Rays casyed end 80cm below the foot
	}

	// LEFT LEG. The bone indices are specific for each leg
	{
		setupData.m_hipIndex = hkaSkeletonUtils::findBoneWithName( skeleton, "LeftHip" );
		setupData.m_kneeIndex = hkaSkeletonUtils::findBoneWithName( skeleton, "LeftKnee" );
		setupData.m_ankleIndex = hkaSkeletonUtils::findBoneWithName( skeleton, "LeftAnkle" );

		m_footPlacementComponent[LEFT_LEG] = new hkaFootPlacementIkSolver(setupData);
	}

	// RIGHT LEG. The bone indices are specific for each leg
	{
		setupData.m_hipIndex = hkaSkeletonUtils::findBoneWithName( skeleton, "RightHip" );
		setupData.m_kneeIndex = hkaSkeletonUtils::findBoneWithName( skeleton, "RightKnee" );
		setupData.m_ankleIndex = hkaSkeletonUtils::findBoneWithName( skeleton, "RightAnkle" );

		m_footPlacementComponent[RIGHT_LEG] = new hkaFootPlacementIkSolver(setupData);
	}

	// footOnAir is used by the application logic (it pushes the character down if both feet are in the air)
	m_footOnAir[LEFT_LEG] = m_footOnAir[RIGHT_LEG] = false;

	// Start without any vertical displacement
	m_verticalDisplacement = 0.0f;
}

// quitFookIK() is called by the destructor of the demo
void Gdc2005Demo::quitFootIK()
{
	delete m_raycastInterface;
	delete m_footPlacementComponent[LEFT_LEG];
	delete m_footPlacementComponent[RIGHT_LEG];
}


// doFootIk() uses the hkaFootPlacementIkSolver to place the feet on the ground, and also updates the vertical position of the character
void Gdc2005Demo::doFootIK (hkBool footIkOn, const hkQsTransform& originalLeftFootMS, const hkQsTransform& originalRightFootMS, hkaPose& poseInOut, hkReal& verticalDisplacementInOut)
{
	// Do foot ik
	hkReal rightError;
	hkReal leftError;

	const bool isStanding = m_animationStateMachine->getCurrentState()==GDC_STAND_STATE;

	// Update some tweakable setup parameters
	m_footPlacementComponent[RIGHT_LEG]->m_setup.m_maxAnkleHeightMS = m_options.m_IK.m_footIkMaxFootHeightMS;
	m_footPlacementComponent[LEFT_LEG]->m_setup.m_maxAnkleHeightMS = m_options.m_IK.m_footIkMaxFootHeightMS;
	m_footPlacementComponent[RIGHT_LEG]->m_setup.m_minAnkleHeightMS = m_options.m_IK.m_footIkMinFootHeightMS;
	m_footPlacementComponent[LEFT_LEG]->m_setup.m_minAnkleHeightMS = m_options.m_IK.m_footIkMinFootHeightMS;

	hkaFootPlacementIkSolver::Input footPlacementInput;
	footPlacementInput.m_raycastInterface = m_raycastInterface;
	footPlacementInput.m_worldFromModel = m_currentTransform;
	footPlacementInput.m_groundAscendingGain = (isStanding) ? m_options.m_IK.m_footIkStandAscendingGain : m_options.m_IK.m_footIkAscendingGain;
	footPlacementInput.m_groundDescendingGain = m_options.m_IK.m_footIkDescendingGain;
	footPlacementInput.m_footPlantedGain = 1.0f;
	footPlacementInput.m_footRaisedGain = 0.2f;
	footPlacementInput.m_onOffGain = m_options.m_IK.m_footIkOnOffGain;
	footPlacementInput.m_footPlacementOn = footIkOn;

	// LEFT LEG
	{
		footPlacementInput.m_originalAnkleTransformMS = originalLeftFootMS;

		hkaFootPlacementIkSolver::Output footPlacementOutput;
		m_footPlacementComponent[LEFT_LEG]->doFootPlacement(footPlacementInput, footPlacementOutput, poseInOut);

		leftError = footPlacementOutput.m_verticalError;
		m_footOnAir[LEFT_LEG] = ! footPlacementOutput.m_hitSomething;
	}

	// RIGHT LEG
	{
		footPlacementInput.m_originalAnkleTransformMS = originalRightFootMS;

		hkaFootPlacementIkSolver::Output footPlacementOutput;
		m_footPlacementComponent[RIGHT_LEG]->doFootPlacement(footPlacementInput, footPlacementOutput, poseInOut);

		rightError = footPlacementOutput.m_verticalError;
		m_footOnAir[RIGHT_LEG] = ! footPlacementOutput.m_hitSomething;
	}

	// If standing, we always bias going down
	const hkReal w = isStanding ? 1.0f : m_options.m_IK.m_pelvisUpDownBias;
	const hkReal weightedError = w * hkMath::min2(leftError, rightError) + (1.0f - w) * hkMath::max2(leftError, rightError);
	hkReal pelvisDelta = weightedError * m_options.m_IK.m_pelvisFeedback; // damp it

	verticalDisplacementInOut += pelvisDelta;

	// Clamp
	if (verticalDisplacementInOut > 0.5f)
	{
		verticalDisplacementInOut = 0.5f;
	}
	if (verticalDisplacementInOut < -0.5f)
	{
		verticalDisplacementInOut = -0.5f;
	}
}


// Implementation of our raycast interface (used by the foot IK - and reused by the hand reach component)
// We use Havok Physics, using our collision layers to only report hits against landscape and movable environment (crates and see-saw)
hkBool GdcDemoRaycastInterface::castRay ( const hkVector4& fromWS, const hkVector4& toWS, hkReal& hitFractionOut, hkVector4& normalWSOut )
{
	HK_TIMER_BEGIN("Raycast", HK_NULL);

	// Raycast info structures, we use the RAYCAST layer to filter hits
	hkpWorldRayCastInput raycastIn;
	raycastIn.m_from = fromWS;
	raycastIn.m_to = toWS;
	raycastIn.m_filterInfo = hkpGroupFilter::calcFilterInfo(Gdc2005Demo::LAYER_RAYCAST,0);

	// Cast the ray into the landscape
	// We only collect the closest hit
	hkpClosestRayHitCollector rayCollector;
	m_world->castRay( raycastIn, rayCollector );

	const hkBool didHit = rayCollector.hasHit();

	if (didHit)
	{
		const hkpWorldRayCastOutput& raycastOut = rayCollector.getHit();

		/*	// Useful IK display code
		{
			hkVector4 intersectionPointWorld;
			intersectionPointWorld.setInterpolate4( raycastIn.m_from, raycastIn.m_to, raycastOut.m_hitFraction );
			HK_DISPLAY_LINE( raycastIn.m_from, intersectionPointWorld, hkColor::YELLOW);
			HK_DISPLAY_ARROW( intersectionPointWorld, raycastOut.m_normal, hkColor::CYAN)
		}
		*/

		normalWSOut = raycastOut.m_normal;
		hitFractionOut = raycastOut.m_hitFraction;

		const hkVector4 upWS(0,1,0);

			// Ignore surfaces too steep. Let's pick an angle of 70 degrees. Cos(70) ~= 0.34
		if (normalWSOut.dot3(upWS) < 0.34f)
		{
			return false;
		}

		// For movable convex shapes (crates), we need to consider that the object they represent is slightly bigger
		// Therefore we move the result up by the radius.

		const hkpShape* shape = raycastOut.m_rootCollidable->getShape();
		if (shape->getType() == HK_SHAPE_CONVEX_TRANSLATE)
		{
			// Add radius
			hkVector4 ray; ray.setSub4( raycastIn.m_to, raycastIn.m_from);

			hkReal rayLength = ray.length3();
			hitFractionOut -= ((const hkpConvexTranslateShape*)shape)->getRadius() / rayLength;
			hitFractionOut = hkMath::max2(0.0f, hitFractionOut);
		}
	}

	HK_TIMER_END();

	return didHit;
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
