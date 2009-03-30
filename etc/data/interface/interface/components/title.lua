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

	registerEvent( EventType.INPUT_KEY_UP, Title.onInput );
	registerEvent( EventType.INPUT_MOUSE_RELEASED, Title.onInput );

	local logo = findWidget( 'logo' );
	local logoX = ( getScreenWidth( ) - logo:getDimensions( ).width ) / 2;
	local logoY = ( getScreenHeight( ) - logo:getDimensions( ).height ) / 2;
	logo:setPosition( logoX, logoY );

end

function Title.onInput( )

	unregisterEvent( EventType.INPUT_KEY_UP, Title.onInput );
	unregisterEvent( EventType.INPUT_MOUSE_RELEASED, Title.onInput );

	local logo = findWidget( 'logo' );
	logo:setVisible( false );
	
	broadcastEvent( EventType.UI_MAIN_MENU ); 

end

Title.initialize( );