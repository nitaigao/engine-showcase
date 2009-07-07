script:include( '/data/entities/scripts/base/object.lua' )

----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

SpawnPoint = {

}

extend( SpawnPoint, Object )

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------

----------------------------------------------------------------
-- SpawnPoint Functions
----------------------------------------------------------------

function SpawnPoint:initialize( )

	script:registerEventHandler( SpawnPoint.onEvent )

end

function SpawnPoint.onEvent( eventName, var1 )

	

end

spawnPoint = SpawnPoint:new( )
spawnPoint:initialize( )
