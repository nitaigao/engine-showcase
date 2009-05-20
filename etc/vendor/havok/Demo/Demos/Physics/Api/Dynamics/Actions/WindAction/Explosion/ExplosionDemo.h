/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef EXPLOSION_DEMO_H
#define EXPLOSION_DEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

#include <Common/Base/Algorithm/PseudoRandom/hkPseudoRandomGenerator.h>

#include <Physics/Utilities/Actions/Wind/hkpWind.h>
#include <Physics/Utilities/Actions/Wind/hkpWindRegion.h>

/// Explosion data.
class ExplosionInfo
{
	public:
			/// The peak strength of the explosion (will be converted to a wind vector)
		hkReal m_peakWindStrength;
			/// The speed at which the explosion propagates.
		hkReal m_propagationSpeed;
			/// The width of shock-wave.
		hkReal m_width;
			/// Constructor.
		ExplosionInfo( hkReal peakWindStrength, hkReal propagationSpeed, hkReal width )
			:	m_peakWindStrength( peakWindStrength ), m_propagationSpeed( propagationSpeed ), m_width( width ) { }
};

/// An explosion modeled using wind.
class Explosion : public hkpWind
{
public:
		/// Constructor.
	Explosion( const hkVector4& center, const ExplosionInfo& info );
		// hkpWind implementation.
	virtual void getWindVector( const hkVector4& pos, hkVector4& windOut ) const;
		/// Returns true if this explosion should be kept alive.
	hkBool isActive() const;
		/// Update the explosion by delta seconds.
	void update( hkReal delta );
private:
		/// The proportion of the maximum peak strength at this time.
	hkReal getStrengthFactor() const;
		/// Details of the explosion.
	const ExplosionInfo m_info;
		/// The point at which the explosion occurred.
	const hkVector4 m_center;
		/// The age of the explosion in seconds.
	hkReal m_age;
};

/// A wind which is the sum of a set of explosions.
class ExplosionManager : public hkpWind
{
	public:
		ExplosionManager();
		virtual ~ExplosionManager();
		void update( hkReal delta );
		virtual void getWindVector( const hkVector4& pos, hkVector4& windOut ) const;
		void addExplosion( const hkVector4& center, const ExplosionInfo& info );
	private:
		hkArray< Explosion* > m_explosions;
};

/// The demo class.
class ExplosionDemo : public hkDefaultPhysicsDemo
{	
	public:
		ExplosionDemo( hkDemoEnvironment* env );
		virtual ~ExplosionDemo();
		virtual Result stepDemo();
		// The half-extents of the demo box.
		const hkReal demoX;
		const hkReal demoY;
		const hkReal demoZ;

	private:
		hkVector4 initialPosition( hkReal height, int i, int gridWidth, hkReal gap );
		ExplosionManager m_explosionManager;
		hkPseudoRandomGenerator m_random;
		ExplosionInfo m_exInfo;
		hkpWindRegion* m_region;
};

#endif // EXPLOSION_DEMO_H

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
