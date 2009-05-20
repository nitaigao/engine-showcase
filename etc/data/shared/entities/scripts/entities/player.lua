script:include( '/data/entities/scripts/entities/character.lua' )

----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Player = {
	movementSet = false,
	attackSet = false
}

extend( Player, Character )

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------

----------------------------------------------------------------
-- Player Functions
----------------------------------------------------------------

function Player:initialize( )

	script:registerEventHandler( Player.onEvent )

end

function Player.onEvent( eventName, var1, var2 )
	
	if ( var1 == script:getName( ) ) then
	
		if ( eventName == 'ACTOR_DEAD' ) then 
		
			print( 'player just died' )
				
		end
		
		if ( eventName == 'ACTOR_HIT' ) then
			
			player:onHit( var2 )
		
		end
		
		if ( eventName == 'ACTOR_IDLE_ATTACK' ) then
		
			attackSet = false
			movementSet = false -- need to switch to a movement animation
			player:stopFiringWeapon( );
		
		end
		
		if ( eventName == 'ACTOR_FIRED' ) then 
		
			player:fireWeapon( )
			script:playAnimation( 'hit', true )
			attackSet = true
		
		end
		
		-- Movement --
		
		if ( eventName == 'ACTOR_IDLE_MOVEMENT' ) then
		
			movementSet = false
			
			if( not attackSet ) then
				script:playAnimation( 'stopped', false )
			end
			
			sfx:keyOutEvent( 'game/biped/running' )
			return
		
		end
		
		if( eventName == 'ACTOR_STRAFE_RIGHT' and not movementSet ) then

			if( not attackSet ) then
				script:playAnimation( 'strafe_right', true )
			end

			sfx:triggerEvent( 'game/biped/running' )
			movementSet = true
		
		end
		
		if( eventName == 'ACTOR_STRAFE_LEFT' and not movementSet ) then
		
			if( not attackSet ) then
				script:playAnimation( 'stafe_left', true )
			end

			sfx:triggerEvent( 'game/biped/running' )
			movementSet = true
		
		end
		
		if( eventName == 'ACTOR_MOVE_FORWARD' and not movementSet ) then
			
			if( not attackSet ) then
				script:playAnimation( 'run_forward', true )
			end
			
			sfx:triggerEvent( 'game/biped/running' )
			movementSet = true
		
		end
		
		if( eventName == 'ACTOR_MOVE_BACKWARD' and not movementSet ) then
		
			if( not attackSet ) then
				script:playAnimation( 'run_backward', true )
			end
			
			sfx:triggerEvent( 'game/biped/running' )
			movementSet = true
		
		end
		
	end

end

player = Player:new( )
player:initialize( )