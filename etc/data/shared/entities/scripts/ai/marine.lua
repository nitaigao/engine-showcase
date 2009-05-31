script:include( '/data/entities/scripts/base/object.lua' )
script:include( '/data/entities/scripts/ai/treenode.lua' )
script:include( '/data/entities/scripts/ai/action_randomwaypoint.lua' )


----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Marine = { 
	behaviorTree = TreeNode:new( )
}

extend( Marine, Object )

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------



----------------------------------------------------------------
-- Marine Functions
----------------------------------------------------------------

function Marine:initialize( )
	
	script:registerUpdateHandler( Marine.onUpdate )
	script:registerEventHandler( Marine.onEvent )
	
end

function Marine.onEvent( eventName, var1, var2 )
			
	if ( eventName == 'WORLD_LOADING_FINISHED' ) then
	
		local actionRandomWaypoint = RandomWaypointAction:new( )
	
		local patrolSubBehavior = TreeNode:new( )
		patrolSubBehavior.nodes = { }
		patrolSubBehavior:addNode( actionRandomWaypoint )
		
		local idleBehavior = TreeNode:new( )
		idleBehavior.nodes = { }
		idleBehavior:addNode( patrolSubBehavior )
		
		local relaxedState = TreeNode:new( )
		relaxedState.nodes = { }
		relaxedState:addNode( idleBehavior )
		
		marine.behaviorTree.nodes = { }
		marine.behaviorTree:addNode( relaxedState )
	
	end

end

function Marine.onUpdate( deltaMilliseconds )

	marine.behaviorTree:update( deltaMilliseconds )

end

marine = Marine:new( )
marine:initialize( )
