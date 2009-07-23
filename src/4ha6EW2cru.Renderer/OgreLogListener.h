/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   OgreLogListener.hpp
*  @date   2009/07/21
*/
#pragma once
#ifndef OGRELOGLISTENER_HPP
#define OGRELOGLISTENER_HPP

#include <OgreLog.h>

namespace Renderer
{
	/*! 
	 *  A Listener to intercept log messages from OGRE
	 */
	class OgreLogListener : public Ogre::LogListener
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~OgreLogListener( ) { };


		/*! Default Constructor
		*
		* @return (  )
		*/
		OgreLogListener( ) { };


		void messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName );

	private:

		OgreLogListener( const OgreLogListener & copy ) { };
		OgreLogListener & operator = ( const OgreLogListener & copy ) { return *this; };
		
	};
};

#endif