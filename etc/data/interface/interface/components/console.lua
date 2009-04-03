----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Console = {}

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------

console = nil;
output = nil;
input = nil;

----------------------------------------------------------------
-- Console Functions
----------------------------------------------------------------

function Console.initialize( )

	registerEventHandler( Console.onEvent );
	
	console = findWidget( 'console' );
	console:setVisible( false );
	
	input = findWidget( 'console_input' );
	scriptWidget( input, 'onKeyUp', Console.onKeyUp );
	
	output = findWidget( 'console_output' );
	
end

function Console.onEvent( eventName, val1 )

	if ( eventName == 'MESSAGE_LOGGED' ) then 
	
		Console.updateConsole( val1 ); 
	
	end
	
	if ( eventName == 'UI_CONSOLE' ) then
	
		Console.onShowConsole( )
	
	end

end

function Console.onShowConsole( )

	if ( Configuration.isConsole ) then
		
		if ( console:isVisible( ) ) then
		
			console:setVisible( false );
			local menu = findWidget( 'menu' );
			
			if ( not menu:isVisible( ) ) then
			
				hideMouse( );
				input:setFocus( false );
			
			end
			
			setInputAllowed( true );
			
		else
		
			input:setFocus( true );
			console:setVisible( true ); 
			showMouse( );

			setInputAllowed( false );
			
		end
		
	end
	
end

function Console.onKeyUp( keyCode, keyText )

	if ( keyCode == 13 ) then
	
		local input = input:asEditBox( );
	
		local text = input:getText( );
		executeString( text:asString( ) );
		
		input:setText( utf( '' ) );
		
	end

end

function Console.updateConsole( message )

	--local output = output:asEditBox( );
	--output:addText( utf( message ) );

end

Console.initialize( );