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

	print( 'Loading Interface' );

	--unregisterEvent( EventType.GAME_INITIALIZED, Interface.initialize );

	loadComponent( 'title' );
	loadComponent( 'menu' );
	loadComponent( 'options' );
	loadComponent( 'console' );
	loadComponent( 'loading' );
	loadComponent( 'hud' );
	loadComponent( 'statistics' );
	
end

function Interface.onEvent( eventName )

	if ( eventName == 'GAME_INITIALIZED' ) then
	
		Interface.initialize( );
	
	end

end

registerEventHandler( Interface.onEvent );