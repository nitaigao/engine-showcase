----------------------------------------------------------------
-- Global Variables
----------------------------------------------------------------

CharacterSelect = { }

----------------------------------------------------------------
-- Local Variables
----------------------------------------------------------------
----------------------------------------------------------------
-- CharacterSelect Functions
----------------------------------------------------------------

function CharacterSelect.initialize( )

	script:registerEventHandler( CharacterSelect.onEvent )

	CharacterSelect.onHide( )

	local marineButton = ux:findWidget( 'character_marine' )
	ux:scriptWidget( marineButton, 'onRelease', CharacterSelect.onMarineClicked )

end

function CharacterSelect.onEvent( eventName, var1 )

	if ( eventName == 'WORLD_LOADING_FINISHED' ) then
	
		CharacterSelect.onShow( )
	
	end

end

function CharacterSelect.onMarineClicked( )

	CharacterSelect.onCharacterSelected( 'marine' )

end

function CharacterSelect.onCharacterSelected( characterName )

	network:selectCharacter( 'marine' )
	CharacterSelect.onHide( )
	ux:hideMouse( )
	ux:setInputAllowed( true )
	
end

function CharacterSelect.onShow( )

	local characterSelect = ux:findWidget( 'character_select' )
	characterSelect:setVisible( true ) 
	ux:showMouse( )
	
end

function CharacterSelect.onHide( )

	local characterSelect = ux:findWidget( 'character_select' )
	characterSelect:setVisible( false ) 

end

 CharacterSelect.initialize( )