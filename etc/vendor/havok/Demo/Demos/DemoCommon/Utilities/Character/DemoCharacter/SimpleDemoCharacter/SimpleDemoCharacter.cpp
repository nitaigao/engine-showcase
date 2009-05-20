/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#include <Demos/demos.h>
#include <Demos/DemoCommon/Utilities/Character/DemoCharacter/SimpleDemoCharacter/SimpleDemoCharacter.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterStepInput.h>
#include <Demos/DemoCommon/Utilities/Character/CharacterProxy/CharacterProxy.h>

#include <Common/Visualize/hkDebugDisplay.h>

SimpleDemoCharacter::SimpleDemoCharacter( SimpleDemoCharacterCinfo& info )
: DemoCharacter(info)
{
	m_gravity = info.m_gravity;
	m_maxVelocity = info.m_maxVelocity;
}

void SimpleDemoCharacter::update( hkReal timestep, hkpWorld* world, const CharacterStepInput& input, struct CharacterActionInfo* actionInfo )
{
	hkVector4 desiredVelocityWS;desiredVelocityWS.setZero4();

	hkVector4 characterLinearVelocity;
	m_characterProxy->getLinearVelocity( characterLinearVelocity );
	if ( input.m_jumpVelocity )
	{
		if (!m_characterProxy->isSupported( timestep ))
		{
			desiredVelocityWS = characterLinearVelocity;
		}
		else
		{
			// this is the common case: add the motion velocity to the downward part of the proxy velocity (not upward though)
			hkReal vertComponent = hkMath::min2(0.0f, static_cast<hkReal>(characterLinearVelocity.dot3( m_characterProxy->getUpLocal() )));
			desiredVelocityWS.addMul4(vertComponent, m_characterProxy->getUpLocal() );
		}
	}
	else
	{
		// when jumping, add the motion velocity to the vertical proxy velocity
		hkReal vertComponent = characterLinearVelocity.dot3(m_characterProxy->getUpLocal());
		desiredVelocityWS.addMul4(vertComponent, m_characterProxy->getUpLocal());
	}

	hkTransform wFc;
	m_characterProxy->getTransform( wFc );

	hkReal turnAngle = input.m_turnVelocity * timestep;
	{
		hkQuaternion newRotation; newRotation.setAxisAngle( m_characterProxy->getUpLocal(), turnAngle );
		hkRotation r; r.set( newRotation );
		wFc.getRotation().mul( r );
		m_characterProxy->setTransform( wFc );
	}

	// Add the forward velocity
	hkVector4 forwardVel;
	forwardVel.setRotatedDir( wFc.getRotation(), m_characterProxy->getForwardLocal() );
	desiredVelocityWS.addMul4( input.m_forwardVelocity, forwardVel );



	// Add in Jump velocity
	if (m_characterProxy->isSupported(timestep))
	{
		desiredVelocityWS.addMul4( input.m_jumpVelocity, m_characterProxy->getUpLocal() );
	}
	else
	{
		// Add some gravity if not supported
		desiredVelocityWS.addMul4( timestep, m_gravity );
	}

	m_characterProxy->setLinearVelocity( desiredVelocityWS );

	m_characterProxy->setTurnVelocity( input.m_turnVelocity );
}

void SimpleDemoCharacter::display( hkReal timestep, hkDemoEnvironment* env )
{
	hkTransform t;
	m_characterProxy->getTransform(t);
	hkVector4 to;
	to.setRotatedDir( t.getRotation(), m_characterProxy->getForwardLocal());
	HK_DISPLAY_ARROW( t.getTranslation(), to, hkColor::WHITE );
	// Nothing for simple proxy

}

// get maximum velocity
hkReal SimpleDemoCharacter::getMaxVelocity() const
{
	return m_maxVelocity;
}

DemoCharacter* SimpleCharacterFactory::createCharacterUsingProxy( CharacterProxy* proxy, const hkVector4& gravity, hkDemoEnvironment* env )
{
	// Simple Character only
	SimpleDemoCharacterCinfo sinfo;
	sinfo.m_characterProxy = proxy;
	sinfo.m_gravity = gravity;

	sinfo.m_maxVelocity = 6.f;

	return new SimpleDemoCharacter( sinfo );
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
