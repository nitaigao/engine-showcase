plugin geometry SpawnPoint
	name: "Spawn Point"
	classID: #(0x133067, 0x54374)
	category: "Game Entities"
	extends: Box

(
	tool create
	(
		on mousePoint click do
		(
			delegate.width = delegate.length = 1
			delegate.height = 0.05
			
			for i in selection do (
				i.wirecolor = Green
				
				playerSettings = ogremax.GetobjectSettingsByName( i.name )
				playerSettings.UserDataClass = "components"
				playerSettings.UserData = "<components><attachSound>false</attachSound><attachGraphics>false</attachGraphics><overrideGraphicsModel></overrideGraphicsModel><graphicsType>default</graphicsType><attachInput>false</attachInput><attachPhysics>false</attachPhysics><physicsType>default</physicsType><attachScript>true</attachScript><scriptName>entities/spawnpoint.lua</scriptName></components>"
			)

			case click of 
			(
				1: nodeTM.translation = gridPoint
				2: #stop
			)
		)
	)
)