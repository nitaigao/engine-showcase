----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Console = {}

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------

console = nil
output = nil
input = nil
console_ingame = false

----------------------------------------------------------------
-- Console Functions
----------------------------------------------------------------

function Console.initialize( )

	script:registerEventHandler( Console.onEvent )
	
	console = ux:findWidget( 'console' )
	console:setVisible( false )
	
	input = ux:findWidget( 'console_input' )
	ux:scriptWidget( input, 'onKeyUp', Console.onKeyUp )
	
	output = ux:findWidget( 'console_output' )
	
end

function Console.onEvent( eventName, val1 )

	if ( eventName == 'WORLD_LOADING_FINISHED' ) then
	
		console_ingame = true
		
	end

	if ( eventName == 'MESSAGE_LOGGED' ) then 
	
		Console.updateConsole( val1 ) 
	
	end
	
	if ( eventName == 'UI_CONSOLE' ) then
	
		Console.onShowConsole( )
	
	end

end

function Console.onShowConsole( )

	if ( Configuration.isConsole ) then
		
		if ( console:isVisible( ) ) then
		
			console:setVisible( false )
			local menu = ux:findWidget( 'menu' )
			
			if ( not menu:isVisible( ) ) then
			
				ux:hideMouse( )
				input:setFocus( false )
			
			end
			
			if ( console_ingame ) then
			
				ux:setInputAllowed( true )
				
			end
			
		else
		
			input:setFocus( true )
			console:setVisible( true ) 
			ux:showMouse( )
			ux:setInputAllowed( false )
				
		end
		
	end
	
end

function Console.onKeyUp( keyCode, keyText )

	if ( keyCode == 28 ) then
	
		local input = input:asEditBox( )
	
		local text = input:getText( )
		script:executeString( text )
		
		input:setText( '' )
		
	end

end

function Console.updateConsole( message )

	local output = output:asEditBox( )
	output:addText( message )

end

Console.initialize( )