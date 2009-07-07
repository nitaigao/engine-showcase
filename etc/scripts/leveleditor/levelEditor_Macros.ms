macroScript levelEditor_exportLevel 
	buttonText:"Export Level" 
	category:"Level Editor" 
	tooltip:"Exports the level to the file system"
	icon:#("HavokCT",1)
(
	on execute do 
	(
		levelEditor_ExportLevel()
	)
)

macroScript levelEditor_exportLevelAndPreview
	buttonText:"Export Level And Preview" 
	category:"Level Editor" 
	tooltip:"Exports the level to the file system and previews it in the game"
	icon:#("HavokCT",2)
(
	on execute do 
	(
		levelEditor_exportLevelAndPreview()
	)
)

macroScript levelEditor_exportModel
	buttonText:"Export Model" 
	category:"Level Editor" 
	tooltip:"Exports a model to the File System"
	icon:#("HavokCT",3)
(
	on execute do 
	(
		levelEditor_exportModel()
	)
)


