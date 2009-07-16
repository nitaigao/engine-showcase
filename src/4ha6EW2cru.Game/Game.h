/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   Game.h
*  @date   2009/04/25
*/
#pragma once
#ifndef GAME_H
#define GAME_H

#include "Configuration/IConfiguration.hpp"

#include "Events/IEvent.hpp"
#include "State/IWorld.hpp"
#include "State/Serilaization/IWorldLoader.hpp"

#include "Export.hpp"

/*! 
 *  The core container of the Game
 */
class Game
{

public:

	/*! Default Destructor
	 *
	 *  @return ()
	 */
	GAMEAPI ~Game( );


	/*! Default Constructor
	 *
	 *  @return ()
	 */
	GAMEAPI Game( )
		: m_isQuitting( false )
		, m_isInitialized( false )
		, m_world( 0 )
		, m_worldLoader( 0 )
	{

	}
	

	/*! Initializes the Game
	 *
	 *  @return (void)
	 */
	void GAMEAPI Initialize( );


	/*! Steps the Game forward
	 *
	 *  @param[in] float deltaMilliseconds
	 *  @return (void)
	 */
	void GAMEAPI Update( );

	
	/*! Releases the Game
	 *
	 *  @return (void)
	 */
	void GAMEAPI Release( );


	/*! Returns whether or not the Game is ready to Quit
	 *
	 *  @return (bool)
	 */
	bool GAMEAPI IsQuitting( ) const { return m_isQuitting; };

private:

	void OnGameQuit( const Events::IEvent* event );
	void OnGameLevelChanged( const Events::IEvent* event );
	void OnGameEnded( const Events::IEvent* event );

	bool m_isQuitting;
	bool m_isInitialized;

	Configuration::IConfiguration* m_configuration;
	Serialization::IWorldSerializer* m_worldLoader;
	State::IWorld* m_world;

	Game( const Game & copy ) { };
	Game & operator = ( const Game & copy ) { return *this; };

};

#endif

// EOF