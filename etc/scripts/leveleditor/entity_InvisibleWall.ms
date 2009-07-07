plugin geometry levelEditor_geometry_InvisbleWall
	name: "InvisibleWall"
	classID: #(0x133068, 0x54375)
	category: "Game Entities"
	extends: Plane

(
	tool create
	(
		on mousePoint click do
		(
			delegate.width = 1000
			delegate.length = 300
			
			for i in selection do (
				i.wirecolor = Yellow
				hvkPhysics_createSingleRigidBody $selection
			)

			case click of 
			(
				1: nodeTM.translation = gridPoint
				2: #stop
			)
		)
	)
)