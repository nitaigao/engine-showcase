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
	
	Script:unregisterEventHandler( Interface.onEvent );

	ux:loadComponent( 'title' );
	ux:loadComponent( 'menu' );
	ux:loadComponent( 'options' );
	ux:loadComponent( 'console' );
	ux:loadComponent( 'loading' );
	ux:loadComponent( 'hud' );
	ux:loadComponent( 'statistics' );
	
end

function Interface.onEvent( eventName )

	if ( eventName == 'GAME_INITIALIZED' ) then
	
		Interface.initialize( );
	
	end

end

Script:registerEventHandler( Interface.onEvent );