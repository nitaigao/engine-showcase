Script:include( '/data/entities/scripts/base/object.lua' );

----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Weapon = { };

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------

----------------------------------------------------------------
-- Character Functions
----------------------------------------------------------------

function Weapon:create( name, damage, fireRate )
	
	weapon = self:new( );
	
	weapon.name = name;
	weapon.damage = damage;
	weapon.fireRate = fireRate;
	weapon.timeOfLastFire = Script:getTime( );
	
	return weapon;
	
end

function Weapon:getType( )

	return 'weapon';

end

function Weapon:fire( )

	local timeNow = Script:getTime( );
	
	if ( ( timeNow - self.timeOfLastFire ) >= self.fireRate ) then
		
		local results = Script:rayQuery( true, 1 );
		
		for key, value in pairs( results ) do
		
			Script:broadcastEvent( 'ACTOR_HIT', value, self.damage );
		
		end
		
		self.timeOfLastFire = timeNow;
	
	end
	
end

extend( Weapon, Object );