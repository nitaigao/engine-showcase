----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Title = {}

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------
----------------------------------------------------------------
-- Title Functions
----------------------------------------------------------------

function Title.initialize( )

	ux:hideMouse( );
	
	Script:registerEventHandler( Title.onEvent );

	local logo = ux:findWidget( 'logo' );
	local logoX = ( ux:getScreenWidth( ) - logo:getDimensions( ).width ) / 2;
	local logoY = ( ux:getScreenHeight( ) - logo:getDimensions( ).height ) / 2;
	logo:setPosition( logoX, logoY );

end

function Title.onEvent( eventName, val1, val2 )

	if ( eventName == 'INPUT_KEY_UP' ) then 
	
		Title.onInput( )
	
	end
	
	if ( eventName == 'INPUT_MOUSE_RELEASED' ) then
	
		Title.onInput( )
	
	end

end

function Title.onInput( )

	Script:unregisterEventHandler( Title.onEvent );

	local logo = ux:findWidget( 'title' );
	logo:setVisible( false );
	
	Script:broadcastEvent( 'UI_MAIN_MENU' ); 

end

Title.initialize( );