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
	
	local hud = findWidget( 'hud' );
	local hudX = ( getScreenWidth( ) - hud:getDimensions( ).width ) / 2;
	local hudY = ( getScreenHeight( ) - hud:getDimensions( ).height ) / 2;
	hud:setPosition( hudX, hudY );
	
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
	print( 'show hud' );

end

function Hud.onHideHud( )

	local hud = findWidget( 'hud' );
	hud:setVisible( false );
	print( 'hide hud' );

end

Hud.initialize( );