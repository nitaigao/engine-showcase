function Should_Call_Function( )

	shouldCallFunction( );

end

function Should_Recieve_Event_Setup( sender )

	addEventListener( sender, EventType.TEST_EVENT, onReceiveEvent );

end

function onReceiveEvent( sender )

	handledEvent( );

end

function testFunction( ) 


end

function killerFunction( )

	error( "Test LUA Error" );

end
