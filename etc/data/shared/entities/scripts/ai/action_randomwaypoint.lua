script:include( '/data/entities/scripts/ai/treenode.lua' )

----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

RandomWaypointAction = { 
	inProgress = false

}

extend( RandomWaypointAction, TreeNode )

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------


----------------------------------------------------------------
-- RandomWaypointAction Functions
----------------------------------------------------------------

function RandomWaypointAction:update( deltaMilliseconds )

	if ( ai:getWaypointCount( ) < 1 ) then
		
		self.inProgress = true
		
		local waypoint = ai:findRandomWaypoint( )
		ai:navigateTo( waypoint )
		ai:playAnimation( 'run_forward', true )
		
	end

end
