----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Statistics = {}

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------
----------------------------------------------------------------
-- Statistics Functions
----------------------------------------------------------------

function Statistics.initialize( )
	
	registerEventHandler( Statistics.onEvent );

end

function Statistics.onEvent( eventName, val1, val2 )

	Statistics.updateStatistics( );
	
end

function Statistics.updateStatistics( )

	local fps = getFps( );
	local stats = findWidget( 'statistics' );
	stats:setText( 'fps: ' .. fps );

end

Statistics.initialize( );