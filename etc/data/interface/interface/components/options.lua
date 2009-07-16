----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Options = { }

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------
----------------------------------------------------------------
-- Options Functions
----------------------------------------------------------------


function Options.initialize( )

	script:registerEventHandler( Options.onEvent )
	
	local options = ux:findWidget( 'options' )
	options:setVisible( false )
	
	local saveButton = ux:findWidget( 'options_button_save' )
	ux:scriptWidget( saveButton, 'onRelease', Options.onSave )
	
	Options.initializeKeyboard( )
	Options.initializeMouse( )
	Options.initializeGraphics( )
	Options.initializeSound( )
	Options.initializeAdvanced( )
	
end

function Options.onEvent( eventName, var1, var2 )
	
	if ( eventName == 'UI_OPTIONS' ) then
	
		Options.onShowOptions( )
	
	end
	
	if ( eventName == 'UI_MAPPINGBOUND' ) then
	
		Options.setupMessageBindings( )
		
	end

end

function Options.onShowOptions( )
    
    local options = ux:findWidget( 'options' )
	options:setVisible( true )
	
end

function Options.onSave( )

	Options.saveInput( )
	Options.saveAdvanced( )
	Options.saveGraphics( )
	
	local options = ux:findWidget( 'options' )
	options:setVisible( false )

end

-- Sound --

function Options.initializeSound( )

	local sfxVolume = ux:findWidget( 'options_sound_sfx' ):asScrollbar( )
	ux:scriptWidget( sfxVolume, 'onScrollChangePosition', Options.onSFXVolumeScroll )
	sfxVolume:setScrollPosition( Configuration.sfxVolume )
	
	local musicVolume = ux:findWidget( 'options_sound_music' ):asScrollbar( )
	ux:scriptWidget( musicVolume, 'onScrollChangePosition', Options.onMusicVolumeScroll )
	musicVolume:setScrollPosition( Configuration.musicVolume )

end

function Options.onSFXVolumeScroll( position )

	local sfxVolume = ux:findWidget( 'options_sound_sfx' ):asScrollbar( )
	Configuration.sfxVolume = sfxVolume:getScrollPosition( )
	
end

function Options.onMusicVolumeScroll( position )

	local musicVolume = ux:findWidget( 'options_sound_music' ):asScrollbar( )
	Configuration.musicVolume = musicVolume:getScrollPosition( )

end

-- Mouse --

function Options.initializeMouse( )

	local invertYButton = ux:findWidget( 'options_mouse_inverty' )
	ux:scriptWidget( invertYButton, 'onRelease', Options.onInvertY )
	
	invertYButton = invertYButton:asButton( )
	invertYButton:setChecked( Configuration.isInvertY )
	
	local smoothButton = ux:findWidget( 'options_mouse_smooth' )
	ux:scriptWidget( smoothButton, 'onRelease', Options.onSmoothMouse )
	
	smoothButton = smoothButton:asButton( )
	smoothButton:setChecked( Configuration.isSmoothMouse )
	
	local smoothAmount = ux:findWidget( 'options_mouse_smoothamount' ):asScrollbar( )
	smoothAmount:setScrollPosition( Configuration.mouseSmoothAmount )

end

function Options.saveInput( )

	local invertYButton = ux:findWidget( 'options_mouse_inverty' ):asButton( )
	
	if not ( invertYButton:getChecked( ) and Configuration.isInvertY ) then
	
		Configuration.isInvertY = invertYButton:getChecked( )
	
	end
	
	local smoothButton = ux:findWidget( 'options_mouse_smooth' ):asButton( )
	
	if not ( smoothButton:getChecked( ) and Configuration.isSmoothMouse ) then
	
		Configuration.isSmoothMouse = smoothButton:getChecked( )
	
	end
	
	local smoothAmount = ux:findWidget( 'options_mouse_smoothamount' ):asScrollbar( )
	Configuration.mouseSmoothAmount = smoothAmount:getScrollPosition( )

end

function Options.onInvertY( )

	local invertYButton = ux:findWidget( 'options_mouse_inverty' ):asButton( )
	invertYButton:setChecked( not invertYButton:getChecked( ) )

end

function Options.onSmoothMouse( )

	local smoothButton = ux:findWidget( 'options_mouse_smooth' ):asButton( )
	smoothButton:setChecked( not smoothButton:getChecked( ) )

end

-- Advanced --

function Options.initializeAdvanced( )

	local consoleButton = ux:findWidget( 'options_console_checkbox' )
	ux:scriptWidget( consoleButton, 'onRelease', Options.onConsole )
	
	consoleButton = consoleButton:asButton( )
	consoleButton:setChecked( Configuration.isConsole )

end

function Options.saveAdvanced( )

	local consoleButton = ux:findWidget( 'options_console_checkbox' ):asButton( )
	
	if not ( consoleButton:getChecked( ) and Configuration.isConsole ) then
	
		Configuration.isConsole = consoleButton:getChecked( )
	
	end

end

function Options.onConsole( )

	local consoleButton = ux:findWidget( 'options_console_checkbox' ):asButton( )
	consoleButton:setChecked( not consoleButton:getChecked( ) )

end

-- Graphics --

