plugin geometry SpawnPoint
	name: "SpawnPoint"
	classID: #(0x133067, 0x54374)
	category: "Game Entities"
	extends: Box

(
	tool create
	(
		on mousePoint click do
		(
			delegate.width = delegate.height = delegate.length = 1
			
			for i in selection do (
				i.name = "player"
				i.wirecolor = Green
				
				playerSettings = ogremax.GetobjectSettingsByName( "player" )
				playerSettings.UserDataClass = "components"
				playerSettings.UserData = "<components><attachGraphics>true</attachGraphics><overrideGraphicsModel>player-fps.model</overrideGraphicsModel><graphicsType>camera</graphicsType><attachInput>true</attachInput><attachPhysics>true</attachPhysics><physicsType>character</physicsType><attachScript>true</attachScript><scriptName>entities/player.lua</scriptName></components>"
			)

			case click of 
			(
				1: nodeTM.translation = gridPoint
				2: #stop
			)
		)
	)
)