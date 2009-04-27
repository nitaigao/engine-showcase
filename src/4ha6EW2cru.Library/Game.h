/*!
*  @company Black Art Studios
*  @author Nicholas Kostelnik
*  @file   Game.h
*  @date   2009/04/25
*/
#ifndef __GAME_H
#define __GAME_H

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
		: _isQuitting( false )
		, _isInitialized( false )
		, _world( 0 )
		, _worldLoader( 0 )
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
	void Update( const float& deltaMilliseconds );

	
	/*! Releases the Game
	 *
	 *  @return (void)
	 */
	void Release( );


	/*! Returns whether or not the Game is ready to Quit
	 *
	 *  @return (bool)
	 */
	inline bool IsQuitting( ) const { return _isQuitting; };

private:

	void OnGameQuit( const Events::IEvent* event );
	void OnGameLevelChanged( const Events::IEvent* event );
	void OnGameEnded( const Events::IEvent* event );

	bool _isQuitting;
	bool _isInitialized;

	Configuration::IConfiguration* _configuration;
	Serialization::IWorldSerializer* _worldLoader;
	State::IWorld* _world;

	Game( const Game & copy ) { };
	Game & operator = ( const Game & copy ) { return *this; };

};

#endif

// EOF