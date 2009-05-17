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
		levelEditor_ExportLevel()
		
		levelName = getFilenameFile( maxfilename )
		commandPath = "C:\\Users\\NK\\Development\\viper\\build\\debug\\bin\\4ha6EW2cru.Applicaton.exe --levelName=" + levelName
		HiddenDOSCommand commandPath startpath:"C:\\Users\\NK\\Development\\viper\\build\\debug\\bin" donotwait:true
	)
)

