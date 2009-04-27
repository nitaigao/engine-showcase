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

	Script:registerEventHandler( Options.onEvent );
	
	local options = ux:findWidget( 'options' );
	options:setVisible( false );
	
	local saveButton = ux:findWidget( 'options_button_save' );
	ux:scriptWidget( saveButton, 'onRelease', Options.onSave );
	
	local consoleButton = ux:findWidget( 'options_console_checkbox' );
	ux:scriptWidget( consoleButton, 'onRelease', Options.onConsole );
	
	consoleButton = consoleButton:asButton( );
	consoleButton:setChecked( Configuration.isConsole );
	
	local fullScreenButton = ux:findWidget( 'options_fullscreen_checkbox' );
	ux:scriptWidget( fullScreenButton, 'onRelease', Options.onFullScreen );
	
	fullScreenButton = fullScreenButton:asButton( );
	fullScreenButton:setChecked( Configuration.isFullScreen ); 
	
	local resolutions = ux:findWidget( 'options_resolution_combo' ):asComboBox( );
    
	local index = 0;    
    local selectedIndex = 0;
	
	local supportedResolutions = ux:getSupportedResolutions( );
	
	for key, value in pairs( supportedResolutions ) do

		local currentResolution = Configuration.displayWidth .. ' x ' .. Configuration.displayHeight;
	
		resolutions:addItem( value, value );
		
		if currentResolution == value  then
		
            selectedIndex = index;
			
		end 
		
		index = index + 1;
        
	end
    
    resolutions:setSelectedIndex( selectedIndex );
	
end

function Options.onEvent( eventName )
	
	if ( eventName == 'UI_OPTIONS' ) then
	
		Options.onShowOptions( )
	
	end

end

function Options.onShowOptions( )
    
    local options = ux:findWidget( 'options' );
	options:setVisible( true );
	
end

function Options.onSave( )
	
	-- Resolution --
	
	local resolutionCombo = ux:findWidget( 'options_resolution_combo' ):asComboBox( );
	local selectedIndex = resolutionCombo:getSelectedIndex( )
	local selectedResolution = resolutionCombo:getValueAt( selectedIndex );
	
	local displayWidth = string.sub( selectedResolution, 0, string.find( selectedResolution, 'x' ) - 2 );
	local displayHeight = string.sub( selectedResolution, string.find( selectedResolution, 'x' ) + 2 );
	
	Configuration.displayWidth = tonumber( displayWidth );
	Configuration.displayHeight = tonumber( displayHeight );
	
	-- Full Screen --
	
	local fullScreenButton = ux:findWidget( 'options_fullscreen_checkbox' ):asButton( );
	
	if not ( fullScreenButton:getChecked( ) and Configuration.isFullScreen ) then
		
		Configuration.isFullScreen = fullScreenButton:getChecked( );
		
	end
	
	ux:changeResolution( tonumber( displayWidth ), tonumber( displayHeight ), fullScreenButton:getChecked( ) );
	
	-- Console --
	
	local consoleButton = ux:findWidget( 'options_console_checkbox' ):asButton( );
	
	if not ( consoleButton:getChecked( ) and Configuration.isConsole ) then
	
		Configuration.isConsole = consoleButton:getChecked( );
	
	end

	local options = ux:findWidget( 'options' );
	options:setVisible( false );

end

function Options.onFullScreen( )

	local fullScreenButton = ux:findWidget( 'options_fullscreen_checkbox' ):asButton( );
	fullScreenButton:setChecked( not fullScreenButton:getChecked( ) );

end

function Options.onConsole( )

	local consoleButton = ux:findWidget( 'options_console_checkbox' ):asButton( );
	consoleButton:setChecked( not consoleButton:getChecked( ) );

end

Options.initialize( );