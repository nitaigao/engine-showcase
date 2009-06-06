script:include( '/data/entities/scripts/base/object.lua' )

----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

SuspiciousSequence = { 
	nodes = { },
	name = 'SuspiciousSequence'
}

extend( SuspiciousSequence, Object )

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------


----------------------------------------------------------------
-- SuspiciousSequence Functions
----------------------------------------------------------------

function SuspiciousSequence:initialize( )


end

function SuspiciousSequence:update( deltaMilliseconds )

	if ( ai:inLineOfSight( ai:getPlayerPosition( ) ) ) then
	
		return false
		
	end

	for index, node in ipairs( self.nodes ) do
	
		if ( not node:update( deltaMilliseconds ) ) then
		
			return false
			
		end
	
	end
	
	return true

end
