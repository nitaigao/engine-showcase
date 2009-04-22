Script:include( '/data/entities/scripts/entities/actor.lua' );
Script:include( '/data/entities/scripts/entities/weapon.lua' );

----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

assaultRifle = Weapon:create( 'assault_rifle', 10, 5 );

Character = { 
	health = 100,
	weapon = assaultRifle,
	isdead = false
}

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------

----------------------------------------------------------------
-- Character Functions
----------------------------------------------------------------

function Character:fireWeapon( )
	
	self.weapon:fire( );

end

function Character:onHit( damage )

	if ( not self.isdead ) then
	
		self.health = self.health - damage;
		
		if ( self.health <= 0 ) then
		
			self:onDie( );
		
		end
		
	end
	
end

function Character:onDie( )

	self.isdead = true;
	Script:broadcastEvent( 'ACTOR_DEAD', Script:getName( ) );
	
end

extend( Character, Actor );