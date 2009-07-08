/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   AnimationController.h
*  @date   2009/06/03
*/
#pragma once
#ifndef ANIMATIONCONTROLLER_H
#define ANIMATIONCONTROLLER_H

#include "IAnimationController.hpp"

namespace Renderer
{
	/*! 
	 *  A Controller for multiple animations given the same name
	 */
	class AnimationController : public IAnimationController
	{

		typedef std::deque< Ogre::AnimationState* > AnimationStateList;

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~AnimationController( ) { };


		/*! Default Constructor
		*
		* @return (  )
		*/
		AnimationController( const std::string& name )
			: m_name( name )
		{

		}


		/*! Adds an Animation State to the internal State List
		*
		* @param[in] AnimationState * animationState
		* @return ( void )
		*/
		inline void AddAnimationState( Ogre::AnimationState* animationState ) { m_animationStates.push_back( animationState ); };


		/*! Sets whether or not the animations will be looped
		*
		* @param[in] const bool & loop
		* @return ( void )
		*/
		void SetLoop( const bool& loop );


		/*! Sets the Weight for the Animations
		*
		* @param[in] const float & weight
		* @return ( void )
		*/
		void SetWeight( const float& weight );


		/*! Sets whether or not the animations within this controller are enabled
		*
		* @param[in] const bool & enabled
		* @return ( void )
		*/
		void SetEnabled( const bool& enabled );


		/*! Returns the Animation Name that Controller contains
		*
		* @return ( void )
		*/
		inline std::string GetName( ) const { return m_name; };


		/*! Steps each Animation within the Controller
		*
		* @return ( void )
		*/
		void Update( const float& deltaMilliseconds );


		/*! Sets the Time Position of the Animation
		*
		* @param[in] const float & time
		* @return ( void )
		*/
		void SetTime( const float& time );


	private:

		AnimationController( const AnimationController & copy ) { };
		AnimationController & operator = ( const AnimationController & copy ) { return *this; };

		AnimationStateList m_animationStates;
		std::string m_name;
		
	};
};

#endif