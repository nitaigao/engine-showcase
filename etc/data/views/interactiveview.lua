ScreenTypes = {
	
	INTROSCREEN = 000001,
	MENUSCREEN = 000002

}

TransitionTypes = {

	CUT = 0,
	FADE = 1

}

function onGameInitialized( sender )

	--print( "HumanView onGameInitialized Fired" );
	
	onScreenChange( sender, "introscreen" );
		
end

function onScreenChange( sender, screenName )

	--print( "Screen change event received" );
	
	--if ( screenName == "introscreen" ) then
		
		changeScreen( sender, screenName, ScreenTypes.INTROSCREEN );
	
	--end
	
	--if ( screenName == "MenuScreen" ) then
	
	--	changeScreen( "MenuScreen", ScreenTypes.MENUSCREEN );
		
	--end

end
