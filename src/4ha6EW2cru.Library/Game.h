/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   Game.h
*  @date   2009/04/25
*/
#ifndef GAME_H
#define GAME_H

#include "System/ISystem.hpp"
#include "Configuration/IConfiguration.hpp"

#include "Events/IEvent.hpp"
#include "State/IWorld.hpp"
#include "State/Serilaization/IWorldLoader.hpp"

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
	~Game( );


	/*! Default Constructor
	 *
	 *  @return ()
	 */
	Game( )
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
	void Initialize( );


	/*! Steps the Game forward
	 *
	 *  @param[in] float deltaMilliseconds
	 *  @return (void)
	 */
	void Update( );

	
	/*! Releases the Game
	 *
	 *  @return (void)
	 */
	void Release( );


	/*! Returns whether or not the Game is ready to Quit
	 *
	 *  @return (bool)
	 */
	inline bool IsQuitting( ) const { return m_isQuitting; };

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