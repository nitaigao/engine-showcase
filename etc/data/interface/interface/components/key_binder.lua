----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

KeyBinder = { }

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------
----------------------------------------------------------------
-- KeyBinder Functions
----------------------------------------------------------------

function KeyBinder.initialize( )

	script:registerEventHandler( KeyBinder.onEvent )
	
	local widget = ux:findWidget( 'key_binder' )
	ux:scriptWidget( widget, 'onKeyUp', KeyBinder.onKeyUp )
	ux:scriptWidget( widget, 'onClick', KeyBinder.onClick )
	
	widget:setVisible( false )
	
end

function KeyBinder.onEvent( eventName )
	
	if ( eventName == 'UI_KEYBINDER' ) then
	
		local widget = ux:findWidget( 'key_binder' )
		widget:setVisible( true )
		widget:setFocus( true )
	
	end

end

function KeyBinder.onHide( )

	local widget = ux:findWidget( 'key_binder' )
	widget:setVisible( false )

end

function KeyBinder.onKeyUp( keyCode )

	if ( keyCode ~= 27 ) then -- Cancel on the Escape Key
	
		script:broadcastEvent( 'UI_MAPPINGBOUND' )
	
	end
	
	KeyBinder.onHide( )

end

function KeyBinder.onClick( mouseId )
	
	script:broadcastEvent( 'UI_MAPPINGBOUND' )
	
	KeyBinder.onHide( )

end

KeyBinder.initialize( )