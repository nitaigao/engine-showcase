/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   AnimationBlender.h
*  @date   2009/06/06
*/
#pragma once
#ifndef ANIMATIONBLENDER_H
#define ANIMATIONBLENDER_H

#include "IAnimationBlender.hpp"
#include <deque>
#include <map>

namespace Animation
{
	/*! 
	 *  Blends a Source Animation with a Target Animation using weights
	 */
	class AnimationBlender : public IAnimationBlender
	{
		typedef std::deque< hkaDefaultAnimationControl* > AnimationList;
		typedef std::map< std::string, hkaDefaultAnimationControl* > AnimationControllerList;

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~AnimationBlender( ) { };


		/*! Default Constructor
		*
		* @return (  )
		*/
		AnimationBlender( )
			: m_sourceController( 0 )
			, m_targetController( 0 )
			, m_timeLeft( 0 )
			, m_blendTime( 0 )
		{

		}

		/*! Registers a controller to play the specified animation name
		*
		* @param[in] const std::string & animationName
		* @param[in] hkaDefaultAnimationControl * controller
		* @return ( void )
		*/
		inline void RegisterController( const std::string& animationName, hkaDefaultAnimationControl* controller ) 
		{ 
			m_animationControllers.insert( std::make_pair( animationName, controller ) ); 
		};


		/*! Blends the given animation into the main animation loop
		*
		* @param[in] const std::string & animationName
		* @return ( void )
		*/
		void Blend( const std::string& animationName );


		/*! UnBlends the given animation into the main animation loop
		*
		* @param[in] const std::string & animationName
		* @return ( void )
		*/
		void UnBlend( const std::string& animationName );


		/*! Steps the Animation Blend forward
		*
		* @param[in] const float & deltaMilliseconds
		* @return ( void )
		*/
		void Update( const float& deltaMilliseconds );

	private:

		AnimationBlender( const AnimationBlender & copy ) { };
		AnimationBlender & operator = ( const AnimationBlender & copy ) { return *this; };

		hkaDefaultAnimationControl* m_sourceController;
		hkaDefaultAnimationControl* m_targetController;

		AnimationControllerList m_blendAnimations;
		AnimationControllerList m_unblendAnimations;
		AnimationControllerList m_animationControllers;

		float m_timeLeft;
		float m_blendTime;
		
	};
};

#endif