----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Statistics = { }
frameSkip = 0

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------
----------------------------------------------------------------
-- Statistics Functions
----------------------------------------------------------------

function Statistics.initialize( )
	
end

function Statistics.onUpdate( deltaMilliseconds )

	if ( frameSkip > 50 ) then
	
		Statistics.updateStatistics( )
		frameSkip = 0
	end
	
	frameSkip = frameSkip + 1

end

function Statistics.updateStatistics( )

	local fps = instrumentation:getFps( )
	local fpsStats = ux:findWidget( 'statistics_fps' )
	fpsStats:setText( 'FPS: ' .. fps )
	
	local logic = instrumentation:getRoundTime( Queues.LOGIC )
	local logicStats = ux:findWidget( 'statistics_logic' )
	logicStats:setText( 'Logic: ' .. logic .. 'ms' )
	
	local house = instrumentation:getRoundTime( Queues.HOUSE )
	local houseStats = ux:findWidget( 'statistics_house' )
	houseStats:setText( 'House: ' .. house .. 'ms' )
	
	local render = instrumentation:getRoundTime( Queues.RENDER )
	local renderStats = ux:findWidget( 'statistics_render' )
	renderStats:setText( 'Render: ' .. render .. 'ms')

end

script:registerUpdateHandler( Statistics.onUpdate )