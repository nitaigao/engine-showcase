/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   SoundController.h
*  @date   2009/04/28
*/
#ifndef SOUNDCONTROLLER_H
#define SOUNDCONTROLLER_H

#include "ISoundController.hpp"
#include "IScriptComponent.hpp"

namespace Script
{
	/*!
	 *  An Interface to the Sound System for Script  
	 */
	class SoundController : public ISoundController
	{

	public:

		/*! Default Destructor
		 *
		 *  @return ()
		 */
		~SoundController( ) { };


		/*! Default Constructor
		 *
		 *  @return ()
		 */
		SoundController( IScriptComponent* scriptComponent )
			: m_scriptComponent( scriptComponent )
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

		SoundController( const SoundController & copy ) { };
		SoundController & operator = ( const SoundController & copy ) { return *this; };

		IScriptComponent* m_scriptComponent;

	};
};

#endif