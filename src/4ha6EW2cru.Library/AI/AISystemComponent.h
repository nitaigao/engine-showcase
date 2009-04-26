/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   src\4ha6EW2cru.Library\AI\AISystemComponent.h
*  @date   2009/04/25
*/
#ifndef __AISYSTEMCOMPONENT_H
#define __AISYSTEMCOMPONENT_H

#include <string>
#include "../System/SystemType.hpp"

#include "../Scripting/ScriptComponent.h"

#include "../Maths/MathVector3.hpp"
#include "../Maths/MathQuaternion.hpp"

#include "IBehavior.hpp"

namespace AI
{
	/*! 
	 *  An Artificial Intelligence System Scene Component
	 */
	class AISystemComponent : public ISystemComponent, public IBehaviour
	{

	public:


		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~AISystemComponent( ) { };


		/*! Default Constructor
		 *
		 *  @param[in] const std::string & name
		 *  @param[in] IScriptComponent * scriptComponent
		 *  @return ()
		 */
		AISystemComponent( const std::string& name, IScriptComponent* scriptComponent )
			: _name( name )
			, _scriptComponent( scriptComponent )
			, _observer( 0 )
		{

		}

		/* Inherited from ISystemComponent */

		/*! Initializes the Component
		 *
		 *  @param[in] AnyValue::AnyValueMap properties
		 *  @return (void)
		 */
		void Initialize( AnyValue::AnyValueMap properties );


		/*! Steps the internal data of the Component
		 *
		 *  @param[in] float deltaMilliseconds
		 *  @return (void)
		 */
		void Update( float deltaMilliseconds );


		/*! Destroys the Component
		 *
		 *  @return (void)
		 */
		void Destroy( ) { };


		/*! Adds an Observer to the Component
		 *
		 *  @param[in] IObserver * observer
		 *  @return (void)
		 */
		void AddObserver( IObserver* observer );


		/*! Observes a change in the Subject
		 *
		 *  @param[in] ISubject * subject
		 *  @param[in] unsigned int systemChanges
		 *  @return (void)
		 */
		void Observe( ISubject* subject, unsigned int systemChanges );


		/*! Pushes any Changes to the Observers
		 *
		 *  @param[in] unsigned int systemChanges
		 *  @return (void)
		 */
		void PushChanges( unsigned int systemChanges );


		/*! Gets the Name of the Component
		 *
		 *  @return (const std::string&)
		 */
		inline const std::string& GetName( ) { return _name; };


		/*! Gets the System::Types::Type of the Component
		 *
		 *  @return (System::Types::Type)
		 */
		inline System::Types::Type GetType( ) { return System::Types::AI; };


		/*! Gets the Tyes of Changes this component is interested in
		 *
		 *  @return (unsigned int)
		 */
		inline unsigned int GetRequestedChanges( ) 
		{ 
			return System::Changes::Geometry::All;
		};


		/*! Gets the properties of the Component
		 *
		 *  @return (AnyValueMap)
		 */
		inline AnyValue::AnyValueMap GetProperties( ) { return AnyValue::AnyValueMap( ); };


		/*! Sets the Properties of the Component
		 *
		 *  @param[in] AnyValue::AnyValueMap systemProperties
		 *  @return (void)
		 */
		inline void SetProperties( AnyValue::AnyValueMap systemProperties ) { };


		/*! Gets the Position of the Component
		 *
		 *  @return (MathVector3)
		 */
		inline MathVector3 GetPosition( ) { return _position; };


		/*! Gets the Scale of the Component
		 *
		 *  @return (MathVector3)
		 */
		inline MathVector3 GetScale( ) { return MathVector3::Zero( ); };


		/*! Gets the Orientation of the Component
		 *
		 *  @return (MathQuaternion)
		 */
		inline MathQuaternion GetOrientation( ) { return _orientation; };


		/* Inherited from IBehavior */

		/*! Sets the current Behavior of the Component
		 *
		 *  @param[in] const std::string & behavior
		 *  @return (void)
		 */
		void SetBehavior( const std::string& behavior );


		/*! Gets the current Behavior of the Component
		 *
		 *  @return (std::string)
		 */
		std::string GetBehavior( ) const { return _behavior; };


		/* AI Specific */
		//TODO: Move these to the Scripting System
		/* Script Callbacks */

		/*! Walks the Parent Entity Forward
		 *
		 *  @return (void)
		 */
		void WalkForward( );


		/*! Walks the Parent Entity Backwards
		 *
		 *  @return (void)
		 */
		void WalkBackward( );


		/*! Rotates the Parent Entity towards the Player
		 *
		 *  @return (void)
		 */
		void FacePlayer( );


		/*! Fires the AI Weapon
		 *
		 *  @return (void)
		 */
		void FireWeapon( );


		/*! Gets the Distance to the Player
		 *
		 *  @return (float)
		 */
		float GetPlayerDistance( );

	private:

		std::string _name;
		IScriptComponent* _scriptComponent;

		IObserver* _observer;

		std::string _behavior;

		MathVector3 _position;
		MathQuaternion _orientation;

		MathVector3 _playerPosition;
		MathQuaternion _playerOrientation;

	};
};

#endif