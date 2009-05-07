script:include( '/data/entities/scripts/entities/character.lua' )

States = {
	IDLE = 0,
	WALKING_FORWARD = 1,
	WALKING_BACKWARD = 2,
	STRAFING_LEFT = 3,
	STRAFING_RIGHT = 4,
	FIRING = 5
}

-- Movement State Machine --

MovementStateMachine = {
	framesSinceLastMoveForward = 0,
	framesSinceLastMoveBackward = 0,
	framesSinceLastStrafeLeft = 0,
	framesSinceLastStrafeRight = 0
}	

function MovementStateMachine:actorMovedForward( )

	self.framesSinceLastMoveForward = 0

end

function MovementStateMachine:actorMovedBackward( )

	self.framesSinceLastMoveBackward = 0

end

function MovementStateMachine:actorStrafedLeft( )

	self.framesSinceLastStrafeLeft = 0

end

function MovementStateMachine:actorStrafedRight( )

	self.framesSinceLastStrafeRight = 0

end

function MovementStateMachine:update( )

	self.framesSinceLastMoveForward = self.framesSinceLastMoveForward + 1
	self.framesSinceLastMoveBackward = self.framesSinceLastMoveBackward + 1
	
	self.framesSinceLastStrafeLeft = self.framesSinceLastStrafeLeft + 1
	self.framesSinceLastStrafeRight = self.framesSinceLastStrafeRight + 1

end

function MovementStateMachine:getState( )

	local state = States.IDLE
	
	local frameWait = 5

	if ( self.framesSinceLastMoveForward < frameWait ) then
	
		state = States.WALKING_FORWARD
	
	end
	
	if ( self.framesSinceLastMoveBackward < frameWait ) then
	
		state = States.WALKING_BACKWARD
	
	end
	
	if ( self.framesSinceLastStrafeLeft < frameWait ) then
	
		state = States.STRAFING_LEFT
	
	end
	
	if ( self.framesSinceLastStrafeRight < frameWait ) then
	
		state = States.STRAFING_RIGHT
	
	end
	
	return state

end

extend( MovementStateMachine, Object )

-- Attack State Machine --

AttackStateMachine = { 
	framesSinceFire = 0
}

function AttackStateMachine:update( )

	self.framesSinceFire = self.framesSinceFire + 1

end

function AttackStateMachine:actorFired( )

	self.framesSinceFire = 0

end

function AttackStateMachine:getState( )

	local state = States.IDLE

	if ( self.framesSinceFire < 10 ) then
	
		return States.FIRING
	
	end
	
	return state

end

extend( AttackStateMachine, Object )

----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Player = { 
	attackStateMachine = AttackStateMachine:new( ),
	movementStateMachine = MovementStateMachine:new( )
}

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------

----------------------------------------------------------------
-- Player Functions
----------------------------------------------------------------

function Player:initialize( )

	script:registerEventHandler( Player.onEvent )
	script:registerUpdateHandler( Player.onUpdate )

end

function Player.onUpdate( deltaMilliseconds )

	player.attackStateMachine:update( )

	if ( player.attackStateMachine:getState( ) == States.FIRING ) then
	
		-- Default Actions
		
		script:playAnimation( 'hit', false )
		
		-- Evaluation
		
		local transition = ( player.attackStateMachine:getState( ) == States.IDLE )
		
		-- Transition
		
		if ( transition ) then
		
			script:playAnimation( 'stopped', false )
		
		end
	
	end
	
	player.movementStateMachine:update( )
	
	if ( player.movementStateMachine:getState( ) == States.WALKING_FORWARD ) and ( player.attackStateMachine:getState( ) ~= States.FIRING ) then
	
		-- Default Actions
		
		script:playAnimation( 'run_forward', false )
		
		-- Evaluation
		
		-- Transition
		
		return
		
	end
	
	if ( player.movementStateMachine:getState( ) == States.WALKING_BACKWARD ) and ( player.attackStateMachine:getState( ) ~= States.FIRING ) then
	
		-- Default Actions
		
		script:playAnimation( 'run_backward', false )
		
		-- Evaluation
		
		-- Transition
		
		return
		
	end
	
	if ( player.movementStateMachine:getState( ) == States.STRAFING_RIGHT ) and ( player.attackStateMachine:getState( ) ~= States.FIRING ) then
	
		-- Default Actions
		
		script:playAnimation( 'strafe_right', false )
		
		-- Evaluation
		
		-- Transition
		
		return
		
	end
	
	if ( player.movementStateMachine:getState( ) == States.STRAFING_LEFT ) and ( player.attackStateMachine:getState( ) ~= States.FIRING ) then
	
		-- Default Actions
		
		script:playAnimation( 'stafe_left', false )
		
		-- Evaluation
		
		-- Transition
		
		return
		
	end
	
	if ( player.movementStateMachine:getState( ) == States.IDLE ) and ( player.attackStateMachine:getState( ) ~= States.FIRING ) then
	
		-- Default Actions
		
		script:playAnimation( 'stopped', false )
		
		-- Evaluation
		
		-- Transition
		
	end
	
end

function Player.onEvent( eventName, var1, var2 )

	if ( eventName == 'INPUT_MOUSE_PRESSED' ) then
	
		player:fireWeapon( )
	
	end
	
	if ( var1 == script:getName( ) ) then
	
		if ( eventName == 'ACTOR_HIT' ) then
			
			player:onHit( var2 )
		
		end
		
		if ( eventName == 'ACTOR_DEAD' ) then 
			
			--print( 'player just died' )
				
		end
		
		if ( eventName == 'ACTOR_FIRED' ) then 
		
			player.attackStateMachine:actorFired( )
		
		end
		
		if( eventName == 'ACTOR_STRAFE_RIGHT' ) then
		
			player.movementStateMachine:actorStrafedRight( )
		
		end
		
		if( eventName == 'ACTOR_STRAFE_LEFT' ) then
		
			player.movementStateMachine:actorStrafedLeft( )
		
		end
		
		if( eventName == 'ACTOR_MOVE_FORWARD' ) then
		
			player.movementStateMachine:actorMovedForward( )
		
		end
		
		if( eventName == 'ACTOR_MOVE_BACKWARD' ) then
		
			player.movementStateMachine:actorMovedBackward( )
		
		end
		
	end

end

extend( Player, Character )
player = Player:new( )
player:initialize( )