----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Options = {}

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------
----------------------------------------------------------------
-- Options Functions
----------------------------------------------------------------

function Options.initialize( )

	registerEventHandler( Options.onEvent );
	
	local options = findWidget( 'options' );
	options:setVisible( false );
	
	local saveButton = findWidget( 'options_button_save' );
	scriptWidget( saveButton, 'onRelease', Options.onSave );
	
	local consoleButton = findWidget( 'options_console_checkbox' );
	scriptWidget( consoleButton, 'onRelease', Options.onConsole );
	
	consoleButton = consoleButton:asButton( );
	consoleButton:setChecked( Configuration.isConsole );
	
	local fullScreenButton = findWidget( 'options_fullscreen_checkbox' );
	scriptWidget( fullScreenButton, 'onRelease', Options.onFullScreen );
	
	fullScreenButton = fullScreenButton:asButton( );
	fullScreenButton:setChecked( Configuration.isFullScreen ); 
	
	local resolutions = findWidget( 'options_resolution_combo' ):asComboBox( );
    
	local index = 0;    
    local selectedIndex = 0;
	for mode in Configuration.availableVideoModes do
	
		local currentResolution = Configuration.displayWidth .. ' x ' .. Configuration.displayHeight;
	
		resolutions:addItem( mode, mode );
		
		if currentResolution == mode  then
		
            selectedIndex = index;
			
		end 
		
		index = index + 1;
        
	end
    
    resolutions:setSelectedIndex( selectedIndex );
	
end

function Options.onEvent( eventName )

	if ( eventName == 'GRAPHICS_SETTINGS_CHANGED' ) then 
	
		Options.onGraphicsSettingsChanged( ); 
	
	end
	
	if ( eventName == 'UI_OPTIONS' ) then
	
		Options.onShowOptions( )
	
	end

end

function Options.setPosition( )

    local options = findWidget( 'options' );
	local optionsX = ( getScreenWidth( ) - options:getDimensions( ).width ) / 2;
	local optionsY = ( getScreenHeight( ) - options:getDimensions( ).height ) / 2;
	options:setPosition( optionsX, optionsY );

end

function Options.onGraphicsSettingsChanged( )

    Options.setPosition( );

end

function Options.onShowOptions( )

    Options.setPosition( );
    
    local options = findWidget( 'options' );
	options:setVisible( true );
	
end

function Options.onSave( )
	
	-- Resolution --
	
	local resolutionCombo = findWidget( 'options_resolution_combo' ):asComboBox( );
	local selectedIndex = resolutionCombo:getSelectedIndex( )
	local selectedResolution = resolutionCombo:getValueAt( selectedIndex );
	
	local displayWidth = string.sub( selectedResolution, 0, string.find( selectedResolution, 'x' ) - 2 );
	local displayHeight = string.sub( selectedResolution, string.find( selectedResolution, 'x' ) + 2 );
	
	Configuration.displayWidth = tonumber( displayWidth );
	Configuration.displayHeight = tonumber( displayHeight );
	
	-- Full Screen --
	
	local fullScreenButton = findWidget( 'options_fullscreen_checkbox' ):asButton( );
	
	if not ( fullScreenButton:getChecked( ) and Configuration.isFullScreen ) then
		
		Configuration.isFullScreen = fullScreenButton:getChecked( );
		
	end
	
	changeResolution( );
	
	-- Console --
	
	local consoleButton = findWidget( 'options_console_checkbox' ):asButton( );
	
	if not ( consoleButton:getChecked( ) and Configuration.isConsole ) then
	
		Configuration.isConsole = consoleButton:getChecked( );
	
	end

	local options = findWidget( 'options' );
	options:setVisible( false );

end

function Options.onFullScreen( )

	local fullScreenButton = findWidget( 'options_fullscreen_checkbox' ):asButton( );
	fullScreenButton:setChecked( not fullScreenButton:getChecked( ) );

end

function Options.onConsole( )

	local consoleButton = findWidget( 'options_console_checkbox' ):asButton( );
	consoleButton:setChecked( not consoleButton:getChecked( ) );

end

Options.initialize( );