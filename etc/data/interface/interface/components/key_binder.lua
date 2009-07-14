----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

KeyBinder = { }
message = ''

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------
----------------------------------------------------------------
-- KeyBinder Functions
----------------------------------------------------------------

function KeyBinder.initialize( )

	script:registerEventHandler( KeyBinder.onEvent )
	
	widget = ux:findWidget( 'key_binder' )
	widget:setVisible( false )
	
	ux:scriptWidget( widget, 'onKeyUp', KeyBinder.onKeyUp )
	ux:scriptWidget( widget, 'onClick', KeyBinder.onClick )
	
end

function KeyBinder.onEvent( eventName, var1 )
	
	if ( eventName == 'UI_KEYBINDER' ) then
	
		widget:setVisible( true )
		widget:setFocus( true )
		
		message = var1
	
	end

end

function KeyBinder.onHide( )

	widget:setVisible( false )

end

function KeyBinder.onKeyUp( keyCode )

	if ( widget:isVisible( ) ) then

		if ( keyCode ~= 27 ) then -- Cancel on the Escape Key
		
			input:setMessageBinding( message, 'k_' .. keyCode )
		
			script:broadcastEvent( 'UI_MAPPINGBOUND' )
		
		end
		
		KeyBinder.onHide( )
	
	end

end

function KeyBinder.onClick( mouseId )

	if ( widget:isVisible( ) ) then
		
		input:setMessageBinding( message, 'm_' .. mouseId )
		
		script:broadcastEvent( 'UI_MAPPINGBOUND' )
		
		KeyBinder.onHide( )
		
	end

end

KeyBinder.initialize( )