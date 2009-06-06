script:include( '/data/entities/scripts/base/object.lua' )
script:include( '/data/entities/scripts/ai/idlesequence.lua' )
script:include( '/data/entities/scripts/ai/suspicioussequence.lua' )
script:include( '/data/entities/scripts/ai/attacksequence.lua' )

----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

BehaviorSelector = { 
	nodes = { },
	name = 'BehaviorSelector'
}

extend( BehaviorSelector, Object )

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------


----------------------------------------------------------------
-- BehaviorSelector Functions
----------------------------------------------------------------

function BehaviorSelector:initialize( )

	self.nodes = { }

	local idleBehavior = IdleSequence:new( )
	idleBehavior:initialize( )
	table.insert( self.nodes, idleBehavior )
	
	local suspiciousSequence = SuspiciousSequence:new( )
	suspiciousSequence:initialize( )
	table.insert( self.nodes, suspiciousSequence )
	
	local attackSequence = AttackSequence:new( )
	attackSequence:initialize( )
	table.insert( self.nodes, attackSequence )

end

function BehaviorSelector:update( deltaMilliseconds )

	for index, node in ipairs( self.nodes ) do
	
		if ( node:update( deltaMilliseconds ) ) then
		
			return true
			
		end
	
	end

end
