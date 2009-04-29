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

	--sfx:playMusic( 'ux/ui/title' );

	ux:hideMouse( );
	
	Script:registerEventHandler( Title.onEvent );

	local logo = ux:findWidget( 'logo' );
	local logoX = ( ux:getScreenWidth( ) - logo:getDimensions( ).width ) / 2;
	local logoY = ( ux:getScreenHeight( ) - logo:getDimensions( ).height ) / 2;
	logo:setPosition( logoX, logoY );
	
	Script:registerUpdateHandler( Title.onShowUpdate );
	logo:setAlpha( 0 );

end

function Title.onShowUpdate( )

	local logo = ux:findWidget( 'logo' );
	local logoAlpha = logo:getAlpha( );
	
	if ( logoAlpha < 1 ) then
	
		logo:setAlpha( logoAlpha + 0.1 );
	else
	
		Script:unregisterUpdateHandler( Title.onShowUpdate );
	
	end

end

function Title.onHideUpdate( )

	local logo = ux:findWidget( 'title' );
	local logoAlpha = logo:getAlpha( );
	
	if ( logoAlpha > 0 ) then
	
		logo:setAlpha( logoAlpha - 0.1 );
	
	else
	
		Script:unregisterUpdateHandler( Title.onHideUpdate );
		Script:unregisterUpdateHandler( Title.onShowUpdate );
		Script:unregisterEventHandler( Title.onEvent );
	
		logo:setVisible( false );
		logo:setAlpha( 1 );
		
		Script:broadcastEvent( 'UI_MAIN_MENU' ); 
	
	end

end

function Title.onEvent( eventName, val1, val2 )

	if ( eventName == 'INPUT_KEY_UP' ) or ( eventName == 'INPUT_MOUSE_RELEASED' ) then 
	
		Script:registerUpdateHandler( Title.onHideUpdate );
	
	end

end

Title.initialize( );