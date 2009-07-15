script:include( '/data/entities/scripts/entities/character.lua' )

----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Player = {
	actions = { }
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

	if ( eventName == 'WORLD_LOADING_FINISHED' ) then
	
		sfx:triggerEvent( 'game/environment/ambience' )
		animation:startAnimation( 'stopped', true )
	
	end
	
	if ( var1 == script:getName( ) ) then
	
		if ( eventName == 'ACTOR_DEAD' ) then 
		
			print( 'player just died' )
				
		end
		
		if ( eventName == 'ACTOR_HIT' ) then
			
			player:onHit( var2 )
			script:broadcastEvent( 'PLAYER_HIT', player.health )
		
		end
		
		-- Attack --
		
		attackSet = false
		
		if ( eventName == '+attack_primary' ) then 
		
			player:fireWeapon( )
			animation:startAnimation( 'shoot', true )
			
			return
		
		end
		
		if ( eventName == '-attack_primary' ) then 
			
			player:stopFiringWeapon( );
			animation:stopAnimation( 'shoot' )
		
		end
		
		-- Movement --
		
		movementSet = false
		
		if( eventName == '-strafe_right' ) then
		
			animation:stopAnimation( 'strafe_right' )
			sfx:keyOutEvent( 'game/biped/running' )
		
		end
		
		if( eventName == '+strafe_right' ) then

			animation:startAnimation( 'strafe_right', true )
			sfx:triggerEvent( 'game/biped/running' )
			
			return
		
		end
		
		if( eventName == '-strafe_left' ) then
		
			animation:stopAnimation( 'strafe_left' )
			sfx:keyOutEvent( 'game/biped/running' )
		
		end
		
		if( eventName == '+strafe_left' ) then
		
			animation:startAnimation( 'strafe_left', true )
			sfx:triggerEvent( 'game/biped/running' )
			
			return
		
		end
		
		if( eventName == '-move_forward' ) then
			
			animation:stopAnimation( 'run_forward' )
			sfx:keyOutEvent( 'game/biped/running' )
		
		end
		
		if( eventName == '+move_forward' ) then
			
			animation:startAnimation( 'run_forward', true )
			sfx:triggerEvent( 'game/biped/running' )
		
		end
		
		if( eventName == '-move_backward' ) then
		
			animation:stopAnimation( 'run_backward' )
			sfx:keyOutEvent( 'game/biped/running' )
		
		end
		
		if( eventName == '+move_backward' ) then
		
			animation:startAnimation( 'run_backward', true )
			sfx:triggerEvent( 'game/biped/running' )
			
			return
		
		end
		
		
	end

end

player = Player:new( )
player:initialize( )