script:include( '/data/entities/scripts/base/object.lua' )

----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

TreeNode = { 
	nodes = { },
	name = 'treeNode'
}

extend( TreeNode, Object )

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------


----------------------------------------------------------------
-- TreeNode Functions
----------------------------------------------------------------

function TreeNode:addNode( node )

	table.insert( self.nodes, node )

end

function TreeNode:update( deltaMilliseconds )

	for index, node in ipairs( self.nodes ) do
	
		node:update( deltaMilliseconds )
	
	end

end
