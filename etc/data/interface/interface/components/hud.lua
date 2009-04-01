----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Hud = {}

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------
----------------------------------------------------------------
-- Hud Functions
----------------------------------------------------------------

function Hud.initialize( )

	registerEventHandler( Hud.onEvent );
	Hud.onHideHud( );
	
end

function Hud.onEvent( eventName, val1, val2 )

	if ( eventName == 'WORLD_LOADING_FINISHED' ) then
	
		Hud.onShowHud( );
	
	end
	
	if ( eventName == 'GAME_ENDED' ) then
	
		Hud.onHideHud( );
	
	end

end

function Hud.onShowHud( )

	local hud = findWidget( 'hud' );
	hud:setVisible( true );

end

function Hud.onHideHud( )

	local hud = findWidget( 'hud' );
	hud:setVisible( false );

end

Hud.initialize( );