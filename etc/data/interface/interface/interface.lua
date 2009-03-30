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

	--loadComponent( 'title' );
	--loadComponent( 'menu' );
	--loadComponent( 'options' );
	--loadComponent( 'pause' );
	--loadComponent( 'console' );
	
end

function Interface.onEvent( eventName )

	--if ( eventName == 'GAME_INITIALIZED' )
	
	--	print( 'GAME_INITIALIZED' .. ' has been fired' );
	
	--end

end

registerEventHandler( Interface.onEvent );

--registerEvent( EventType.GAME_INITIALIZED, Interface.initialize );