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

	hideMouse( );
	
	registerEventHandler( Title.onEvent );

	local logo = findWidget( 'logo' );
	local logoX = ( getScreenWidth( ) - logo:getDimensions( ).width ) / 2;
	local logoY = ( getScreenHeight( ) - logo:getDimensions( ).height ) / 2;
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

	unregisterEventHandler( Title.onEvent );

	local logo = findWidget( 'title' );
	logo:setVisible( false );
	
	broadcastEvent( 'UI_MAIN_MENU' ); 

end

Title.initialize( );