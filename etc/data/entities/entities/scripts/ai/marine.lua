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

movementState = States.IDLE;
attackState = States.IDLE;
attackDistance = 10;

----------------------------------------------------------------
-- Marine Functions
----------------------------------------------------------------

function Marine.initialize( )

	me:setBehavior( 'idle' );
	
end

function Marine.onEvent( eventName, var1, var2 )

	if ( var1 == Script:getName( ) ) then 

		if ( eventName == 'ACTOR_HIT' ) then
		
			
		
		end
		
		if( eventName == 'ACTOR_DEAD' ) then 
		
			movementState = States.DEAD;
			attackState = States.DEAD;
			
		end
		
	end

end

function Marine.update( )
	
	------ Movement ------
	
	if ( movementState == States.ATTACK ) then
	
		-- sense
		
		-- memory
		
		-- analysis
		
		if ( me:getPlayerDistance( ) > attackDistance ) then
		
			movementState = States.IDLE;
			me:setBehavior( 'idle' );
		
		end
	
		-- output
	
		me:facePlayer( );
	
	end

	if ( movementState == States.IDLE ) then
	
		-- sense
		
		-- memory
		
		-- analysis
		
		if ( me:getPlayerDistance( ) <= attackDistance ) then
		
			movementState = States.ATTACK;
		
		end
	
		-- output
	
	end
	
	if ( movementState == States.DEAD ) then
	
		-- sense
		
		-- memory
		
		-- analysis
	
		-- output
		
		me:setBehavior( 'die_backward' );
		movementState = States.NONE;
	
	end
	
	------ Attack ------
	
	if ( attackState == States.ATTACK ) then
	
		-- sense
		
		if ( me:getPlayerDistance( ) > attackDistance ) then

			attackState = States.IDLE;
		
		end
		
		-- memory
		
		-- analysis
	
		-- output
		
		me:fireWeapon( );
	
	end
	
	if ( attackState == States.IDLE ) then
	
		-- sense
		
		if ( me:getPlayerDistance( ) <= attackDistance ) then

			attackState = States.ATTACK;
			me:setBehavior( 'hit' );
		
		end
		
		-- memory
		
		-- analysis
	
		-- output

	end
	
	if ( attackState == States.DEAD ) then
	
		attackState = States.NONE;
	
	end

end

function update( )

	Marine.update( );

end

Script:registerEventHandler( Marine.onEvent );

Marine.initialize( );
