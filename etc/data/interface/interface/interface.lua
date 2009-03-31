----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Interface = {}

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------
----------------------------------------------------------------
-- Interface Functions
----------------------------------------------------------------

function Interface.initialize( )

	--unregisterEvent( EventType.GAME_INITIALIZED, Interface.initialize );

	loadComponent( 'title' );
	loadComponent( 'menu' );
	loadComponent( 'options' );
	loadComponent( 'pause' );
	loadComponent( 'console' );
	loadComponent( 'loading' );
	
end

function Interface.onEvent( eventName )

	if ( eventName == 'GAME_INITIALIZED' ) then
	
		Interface.initialize( );
	
	end

end

registerEventHandler( Interface.onEvent );