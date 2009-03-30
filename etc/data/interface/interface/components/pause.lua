----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Pause = {}

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------
----------------------------------------------------------------
-- Pause Functions
----------------------------------------------------------------

function Pause.initialize( )

	registerEvent( EventType.UI_PAUSE_MENU, Pause.onShowPause );
	
	local pause = findWidget( 'pause' );
	pause:setVisible( false );

end

function Pause.onShowPause( )

	showMouse( );

	local pause = findWidget( 'pause' );
	pause:setVisible( true ); 
	
	local quitButton = findWidget( 'pause_button_quit' );
	scriptWidget( quitButton, 'onRelease', Pause.onQuitReleased );

end

function Pause.onQuitReleased( )

	quit( );

end

Pause.initialize( );