script:include( '/data/entities/scripts/base/object.lua' )

script:include( '/data/entities/scripts/ai/action_shootplayer.lua' )

----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

AttackSequence = { 
	nodes = { },
	name = 'AttackSequence'
}

extend( AttackSequence, Object )

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------


----------------------------------------------------------------
-- AttackSequence Functions
----------------------------------------------------------------

function AttackSequence:initialize( )

	local shootPlayer = ShootPlayerAction:new( );
	table.insert( self.nodes, shootPlayer )

end

function AttackSequence:update( deltaMilliseconds )

	for index, node in ipairs( self.nodes ) do
	
		if ( not node:update( deltaMilliseconds ) ) then
		
			return false
			
		end
	
	end
	
	return true

end
