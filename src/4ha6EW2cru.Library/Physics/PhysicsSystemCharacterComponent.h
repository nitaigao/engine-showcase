/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   PhysicsSystemCharacterComponent.h
*  @date   2009/04/26
*/
#ifndef PHYSICSSYSTEMCHARACTERCOMPONENT_H
#define PHYSICSSYSTEMCHARACTERCOMPONENT_H

#include "PhysicsSystemComponent.h"

#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>

namespace Physics
{
	/*! 
	 *  A representation of a Character within the Physics System
	 */
	class PhysicsSystemCharacterComponent : public PhysicsSystemComponent
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~PhysicsSystemCharacterComponent( );


		/*! Default Constructor
		 *
		 *  @param[in] const std::string & name
		 *  @param[in] HavokPhysicsSystemScene * scene
		 *  @param[in] int componentId
		 *  @return ()
		 */
		PhysicsSystemCharacterComponent( const std::string& name, HavokPhysicsSystemScene* scene )
			: PhysicsSystemComponent( name, scene )
			, m_characterBody( 0 )
			, m_characterContext( 0 )
			, m_framesInAir( 0 )
			, m_forwardBackward( 0 )
			, m_leftRight( 0 )
			, m_previousGround( new hkpSurfaceInfo( ) )
		{

		}


		/*! Initializes the Component
		*
		*  @param[in] AnyValue::AnyValueMap properties
		*  @return (void)
		*/
		void Initialize( );


		/*! Steps the internal data of the Component
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds );

		AnyValue Message( const unsigned int& messageId, AnyValue::AnyValueKeyMap parameters );

	private:

		hkpCharacterContext* m_characterContext;
		hkpCharacterRigidBody* m_characterBody;
		hkpCharacterInput m_characterInput;
		hkpSurfaceInfo* m_previousGround;

		float m_forwardBackward;
		float m_leftRight;
		int m_framesInAir;

		PhysicsSystemCharacterComponent & operator = ( const PhysicsSystemCharacterComponent & copy ) { return *this; };

	};
};

#endif