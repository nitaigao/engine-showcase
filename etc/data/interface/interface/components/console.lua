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

	registerEvent( EventType.UI_CONSOLE, Console.onShowConsole );
	registerEvent( EventType.LOG_MESSAGE_APPENDED, Console.onMessageLogged );
	
	local console = findWidget( 'console' );
	console:setVisible( false );
	
	local input = findWidget( 'console_input' );
	scriptWidget( input, 'onKeyUp', Console.onKeyUp );
	
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

function Console.onMessageLogged( message )

	-- Its not a good idea to use print( ) 
	-- here as that would send us into a loop

	local output = findWidget( 'console_output' );
	output:setText( utf( message ) );

end

Console.initialize( );