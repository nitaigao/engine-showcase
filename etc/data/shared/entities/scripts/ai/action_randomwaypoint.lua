script:include( '/data/entities/scripts/ai/treenode.lua' )

----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

RandomWaypointAction = { 
	inProgress = false,
	waypointQueue = { }

}

extend( RandomWaypointAction, TreeNode )

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------


----------------------------------------------------------------
-- RandomWaypointAction Functions
----------------------------------------------------------------

function RandomWaypointAction:update( deltaMilliseconds )

	if ( #self.waypointQueue < 1 ) then
		
		local waypoint = ai:findRandomWaypoint( )
		self.waypointQueue = ai:getPathTo( waypoint )
		
	end
	
	if ( ai:getWaypointCount( ) < 1 and #self.waypointQueue > 0 ) then
	
		ai:navigateTo( self.waypointQueue[ 1 ] )
		table.remove( self.waypointQueue, 1 )
		
		animation:playAnimation( 'run_forward', true )
		print( 'run_forward' )
	
	end
	
	return true

end
