/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   PhysicsSystemCharacterComponent.h
*  @date   2009/04/26
*/
#ifndef __PHYSICSSYSTEMCHARACTERCOMPONENT_H
#define __PHYSICSSYSTEMCHARACTERCOMPONENT_H

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
		PhysicsSystemCharacterComponent( const std::string& name, HavokPhysicsSystemScene* scene, int componentId )
			: PhysicsSystemComponent( name, scene, componentId )
			, _characterBody( 0 )
			, _characterContext( 0 )
			, _framesInAir( 0 )
			, _forwardBackward( 0 )
			, _leftRight( 0 )
			, _previousGround( new hkpSurfaceInfo( ) )
		{

		}


		/*! Initializes the Component
		*
		*  @param[in] AnyValue::AnyValueMap properties
		*  @return (void)
		*/
		void Initialize( AnyValue::AnyValueMap& properties );


		/*! Steps the internal data of the Component
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds );


		/*! Observes a change in the Subject
		*
		*  @param[in] ISubject * subject
		*  @param[in] const unsigned int& systemChanges
		*  @return (void)
		*/
		void Observe( ISubject* subject, const unsigned int& systemChanges );

	private:

		hkpCharacterContext* _characterContext;
		hkpCharacterRigidBody* _characterBody;
		hkpCharacterInput _characterInput;
		hkpSurfaceInfo* _previousGround;

		float _forwardBackward;
		float _leftRight;
		int _framesInAir;

		PhysicsSystemCharacterComponent & operator = ( const PhysicsSystemCharacterComponent & copy ) { return *this; };

	};
};

#endif