script:include( '/data/entities/scripts/entities/actor.lua' )
script:include( '/data/entities/scripts/entities/weapon.lua' )

----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

assaultRifle = Weapon:create( 'assault_rifle', 10, 5 )

Character = { 
	health = 100,
	weapon = assaultRifle,
	isdead = false,
	eyeLevel = Vector( 0, 1.73, 0 )
}

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------

----------------------------------------------------------------
-- Character Functions
----------------------------------------------------------------

function Character:fireWeapon( )
	
	self.weapon:fireAt( script:getPosition( ) + self.eyeLevel, script:getLookAt( ) + self.eyeLevel )

end

function Character:onHit( damage )

	if ( not self.isdead ) then

		self.health = self.health - damage

		if ( self.health <= 0 ) then
		
			self:onDie( )
		
		end
		
	end
	
end

function Character:onDie( )

	self.isdead = true
	script:broadcastEvent( 'ACTOR_DEAD', script:getId( ) )
	
end

extend( Character, Actor )