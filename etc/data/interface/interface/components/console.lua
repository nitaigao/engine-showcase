----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

Console = {}

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------
----------------------------------------------------------------
-- Console Functions
----------------------------------------------------------------

function Console.initialize( )

	registerEventHandler( Console.onEvent );
	
	local console = findWidget( 'console' );
	console:setVisible( false );
	
	local input = findWidget( 'console_input' );
	scriptWidget( input, 'onKeyUp', Console.onKeyUp );
	
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

	local console = findWidget( 'console' );
	console:setVisible( not console:isVisible( ) ); 

end

function Console.onKeyUp( keyCode, keyText )

	if ( keyCode == 13 ) then
	
		local input = findWidget( 'console_input' ):asEditBox( );
	
		local text = input:getText( );
		executeString( text:asString( ) );
		
		input:setText( utf( '' ) );
		
	end

end

function Console.updateConsole( message )

	local output = findWidget( 'console_output' ):asEditBox( );
	output:addText( utf( message ) );

end

Console.initialize( );