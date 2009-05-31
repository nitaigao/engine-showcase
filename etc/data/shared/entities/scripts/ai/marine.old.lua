----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Marine = {}

States = {
	NONE = 0,
	PATROL = 1,
	IDLE = 2,
	FOLLOW = 3,
	ATTACK = 4,
	HIT = 5,
	DEAD = 6
}

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------

movementState = States.IDLE
attackState = States.IDLE
attackDistance = 10

----------------------------------------------------------------
-- Marine Functions
----------------------------------------------------------------

function Marine.initialize( )

	script:registerEventHandler( Marine.onEvent )
	
end

function Marine.onEvent( eventName, var1, var2 )
			
	if ( var1 == ai:getName( ) ) then 

		if ( eventName == 'ACTOR_HIT' ) then
		
		
		end
		
		if( eventName == 'ACTOR_DEAD' ) then 
	
			movementState = States.DEAD
			attackState = States.DEAD
			
		end
		
	end

end

function Marine.onUpdate( deltaMilliseconds )

	local playerDistance = ai:getPlayerDistance( );
	
	------ Movement ------
	
	if ( movementState == States.ATTACK ) then
	
		-- Default Actions
	
		ai:facePlayer( )
		
		-- Evaluation
		
		local transition = ( playerDistance > attackDistance )
		
		-- Transition
		
		if ( transition ) then
		
			movementState = States.IDLE
		
		end
	
	end
	
	if ( movementState == States.IDLE ) then
	
		-- Default Actions
		
		--ai:playAnimation( 'idle', false )
		
		-- Evaluation
		
		local transition = ( playerDistance <= attackDistance )
		
		-- Transition
		
		if ( transition ) then
		
			movementState = States.ATTACK
		
		end
	
	end
	
	if ( movementState == States.DEAD ) then
	
		-- Default Actions
		
		-- Evaluation
		
		-- Transition
		
		script:broadcastEvent( 'ACTOR_DEAD', ai:getName( ) )
		script:unregisterEventHandler( Marine.onEvent )
		movementState = States.NONE
	
	end
	
	------ Attack ------
	
	if ( attackState == States.ATTACK ) then
	
		-- Default Actions
		
		script:broadcastEvent( 'WEAPON_FIRED', ai:getName( ) )
		
		-- Evaluation
		
		local transition = ( playerDistance > attackDistance )
		
		-- Transition
		
		if ( transition ) then
		
			script:broadcastEvent( 'WEAPON_STOPPED', ai:getName( ) )
			attackState = States.IDLE
		
		end
	
	end
	
	if ( attackState == States.IDLE ) then
	
		-- Default Actions
		
		-- Evaluation
		
		local transition = ( playerDistance <= attackDistance )
		
		-- Transition
		
		if ( transition ) then
		
			attackState = States.ATTACK
		
		end
	
	end
	
	if ( attackState == States.DEAD ) then
	
		-- Default Actions
		
		attackState = States.NONE
		
		-- Evaluation
		
		-- Transition
	
	end

end

function update( deltaMilliseconds )

	Marine.onUpdate( deltaMilliseconds )

end

Marine.initialize( )
