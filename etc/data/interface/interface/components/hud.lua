----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Hud = {}

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------

gunWidth = 0
gunHeight = 0

----------------------------------------------------------------
-- Hud Functions
----------------------------------------------------------------

function Hud.initialize( )

	script:registerEventHandler( Hud.onEvent )

	Hud.resetPositions( )
	
	Hud.onHideHud( )
	
end

function Hud.resetPositions( )

	local crosshair = ux:findWidget( 'crosshair' )
	local crosshairX = ( ux:getScreenWidth( ) - crosshair:getDimensions( ).width ) / 2
	local crosshairY = ( ux:getScreenHeight( ) - crosshair:getDimensions( ).height ) / 2
	crosshair:setPosition( crosshairX, crosshairY )

end

function Hud.onEvent( eventName, val1, val2 )

	if ( eventName == 'WORLD_LOADING_FINISHED' ) then
	
		Hud.onShowHud( )
	
	end
	
	if ( eventName == 'GAME_ENDED' ) then
	
		Hud.onHideHud( )
	
	end
	
	if ( eventName == 'GRAPHICS_SETTINGS_CHANGED' ) then
	
		Hud.resetPositions( )
	
	end

end

function Hud.onShowHud( )

	local hud = ux:findWidget( 'hud' )
	hud:setVisible( true )

end

function Hud.onHideHud( )

	local hud = ux:findWidget( 'hud' )
	hud:setVisible( false )

end

Hud.initialize( )