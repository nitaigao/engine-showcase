/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_CHARACTER_RIGIDBODY__H
#define HK_CHARACTER_RIGIDBODY__H

#include <Common/Base/Types/Physics/hkStepInfo.h>
#include <Physics/Utilities/CharacterControl/hkpCharacterControl.h>

class hkpRigidBody;
class hkpPhantomCallbackShape;
class hkCharacterRigidBodyCollisionListener;
class hkpShape;

struct hkpSurfaceInfo;

/// Character controller cinfo
struct hkpCharacterRigidBodyCinfo
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CHARACTER, hkpCharacterRigidBodyCinfo );

	//
	// Values used to seup rigid body
	//
		/// The collision filter info
		/// See hkRigidBodyCinfo for details
	hkUint32 m_collisionFilterInfo;

		/// The shape
		/// See hkRigidBodyCinfo for details
	hkpShape* m_shape;	

		/// Initial position
		/// See hkRigidBodyCinfo for details
	hkVector4 m_position;

		/// Initial rotation
		/// See hkRigidBodyCinfo for details
	hkQuaternion m_rotation;

		/// The mass of character
		/// See hkRigidBodyCinfo for details
	hkReal m_mass;	

		/// Set friction of character
		/// See hkRigidBodyCinfo for details
	hkReal m_friction;

		/// Set maximal linear velocity
		/// See hkRigidBodyCinfo for details
	hkReal m_maxLinearVelocity;

		/// Set maximal allowed penetration depth
		/// See hkRigidBodyCinfo for details
	hkReal m_allowedPenetrationDepth;


	//
	// Character controller specific values
	//

		/// Set up direction 
	hkVector4 m_up;	

		/// Set maximal slope
	hkReal m_maxSlope;

		/// Set maximal force of character
	hkReal m_maxForce;


		//
		// Parameters used by checkSupport
		//

		/// Set maximal speed for simplex solver
	hkReal m_maxSpeedForSimplexSolver;

		/// Set penetration recovery speed
	hkReal m_penetrationRecoverySpeed;

		/// Internal use
	hkInt32 m_userPlane;


		/// Set color of character for the visual debugger
	hkInt32	m_vdbColor;




	// Constructor. Sets some defaults.
	hkpCharacterRigidBodyCinfo()
	{
		m_mass = 100.0f;
		m_maxForce = 1000.0f;
		m_friction = 0.0f;
		m_maxSlope = HK_REAL_PI / 3.0f;;
		m_up.set(0,0,1,0);
		m_userPlane = 0;
		m_maxLinearVelocity = 20.0f;
		m_allowedPenetrationDepth = -0.1f;
		m_maxSpeedForSimplexSolver = 10.0f;
		m_penetrationRecoverySpeed = 1.0f;
		m_collisionFilterInfo = 0;
		m_position.setZero4();
		m_rotation.setIdentity();

		m_vdbColor = 0xA0FF0000;
	}

};


/// The character rigid body class is used to represent a rigid body that can move dynamically around 
/// the scene. It is called character rigid body because it is usually used to represent game characters. 
/// It is a rigid body analogy to hkpCharacterProxy controller. But opposite to hkpCharacterProxy, which 
/// is represented by virtual phantom shape, hkpCharacterRigidBody controller is physically presented in the dynamic scene. 
/// It's movement is controlled by direct setting its linear and angular velocities. 
class hkpCharacterRigidBody :	public hkReferencedObject	
{

	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CHARACTER);

			/// Constructor
			/// Creates a rigid body, and an associated contact point listener
		hkpCharacterRigidBody( const hkpCharacterRigidBodyCinfo& info );

			/// Destructor
		~hkpCharacterRigidBody();

			/// Get rigid body of character		
		hkpRigidBody* getRigidBody() const;
				
			/// Check and see if the character is supported in the given direction (and by what).
			/// This call checks the geometry immediately around the character, and does not take velocity into account.
			/// i.e. if the character is moving away from the ground, but is still touching the ground, this function
			/// will return SUPPORTED as the supported state.
		void checkSupport(const hkStepInfo& stepInfo, hkpSurfaceInfo& ground) const;	


			/// Set linear acceleration for mass modifier
		void setLinearAccelerationToMassModifier(const hkVector4& newAcc);
		

			/// Set the maximum force the character is able to apply to another physical object
		void setMaxForce( hkReal maxForce );

			//
			// Simple wrappers for rigid body functions
			//

			/// Set character linear velocity.  The timestep argument should be given the same timestep that is passed
			/// to the physics simulation step.  Note, the timestep must be greater than 0.0f.  A value of 0.0f or less
			/// will set hkpCharacterRigidBodyCollisionListener::m_acceleration to be INF, which could cause crashes.
		void setLinearVelocity(const hkVector4& newVel, hkReal timestep );

			/// Get character linear velocity
		const hkVector4& getLinearVelocity() const;
		
			/// Set character angular velocity
		void setAngularVelocity(const hkVector4& newAVel);

			/// Get character angular velocity
		const hkVector4& getAngularVelocity() const;
		
			/// Get character position
		const hkVector4& getPosition() const;


	public:

		hkpRigidBody*	m_character;

		hkCharacterRigidBodyCollisionListener* m_characterCollisionListener; 

		hkVector4 m_up;

		hkReal m_maxSlopeCosine;

		hkInt32 m_userPlanes;
		
		hkReal m_penetrationRecoverySpeed;

		hkReal m_maxSpeedForSimplexSolver;
	
};

#endif //HK_CHARACTER_RIGIDBODY__H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20080925)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2008
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
