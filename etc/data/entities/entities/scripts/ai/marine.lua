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
	
end

function Marine.onEvent( eventName, var1, var2 )

	if ( var1 == script:getName( ) ) then 

		if ( eventName == 'ACTOR_HIT' ) then
		
		end
		
		if( eventName == 'ACTOR_DEAD' ) then 
		
			movementState = States.DEAD
			attackState = States.DEAD
			
		end
		
	end

end

function Marine.onUpdate( deltaMilliseconds )
	
	------ Movement ------
	
	if ( movementState == States.ATTACK ) then
	
		-- Default Actions
	
		ai:facePlayer( )
		
		-- Evaluation
		
		local transition = ( ai:getPlayerDistance( ) > attackDistance )
		
		-- Transition
		
		if ( transition ) then
		
			movementState = States.IDLE
		
		end
	
	end
	
	if ( movementState == States.IDLE ) then
	
		-- Default Actions
		
		ai:playAnimation( 'idle', false )
		
		-- Evaluation
		
		local transition = ( ai:getPlayerDistance( ) <= attackDistance )
		
		-- Transition
		
		if ( transition ) then
		
			movementState = States.ATTACK
		
		end
	
	end
	
	if ( movementState == States.DEAD ) then
	
		-- Default Actions
		
		ai:playAnimation( 'die_backward', false )
		movementState = States.NONE
		
		-- Evaluation
		
		-- Transition
	
	end
	
	------ Attack ------
	
	if ( attackState == States.ATTACK ) then
	
		-- Default Actions
		
		ai:fireWeapon( )
		ai:playAnimation( 'hit', false )
		
		-- Evaluation
		
		local transition = ( ai:getPlayerDistance( ) > attackDistance )
		
		-- Transition
		
		if ( transition ) then
		
			attackState = States.IDLE
		
		end
	
	end
	
	if ( attackState == States.IDLE ) then
	
		-- Default Actions
		
		-- Evaluation
		
		local transition = ( ai:getPlayerDistance( ) <= attackDistance )
		
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

script:registerEventHandler( Marine.onEvent )
script:registerUpdateHandler( Marine.onUpdate )

Marine.initialize( )
