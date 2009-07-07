plugin geometry AIWaypoint
	name: "AI_Waypoint"
	classID: #(0x133067, 0x54376)
	category: "Game Entities"
	extends: Box
(
	tool create
	(
		on mousePoint click do
		(
			delegate.width = delegate.height = delegate.length = 0.5
			
			for i in selection do (
				i.wirecolor = Blue
				
				playerSettings = ogremax.GetobjectSettingsByName( i.name )
				playerSettings.UserDataClass = "components"
				playerSettings.UserData = "<components><attachAI>true</attachAI><aifilePath></aifilePath><aiType>waypoint</aiType><attachGraphics>false</attachGraphics><overrideGraphicsModel></overrideGraphicsModel><graphicsType>default</graphicsType><attachInput>false</attachInput><attachPhysics>false</attachPhysics><physicsType>default</physicsType><attachScript>false</attachScript><scriptName></scriptName><attachSound>false</attachSound></components>"
			)

			case click of 
			(
				1: nodeTM.translation = gridPoint
				2: #stop
			)
		)
	)
)