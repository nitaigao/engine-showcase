Script:include( '/data/entities/scripts/entities/character.lua' );

----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Marine = { }

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------

----------------------------------------------------------------
-- Marine Functions
----------------------------------------------------------------

function Marine.onEvent( eventName, var1, var2 )

	if ( var1 == Script:getName( ) ) then

		if ( eventName == 'AI_WEAPON_FIRED' ) then
		
			marine:fireWeapon( );
		
		end
	
		if ( eventName == 'ACTOR_HIT' ) then
	
			marine:onHit( var2 );
		
		end
	
	end

end

extend( Marine, Character );
marine = Marine:new( );

Script:registerEventHandler( Marine.onEvent );