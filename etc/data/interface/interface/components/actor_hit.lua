----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

ActorHit = { }
alpha = 0
timeLastHit = 0
fadeInterval = 300
fadeAmount = 0.01
visibleThreshold = 30

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------
----------------------------------------------------------------
-- ActorHit Functions
----------------------------------------------------------------

function ActorHit.initialize( )

	script:registerEventHandler( ActorHit.onEvent )
	script:registerUpdateHandler( ActorHit.onUpdate )
	
	local widget = ux:findWidget( 'actor_hit' )
	widget:setAlpha( 0 )

end

function ActorHit.onUpdate( )

	if ( alpha > 0 ) then
	
		if ( script:getTime( ) - timeLastHit > fadeInterval ) then
		
			alpha = alpha - fadeAmount;
			
			if ( alpha < 0 ) then
				
				alpha = 0
				
			end
			
			local widget = ux:findWidget( 'actor_hit' )
			widget:setAlpha( alpha )
			
		end
		
	end

end

function ActorHit.onEvent( eventName, var1, var2 )

	if ( eventName == 'PLAYER_HIT' ) then
	
		if ( var1 < visibleThreshold ) then

			timeLastHit = script:getTime( )
			
			alpha = 1 - ( var1 / visibleThreshold ) 

			local widget = ux:findWidget( 'actor_hit' )
			widget:setAlpha( alpha )
			
		end
		
	end

end

ActorHit.initialize( )