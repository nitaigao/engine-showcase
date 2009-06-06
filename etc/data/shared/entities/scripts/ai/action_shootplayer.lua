script:include( '/data/entities/scripts/ai/treenode.lua' )

----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

ShootPlayerAction = { 
	inProgress = false,
	waypointQueue = { }

}

extend( ShootPlayerAction, TreeNode )

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------


----------------------------------------------------------------
-- ShootPlayerAction Functions
----------------------------------------------------------------

function ShootPlayerAction:update( deltaMilliseconds )

	ai:facePlayer( ai:getPlayerPosition( ) )
	ai:fireWeapon( )
	
	return true

end
