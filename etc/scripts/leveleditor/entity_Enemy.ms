plugin geometry Enemy
	name: "enemy_marine"
	classID: #(0x133067, 0x54375)
	category: "Game Entities"
	extends: Box
(
	tool create
	(
		on mousePoint click do
		(
			delegate.width = delegate.height = delegate.length = 0.5
			
			for i in selection do (
				i.wirecolor = Red
				
				playerSettings = ogremax.GetobjectSettingsByName( i.name )
				playerSettings.UserDataClass = "components"
				playerSettings.UserData = "<components><attachSound>true</attachSound><attachGraphics>true</attachGraphics><overrideGraphicsModel>marine.model</overrideGraphicsModel><graphicsType>default</graphicsType><attachInput>false</attachInput><attachPhysics>true</attachPhysics><physicsType>character</physicsType><attachScript>true</attachScript><scriptName>entities/marine.lua</scriptName><attachAI>true</attachAI><aifilePath>scripts/ai/marine.lua</aifilePath><aiType>default</aiType></components>"
			)

			case click of 
			(
				1: nodeTM.translation = gridPoint
				2: #stop
			)
		)
	)
)