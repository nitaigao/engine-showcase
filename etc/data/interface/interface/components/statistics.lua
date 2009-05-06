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

	local fps = ux:getFps( )
	local stats = ux:findWidget( 'statistics' )
	stats:setText( 'fps: ' .. fps )

end

script:registerUpdateHandler( Statistics.onUpdate )