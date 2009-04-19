----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Marine = {}

States = {
	PATROL = 0,
	IDLE = 1,
	FOLLOW = 2,
	ATTACK = 3
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

function Marine.onEvent( eventName )


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
	
	------ Attack ------
	
	if ( attackState == States.ATTACK ) then
	
		-- sense
		
		if ( me:getPlayerDistance( ) > attackDistance ) then

			attackState = States.IDLE;
		
		end
		
		-- memory
		
		-- analysis
	
		-- output
	
	end
	
	if ( attackState == States.IDLE ) then
	
		-- sense
		
		if ( me:getPlayerDistance( ) <= attackDistance ) then

			attackState = States.ATTACK;
			me:setBehavior( 'shoot_small' );
		
		end
		
		-- memory
		
		-- analysis
	
		-- output

	end

end

function update( )

	Marine.update( );

end

Marine.initialize( );
