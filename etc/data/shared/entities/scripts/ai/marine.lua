script:include( '/data/entities/scripts/base/object.lua' )

script:include( '/data/entities/scripts/ai/behaviorselector.lua' )

----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Marine = { 
	behaviorTree = BehaviorSelector:new( )
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
	
		marine.behaviorTree:initialize( )
	
	end
	
	if ( var1 == ai:getName( ) ) then
		
		if ( eventName == 'ACTOR_DEAD' ) then
		
			script:unregisterEventHandler( Marine.onEvent )
			script:unregisterUpdateHandler( Marine.onUpdate )
			
		end
	
	end

end

function Marine.onUpdate( deltaMilliseconds )

	marine.behaviorTree:update( deltaMilliseconds )

end

marine = Marine:new( )
marine:initialize( )
