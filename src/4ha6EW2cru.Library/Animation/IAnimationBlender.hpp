/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   IAnimationBlender.hpp
*  @date   2009/06/06
*/
#pragma once
#ifndef IANIMATIONBLENDER_HPP
#define IANIMATIONBLENDER_HPP

#include <string>
#include <Common/Base/hkBase.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>

namespace Animation
{
	/*! 
	 *  Blends a Source Animation with a Target Animation using weights
	 */
	class IAnimationBlender
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		virtual ~IAnimationBlender( ) { };


		/*! Registers a controller to play the specified animation name
		*
		* @param[in] const std::string & animationName
		* @param[in] hkaDefaultAnimationControl * controller
		* @return ( void )
		*/
		virtual void RegisterController( const std::string& animationName, hkaDefaultAnimationControl* controller ) = 0;


		/*! Blends the given animation into the main animation loop
		*
		* @param[in] const std::string & animationName
		* @return ( void )
		*/
		virtual void Blend( const std::string& animationName ) = 0;


		/*! UnBlends the given animation into the main animation loop
		*
		* @param[in] const std::string & animationName
		* @return ( void )
		*/
		virtual void UnBlend( const std::string& animationName ) = 0;


		/*! Steps the Animation Blend forward
		*
		* @param[in] const float & deltaMilliseconds
		* @return ( void )
		*/
		virtual void Update( const float& deltaMilliseconds ) = 0;
		
	};
};

#endif