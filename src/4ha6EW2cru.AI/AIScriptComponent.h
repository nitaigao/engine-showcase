/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   AIScriptComponent.h
*  @date   2009/07/08
*/
#pragma once
#ifndef AISCRIPTCOMPONENT_H
#define AISCRIPTCOMPONENT_H

#include "AISystemComponent.h"

#include "Maths/MathVector3.hpp"

namespace AI
{
	/*! 
	 *  A Script Component for the AI System
	 */
	class AIScriptComponent : public AISystemComponent
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
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


		/*! Returns the position of the Player
		*
		* @return ( Maths::MathVector3 )
		*/
		inline Maths::MathVector3 GetPlayerPosition( ) { return m_attributes[ System::Parameters::PlayerPosition ].As< Maths::MathVector3 >( ); };


		/*! Returns the number of waypoints left to process
		*
		* @return ( unsigned int )
		*/
		unsigned int GetWaypointCount( ) { return m_activeWaypoints.size( ); };



		/*! Tests to see if the given position is in Line of Sight
		*
		* @param[in] const Maths::MathVector3 & position
		* @return ( bool )
		*/
		bool InLineOfSight( const Maths::MathVector3& position );


		/*! Finds a Random waypoint on the Map
		*
		* @return ( Maths::MathVector3 )
		*/
		Maths::MathVector3 FindRandomWaypoint( );


		/*! Returns the waypoint paths to the given position
		*
		* @param[in] const MathVector3 & position
		* @return ( MathVector3::MathVector3List )
		*/
		Maths::MathVector3::MathVector3List GetPathTo( const Maths::MathVector3& position );


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
		
	private:

		AIScriptComponent( ) { };
		AIScriptComponent( const AIScriptComponent & copy ) { };
		AIScriptComponent & operator = ( const AIScriptComponent & copy ) { return *this; };
		
	};
};

#endif