script:include( '/data/entities/scripts/base/object.lua' )

script:include( '/data/entities/scripts/ai/action_randomwaypoint.lua' )

----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

IdleSequence = { 
	nodes = { },
	name = 'IdleSequence'
}

extend( IdleSequence, Object )

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------


----------------------------------------------------------------
-- IdleSequence Functions
----------------------------------------------------------------

function IdleSequence:initialize( )

	local randomWaypoint = RandomWaypointAction:new( );
	table.insert( self.nodes, randomWaypoint )

end

function IdleSequence:update( deltaMilliseconds )

	if ( ai:getPlayerDistance( ) < 20 ) then
	
		return false
		
	end

	for index, node in ipairs( self.nodes ) do
	
		if ( not node:update( deltaMilliseconds ) ) then
		
			return false
			
		end
	
	end
	
	return true

end
