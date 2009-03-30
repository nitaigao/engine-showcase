----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Menu = {}

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------
----------------------------------------------------------------
-- Menu Functions
----------------------------------------------------------------

function Menu.initialize( )

	registerEvent( EventType.UI_MAIN_MENU, Menu.onShowMenu );
	
	local menu = findWidget( 'menu' );
	menu:setVisible( false );

end

function Menu.onShowMenu( )

	showMouse( );

	local menu = findWidget( 'menu' );
	menu:setVisible( true ); 
	
	local quitButton = findWidget( 'button_quit' );
	scriptWidget( quitButton, 'onRelease', Menu.onQuitReleased );
	
	local optionsButton = findWidget( 'button_options' );
	scriptWidget( optionsButton, 'onRelease', Menu.onOptionsReleased );
	
	local playButton = findWidget( 'button_play' );
	scriptWidget( playButton, 'onRelease', Menu.onPlayReleased );

end

function Menu.onQuitReleased( )

	quit( );

end

function Menu.onOptionsReleased ( )

	broadcastEvent( EventType.UI_OPTIONS ); 

end

function Menu.onPlayReleased( )

	local menu = findWidget( 'menu' );
	menu:setVisible( false ); 
	hideMouse( );
	
	loadLevel( 'level1' );

end

Menu.initialize( );