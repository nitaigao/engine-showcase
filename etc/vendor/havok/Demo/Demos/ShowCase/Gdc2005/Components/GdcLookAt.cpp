/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Demos/demos.h>
#include <Demos/ShowCase/Gdc2005/Gdc2005Demo.h>

// Poses & Rigs
#include <Animation/Animation/Rig/hkaPose.h>
#include <Animation/Animation/Rig/hkaSkeleton.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>

#include <Animation/Animation/Ik/LookAt/hkaLookAtIkSolver.h>

// Graphics & Window Stuff
#include <Common/Visualize/hkDebugDisplay.h>

/*
** LOOK AT component.
**
** It makes the character's head follow a target. It uses the LookAt IK solver for so. Thus, the code here mostly deals with
** smoothing out movements and easing the look at in and out.
**
*/

// initLookAt() just stores the indices of the two bones involved (head and neck); it also initializes internal state (used for smoothing)
void Gdc2005Demo::initLookAt()
{
	m_headIdx = hkaSkeletonUtils::findBoneWithName( *m_animatedSkeleton->getSkeleton(), "head" );
	m_neckIdx = hkaSkeletonUtils::findBoneWithName( *m_animatedSkeleton->getSkeleton(), "neck" );
	m_lookAtLastTargetWS.setZero4();
	m_lookAtWeight = 0.0f;
}

// doLookAt() is called during the animation loop. It is called both when we do and when we don't want to do look at, since it handles
// the easing in and out internally. Hence the "lookAtOn" parameter. Most of the work is done in the LookAt Ik solver. Here we just handle
// the smoothing of changes in the target as well as the easing in and out of the component.
void Gdc2005Demo::doLookAt( hkBool lookAtOn, const hkVector4& targetPosWS, hkaPose& thePose)
{
	// Just an alias for the reference frame
	const hkQsTransform& worldFromModel = m_currentTransform;

	// Current transforms (MS) of the head and neck
	const hkQsTransform& headMS = thePose.getBoneModelSpace(m_headIdx);
	const hkQsTransform& neckMS = thePose.getBoneModelSpace(m_neckIdx);

	// Position of the head (MS)
	const hkVector4& headPosMS = headMS.getTranslation();

	// Forward-looking direction (MS)
	hkVector4 forwardDirMS; forwardDirMS.setRotatedDir(headMS.getRotation(), hkVector4(0,0,1));

	if (m_options.m_Display.m_showIkInfo)
	{
		hkVector4 headPosWS; headPosWS.setTransformedPos(worldFromModel, headPosMS);
		HK_DISPLAY_LINE(headPosWS, targetPosWS, 0xffff0000);
	}

	// We want to look at something; but we shouldn't try to look at something behind the character
	if (lookAtOn)
	{
		hkVector4 targetPosMS; targetPosMS.setTransformedInversePos(worldFromModel, targetPosWS);

		hkVector4 headToTargetDirMS; headToTargetDirMS.setSub4(targetPosMS, headPosMS);
		if (headToTargetDirMS.dot3(forwardDirMS)<0.0f)
		{
			// Target is behind us, ignore it
			lookAtOn = false;
		}
	}

	// Still we want to look at something
	if (lookAtOn)
	{
		// If we were doing look at before, blend the new position
		if (m_lookAtWeight>0.0f)
		{
			const hkReal targetGain = m_options.m_IK.m_lookAtTargetGain;
			m_lookAtLastTargetWS.setInterpolate4(m_lookAtLastTargetWS, targetPosWS, targetGain);
		}
		else
		{
			// Otherwise use this new position straight away
			m_lookAtLastTargetWS = targetPosWS;
		}
	}

	// Ease in/out the weight depending on whether there is something we want to look at
	const hkReal desiredWeight = lookAtOn ? 1.0f : 0.0f;
	m_lookAtWeight += (desiredWeight - m_lookAtWeight) * m_options.m_IK.m_lookAtGain;

	// If weight gets under threshold, do nothing
	if (m_lookAtWeight < 0.01f)
	{
		m_lookAtWeight = 0.0f;
	}
	else
	{
		// Use the LookAt Ik solver to modify the orientation of the head
		// Use the weight the gain, so we move smoothly over time
		hkaLookAtIkSolver::Setup setup;
		setup.m_fwdLS.set(0,0,1);
		setup.m_limitAxisMS.setRotatedDir(neckMS.getRotation(), hkVector4(0, 0.34f ,1.0f));
		setup.m_limitAxisMS.normalize3();
		setup.m_limitAngle = m_options.m_IK.m_lookAtLimit;

		hkVector4 targetMS; targetMS.setTransformedInversePos(worldFromModel, m_lookAtLastTargetWS);

		// By using hkaPose and the PROPAGATE flag we automatically update the descendant bones of the head
		hkaLookAtIkSolver::solve(setup, targetMS, m_lookAtWeight, thePose.accessBoneModelSpace(m_headIdx, hkaPose::PROPAGATE));
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
