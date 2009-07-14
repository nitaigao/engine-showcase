/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   SoundFacade.h
*  @date   2009/04/28
*/
#pragma once
#ifndef SOUNDFACADE_H
#define SOUNDFACADE_H

#include "ISoundFacade.hpp"
#include "IScriptComponent.hpp"

#include <luabind/luabind.hpp>

namespace Script
{
	/*!
	 *  An Interface to the Sound System for Script  
	 */
	class SoundFacade : public ISoundFacade
	{

	public:

		/*! Registers the Script functions with the given state
		*
		* @return ( void )
		*/
		static luabind::scope RegisterFunctions( );

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~SoundFacade( ) { };


		/*! Default Constructor
		 *
		 *  @return ()
		 */
		SoundFacade( ISystemComponent* component )
			: m_component( component )
		{

		}


		void TriggerEvent( const std::string& eventPath );

		void KeyOutEvent( const std::string& eventPath );

		/*! Instructs the Sound System to play some music from the given event path
		*
		*  @param[in] const std::string & eventPath
		*  @return (void)
		*/
		void PlayMusic( const std::string& eventPath );

	private:

		SoundFacade( const SoundFacade & copy ) { };
		SoundFacade & operator = ( const SoundFacade & copy ) { return *this; };

		ISystemComponent* m_component;
	};
};

#endif