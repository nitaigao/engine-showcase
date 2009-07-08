/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IAnimationController.hpp
*  @date   2009/06/03
*/
#pragma once
#ifndef IANIMATIONCONTROLLER_HPP
#define IANIMATIONCONTROLLER_HPP

#include <Ogre.h>

namespace Renderer
{
	/*! 
	 *  A collection of AnimationStates grouped under the same name
	 */
	class IAnimationController
	{

	public:

		typedef std::map< std::string, IAnimationController* > AnimationControllerMap;

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IAnimationController( ) { };


		/*! Adds an Animation State to the internal State List
		*
		* @param[in] AnimationState * animationState
		* @return ( void )
		*/
		virtual void AddAnimationState( Ogre::AnimationState* animationState ) = 0;


		/*! Sets whether or not the animations will be looped
		*
		* @param[in] const bool & loop
		* @return ( void )
		*/
		virtual void SetLoop( const bool& loop ) = 0;


		/*! Sets the Weight for the Animations
		*
		* @param[in] const float & weight
		* @return ( void )
		*/
		virtual void SetWeight( const float& weight ) = 0;


		/*! Sets whether or not the animations within this controller are enabled
		*
		* @param[in] const bool & enabled
		* @return ( void )
		*/
		virtual void SetEnabled( const bool& enabled ) = 0;


		/*! Returns the Animation Name that Controller contains
		*
		* @return ( void )
		*/
		virtual std::string GetName( ) const = 0;


		/*! Steps each Animation within the Controller
		*
		* @return ( void )
		*/
		virtual void Update( const float& deltaMilliseconds ) = 0;


		/*! Sets the Time Position of the Animation
		*
		* @param[in] const float & time
		* @return ( void )
		*/
		virtual void SetTime( const float& time ) = 0;

		
	};
};

#endif