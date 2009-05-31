#pragma once
#ifndef AISCRIPTCOMPONENT_H
#define AISCRIPTCOMPONENT_H

#include "AISystemComponent.h"

#include "../Maths/MathVector3.hpp"

namespace AI
{
	class AIScriptComponent : public AISystemComponent
	{

	public:

		~AIScriptComponent( ) { };

		AIScriptComponent( const std::string& name )
			: AISystemComponent( name )
		{

		}


		/* Inherited from ISystemComponent */

		/*! Initializes the Component
		*
		*  @param[in] AnyType::AnyValueMap attributes
		*  @return (void)
		*/
		void Initialize( );


		/*! Steps the internal data of the Component
		*
		*  @param[in] float deltaMilliseconds
		*  @return (void)
		*/
		void Update( const float& deltaMilliseconds );


		/*! Destroys the Component
		*
		*  @return (void)
		*/
		void Destroy( );

		/* AI Specific */


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
		void FacePosition( const Maths::MathVector3& position );


		/*! Fires the AI Weapon
		*
		*  @return (void)
		*/
		void FireWeapon( );


		/*! Gets the Distance to the Player
		*
		*  @return (float)
		*/
		inline float GetPlayerDistance( ) { return m_playerDistance; };



		/*! Returns the number of waypoints left to process
		*
		* @return ( unsigned int )
		*/
		unsigned int GetWaypointCount( ) { return m_activeWaypoints.size( ); };


		/*! Finds a Random waypoint on the Map
		*
		* @return ( Maths::MathVector3 )
		*/
		Maths::MathVector3 FindRandomWaypoint( );


		/*! Navigate to a position in the World
		*
		* @param[in] const Maths::MathVector3 & position
		* @return ( void )
		*/
		void NavigateTo( const Maths::MathVector3& position );


		/*! Plays an Animation
		*
		*  @param[in] const std::string & animationName
		*  @param[in] const bool & loopAnimation
		*  @return (void)
		*/
		void PlayAnimation( const std::string& animationName, const bool& loopAnimation );


		/*! Gets the Name of the Component
		*
		*  @return (const std::string&)
		*/
		inline std::string GetName( ) { return m_attributes[ System::Attributes::Name ].As< std::string >( ); };

	};
};



#endif