function Options.initializeGraphics( )

	local fullScreenButton = ux:findWidget( 'options_fullscreen_checkbox' )
	ux:scriptWidget( fullScreenButton, 'onRelease', Options.onFullScreen )
	
	fullScreenButton = fullScreenButton:asButton( )
	fullScreenButton:setChecked( Configuration.isFullScreen ) 
	
	local resolutions = ux:findWidget( 'options_resolution_combo' ):asComboBox( )
    
	local index = 0    
    local selectedIndex = 0
	
	local supportedResolutions = ux:getSupportedResolutions( )
	
	for key, value in pairs( supportedResolutions ) do
	
		resolutions:addItem( value, value )

		local currentResolution = Configuration.displayWidth .. ' x ' .. Configuration.displayHeight
		
		if currentResolution == value then
		
           selectedIndex = index
			
		end 
		
		index = index + 1
        
	end
    
    resolutions:setSelectedIndex( selectedIndex )

end

function Options.saveGraphics( )

	local resolutionCombo = ux:findWidget( 'options_resolution_combo' ):asComboBox( )
	local selectedIndex = resolutionCombo:getSelectedIndex( )
	local selectedResolution = resolutionCombo:getValueAt( selectedIndex )
	
	local displayWidth = string.sub( selectedResolution, 0, string.find( selectedResolution, 'x' ) - 2 )
	local displayHeight = string.sub( selectedResolution, string.find( selectedResolution, 'x' ) + 2 )
	
	Configuration.displayWidth = tonumber( displayWidth )
	Configuration.displayHeight = tonumber( displayHeight )
	
	-- Full Screen --
	
	local fullScreenButton = ux:findWidget( 'options_fullscreen_checkbox' ):asButton( )
	
	if not ( fullScreenButton:getChecked( ) and Configuration.isFullScreen ) then
		
		Configuration.isFullScreen = fullScreenButton:getChecked( )
		
	end
	
	ux:changeResolution( tonumber( displayWidth ), tonumber( displayHeight ), fullScreenButton:getChecked( ) )

end

function Options.onFullScreen( )

	local fullScreenButton = ux:findWidget( 'options_fullscreen_checkbox' ):asButton( )
	fullScreenButton:setChecked( not fullScreenButton:getChecked( ) )

end

-- Keyboard --

function Options.initializeKeyboard( )

	local keymapList = ux:findWidget( 'options_keylist' ):asMultiList( )
	ux:scriptWidget( keymapList, 'onListSelectAccept', Options.onKeyListDoubleClick )
	
	keymapList:addColumn( '', 0 ) -- stops the list from auto sorting
	keymapList:addColumn( 'Action', 280 )
	keymapList:addColumn( 'Key Mapping', 200 )
	keymapList:addColumn( 'Message', 0 ) -- stores the message
	
	Options.setupMessageBindings( )
	
end

function Options.setupMessageBindings( )

	input:setDefaultMessageBinding( '+move_forward', 'k_17' )
	input:setDefaultMessageBinding( '+move_backward', 'k_31' )
	input:setDefaultMessageBinding( '+strafe_left', 'k_30' )
	input:setDefaultMessageBinding( '+strafe_right', 'k_32' )
	input:setDefaultMessageBinding( '+attack_primary', 'm_0' )
	input:setDefaultMessageBinding( 'jump', 'k_57' )

	local keymapList = ux:findWidget( 'options_keylist' ):asMultiList( )
	keymapList:removeAllItems( )
	
	keymapList:addItem( '' )
	keymapList:setSubItemName( 1, 0, 'Primary Attack' )
	keymapList:setSubItemName( 2, 0, input:getTextForMessage( '+attack_primary' ) )
	keymapList:setSubItemName( 3, 0, '+attack_primary' )
	
	keymapList:addItem( '' )
	
	keymapList:addItem( '' )
	keymapList:setSubItemName( 1, 2, 'Move Forward' )
	keymapList:setSubItemName( 2, 2, input:getTextForMessage( '+move_forward' ) )
	keymapList:setSubItemName( 3, 2, '+move_forward' )
	
	keymapList:addItem( '' )
	keymapList:setSubItemName( 1, 3, 'Move Backwards' )
	keymapList:setSubItemName( 2, 3, input:getTextForMessage( '+move_backward' ) )
	keymapList:setSubItemName( 3, 3, '+move_backward' )
	
	keymapList:addItem( '' )
	keymapList:setSubItemName( 1, 4, 'Strafe Left' )
	keymapList:setSubItemName( 2, 4, input:getTextForMessage( '+strafe_left' ) )
	keymapList:setSubItemName( 3, 4, '+strafe_left' )
	
	keymapList:addItem( '' )
	keymapList:setSubItemName( 1, 5, 'Strafe Right' )
	keymapList:setSubItemName( 2, 5, input:getTextForMessage( '+strafe_right' ) )
	keymapList:setSubItemName( 3, 5, '+strafe_right' )
	
	keymapList:addItem( '' )
	keymapList:setSubItemName( 1, 6, 'Jump' )
	keymapList:setSubItemName( 2, 6, input:getTextForMessage( 'jump' ) )
	keymapList:setSubItemName( 3, 6, 'jump' )

end

function Options.onKeyListDoubleClick( index )

	local keymapList = ux:findWidget( 'options_keylist' ):asMultiList( )
	
	if ( keymapList:getSubItemName( 1, index ) ~= '' ) then -- if the item isnt a spacer

		script:broadcastEvent( 'UI_KEYBINDER', keymapList:getSubItemName( 3, index ) )
		
	end

end

Options.initialize( )