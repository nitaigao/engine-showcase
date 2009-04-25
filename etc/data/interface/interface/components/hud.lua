----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Hud = {}

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------

gunWidth = 0;
gunHeight = 0;

----------------------------------------------------------------
-- Hud Functions
----------------------------------------------------------------

function Hud.initialize( )

	registerEventHandler( Hud.onEvent );

	--local gun = findWidget( 'gun' );
	--gunWidth = gun:getDimensions( ).width;
	--gunHeight = gun:getDimensions( ).height;
	
	--Hud.scaleGun( );
	Hud.resetPositions( );
	
	Hud.onHideHud( );
	
end

function Hud.resetPositions( )

	local crosshair = findWidget( 'crosshair' );
	local crosshairX = ( getScreenWidth( ) - crosshair:getDimensions( ).width ) / 2;
	local crosshairY = ( getScreenHeight( ) - crosshair:getDimensions( ).height ) / 2;
	crosshair:setPosition( crosshairX, crosshairY );
	
	--local gun = findWidget( 'gun' );
	
	--local centerX = ( getScreenWidth( ) - gun:getDimensions( ).width ) / 2;
	--local gunXOffset = getScreenWidth( ) / 10;
	--local gunX = centerX + gunXOffset; 
	--local gunY = ( getScreenHeight( ) - gun:getDimensions( ).height );
	--gun:setPosition( gunX, gunY );

end

function Hud.scaleGun( )

	local gun = findWidget( 'gun' );
	
	if ( getScreenWidth( ) > 600 ) then

		gun:setSize( gunWidth * 0.5, gunHeight * 0.5 );
	
	end
	
	if ( getScreenWidth( ) > 700 ) then

		gun:setSize( gunWidth * 0.75, gunHeight * 0.75 );
	
	end
	
	if ( getScreenWidth( ) > 1000 ) then

		gun:setSize( gunWidth, gunHeight );
	
	end
	
	if ( getScreenWidth( ) > 1900 ) then

		gun:setSize( gunWidth * 1.75, gunHeight * 1.75 );
	
	end
	
	if ( getScreenWidth( ) > 1900 ) then

		gun:setSize( gunWidth * 1.5, gunHeight * 1.5 );
	
	end
	
	if ( getScreenWidth( ) > 2000 ) then
	
		gun:setSize( gunWidth * 2, gunHeight * 2 );
	
	end

end

function Hud.onEvent( eventName, val1, val2 )

	if ( eventName == 'WORLD_LOADING_FINISHED' ) then
	
		Hud.onShowHud( );
	
	end
	
	if ( eventName == 'GAME_ENDED' ) then
	
		Hud.onHideHud( );
	
	end
	
	if ( eventName == 'GRAPHICS_SETTINGS_CHANGED' ) then
	
		--Hud.scaleGun( );
		Hud.resetPositions( );
	
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