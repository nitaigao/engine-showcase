----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Marine = {}

States = {
	PATROL = 0,
	IDLE = 1
}

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------

state = States.IDLE;

----------------------------------------------------------------
-- Marine Functions
----------------------------------------------------------------

function Marine.initialize( )

	
end

function Marine.onEvent( eventName )


end

function Marine.update( )

	if ( state == States.IDLE ) then
	
		me:setBehavior( 'idle' );
	
	end

	if ( state == States.PATROL ) then
	
		me:setBehavior( 'walk_backward' );
		me:walkBackward( );
	
	end

end

function update( )

	Marine.update( );

end

Marine.initialize( );
