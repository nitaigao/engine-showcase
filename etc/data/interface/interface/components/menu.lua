----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Menu = {}
menu_ingame = false;

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------
----------------------------------------------------------------
-- Menu Functions
----------------------------------------------------------------

function Menu.initialize( )

	registerEventHandler( Menu.onEvent );

	Menu.onHideMenu( );
	
	local quitButton = findWidget( 'button_quit' );
	scriptWidget( quitButton, 'onRelease', Menu.onQuitReleased );
	
	local optionsButton = findWidget( 'button_options' );
	scriptWidget( optionsButton, 'onRelease', Menu.onOptionsReleased );
	
	local playButton = findWidget( 'button_play' );
	scriptWidget( playButton, 'onRelease', Menu.onPlayReleased );
	
	local resumeButton = findWidget( 'button_resume' );
	scriptWidget( resumeButton, 'onRelease', Menu.onResumeReleased );
	
	local endGameButton = findWidget( 'button_endgame' );
	scriptWidget( endGameButton, 'onRelease', Menu.onEndGameReleased );

	Menu.updatePosition( );
	
end

function Menu.onEvent( eventName, val1, val2 )

	if ( eventName == 'INPUT_KEY_UP' ) then 
	
		Menu.onKeyUp( val1 );
	
	end
	
	if ( eventName == 'UI_MAIN_MENU' ) then
	
		Menu.onShowMenu( )
	
	end
	
	if ( eventName == 'WORLD_LOADING_STARTED' ) then
	
		Menu.onHideMenu( );
	
	end
	
	if ( eventName == 'GRAPHICS_SETTINGS_CHANGED' ) then
	
		Menu.updatePosition( );
	
	end

end

function Menu.updatePosition( )

	local menu = findWidget( 'menubox' );
	
	local menuX = getScreenWidth( ) / 20;
	
	local halfScreenHeight = ( getScreenHeight( ) - menu:getDimensions( ).height ) / 2;
	local offsetY = halfScreenHeight / 2;
	local menuY = halfScreenHeight + offsetY;
	
	menu:setPosition( menuX, menuY );

end

function Menu.onShowMenu( )

	local menu = findWidget( 'menu' );
	menu:setVisible( true ); 
	showMouse( );

end

function Menu.onHideMenu( )

	local menu = findWidget( 'menu' );
	menu:setVisible( false );
	hideMouse( );

end

function Menu.onKeyUp( keyCode )

	if ( keyCode == '1' and menu_ingame ) then
	
		Menu.ToggleInGameMenu( );
	
	end
	
end

function Menu.ToggleInGameMenu( )
	
	local menu = findWidget( 'menu' );
	
	if ( menu:isVisible( ) ) then 
	
		local playButton = findWidget( 'button_play' );
		playButton:setVisible( true );
		
		local resumeButton = findWidget( 'button_resume' );
		resumeButton:setVisible( false );
		
		local quitButton = findWidget( 'button_quit' );
		quitButton:setVisible( true );
		
		local endGameButton = findWidget( 'button_endgame' );
		endGameButton:setVisible( false );
		
		local titleBackground = findWidget( 'background_title' );
		titleBackground:setVisible( true );
	
		setInputAllowed( true );
		Menu.onHideMenu( );
		
	else
	
		local playButton = findWidget( 'button_play' );
		playButton:setVisible( false );
		
		local resumeButton = findWidget( 'button_resume' );
		resumeButton:setVisible( true );
		
		local quitButton = findWidget( 'button_quit' );
		quitButton:setVisible( false );
		
		local endGameButton = findWidget( 'button_endgame' );
		endGameButton:setVisible( true );
		
		local titleBackground = findWidget( 'background_title' );
		titleBackground:setVisible( false );
	
		setInputAllowed( false );
		Menu.onShowMenu( );
	
	end

end

function Menu.onQuitReleased( )

	quit( );

end

function Menu.onOptionsReleased( )

	broadcastEvent( 'UI_OPTIONS' ); 

end

function Menu.onPlayReleased( )

	Menu.onHideMenu( );
	hideMouse( );
	menu_ingame = true;
	loadLevel( 'level0' );

end

function Menu.onResumeReleased( )

	Menu.ToggleInGameMenu( );

end

function Menu.onEndGameReleased( )

	Menu.ToggleInGameMenu( );
	menu_ingame = false;
	endGame( );
	Menu.onShowMenu( );

end

Menu.initialize( );