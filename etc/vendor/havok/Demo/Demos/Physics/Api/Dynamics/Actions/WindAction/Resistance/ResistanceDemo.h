/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_AERODYNAMICS_ACTION_API_H
#define HK_AERODYNAMICS_ACTION_API_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

#include <Physics/Dynamics/World/Listener/hkpWorldPostSimulationListener.h>

#include <Physics/Utilities/Actions/Wind/hkpWind.h>

/// A very simple way of applying air resistance to a set of rigid bodies is to use a
/// hkpWorldPostSimulationListener.
class ResistanceManager : public hkpWorldPostSimulationListener
{
	public:
			/// Apply resistances to the rigid bodies.
		virtual void postSimulationCallback( hkpWorld* world );
			/// Add a rigid body to the collection.
		void add( hkpRigidBody* rb );
			/// Destructor.
			/// Removes references to the bodies.
		virtual ~ResistanceManager();
	private:
			/// The rigid bodies whose resistance we're managing.
		hkArray< hkpRigidBody* > m_bodies;

public:
		hkpWind* m_wind;
};

class ResistanceDemo : public hkDefaultPhysicsDemo
{
	public:
		ResistanceDemo( hkDemoEnvironment* env );

	private:
		hkVector4 initialPosition( int i );

		hkArray< hkpRigidBody* > m_bodies;

		ResistanceManager m_resistanceManager;
};

#endif // HK_AERODYNAMICS_ACTION_API_H

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
