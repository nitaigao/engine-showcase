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
	
	Options.setupMessageBindings( )
	
end

function Options.onEvent( eventName, var1, var2 )
	
	if ( eventName == 'UI_OPTIONS' ) then
	
		Options.onShowOptions( )
	
	end
	
	if ( eventName == 'UI_MAPPINGBOUND' ) then
	
		print( 'options reload' )
		
	end

end

function Options.onShowOptions( )
    
    local options = ux:findWidget( 'options' )
	options:setVisible( true )
	
end

function Options.onSave( )
	
	local options = ux:findWidget( 'options' )
	options:setVisible( false )

end

function Options.setupMessageBindings( )

	local keymapList = ux:findWidget( 'options_keylist' ):asMultiList( )
	ux:scriptWidget( keymapList, 'onListSelectAccept', Options.onKeyListDoubleClick )
	
	keymapList:addColumn( '', 0 ) -- stops the list from auto sorting
	keymapList:addColumn( 'Action', 280 )
	keymapList:addColumn( 'Key Mapping', 200 )
	keymapList:addColumn( 'Message', 0 ) -- stores the message
	
	local messageBinding = ux:getBindingForMessage( 'attack_primary' )
	
	keymapList:addItem( '' )
	keymapList:setSubItemName( 1, 0, 'Primary Attack' )
	keymapList:setSubItemName( 2, 0, messageBinding:getText( ) )
	keymapList:setSubItemName( 3, 0, messageBinding:getMessage( ) )
	
	keymapList:addItem( '' )
	
	messageBinding = ux:getBindingForMessage( 'move_forward' )
	
	keymapList:addItem( '' )
	keymapList:setSubItemName( 1, 2, 'Move Forward' )
	keymapList:setSubItemName( 2, 2, messageBinding:getText( ) )
	keymapList:setSubItemName( 3, 2, messageBinding:getMessage( ) )
	
	messageBinding = ux:getBindingForMessage( 'move_backward' )
	
	keymapList:addItem( '' )
	keymapList:setSubItemName( 1, 3, 'Move Backwards' )
	keymapList:setSubItemName( 2, 3, messageBinding:getText( ) )
	keymapList:setSubItemName( 3, 3, messageBinding:getMessage( ) )
	
	messageBinding = ux:getBindingForMessage( 'strafe_left' )
	
	keymapList:addItem( '' )
	keymapList:setSubItemName( 1, 4, 'Strafe Left' )
	keymapList:setSubItemName( 2, 4, messageBinding:getText( ) )
	keymapList:setSubItemName( 3, 4, messageBinding:getMessage( ) )
	
	messageBinding = ux:getBindingForMessage( 'strafe_right' )
	
	keymapList:addItem( '' )
	keymapList:setSubItemName( 1, 5, 'Strafe Right' )
	keymapList:setSubItemName( 2, 5, messageBinding:getText( ) )
	keymapList:setSubItemName( 3, 5, messageBinding:getMessage( ) )
	
	messageBinding = ux:getBindingForMessage( 'jump' )
	
	keymapList:addItem( '' )
	keymapList:setSubItemName( 1, 6, 'Jump' )
	keymapList:setSubItemName( 2, 6, messageBinding:getText( ) )
	keymapList:setSubItemName( 3, 6, messageBinding:getMessage( ) )

end

function Options.onKeyListDoubleClick( index )

	local keymapList = ux:findWidget( 'options_keylist' ):asMultiList( )
	
	if ( keymapList:getSubItemName( 1, index ) ~= '' ) then -- if the item isnt a spacer
	
		print( keymapList:getSubItemName( 3, index ) )

		script:broadcastEvent( 'UI_KEYBINDER', keymapList:getSubItemName( 3, index ) )
		
	end

end

Options.initialize( )