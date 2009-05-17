function levelEditor_exportLevel = 
(
	--baseDir = getSavePath initialDir:"C:\\Users\\NK\\Development\\viper\\etc\\data\\"
	baseDir = "C:\\Users\\NK\\Development\\viper\\etc\\data\\"
	baseDir += "\\" + getFilenameFile( maxfilename ) + "\\entities\\"
	levelName = getFilenameFile( maxfilename )
	
	DosCommand( "C:\\Users\\NK\\Development\\viper\\etc\\scripts\\purge.bat " + baseDir )
	
	makeDir( baseDir )
	makeDir( baseDir + "bodies" )
	makeDir( baseDir + "meshes" )
	makeDir( baseDir + "materials" )
	makeDir( baseDir + "models" )
	makeDir( baseDir + "textures" )
	
	hkSceneExportUtility.environmentVariables = "BASEDIR=" + baseDir
	-- Havok Physics
	for o in geometry do ( 
		if ( hvkPhysics_isRigidBody o ) then 
			select o
			hkSceneExportUtility.exportSelectedOnly = true
			hkSceneExportUtility.exportScene true
	)
	
	-- Ogre Max 
	sceneSettings = ogremax.GetSceneSettings()
	sceneSettings.MeshPath = "meshes"
	sceneSettings.MaterialPath = "materials"
	sceneSettings.ModelPath = "models"
	sceneSettings.BitmapPath = "textures"
	sceneSettings.CopyBitmaps = true
	ogremax.ExportScene()
	
	-- Level Parse
	scenePath = baseDir + levelName + ".scene"
	DOSCommand( "C:\\Users\\NK\\Development\\viper\\etc\\scripts\\generate.bat " + scenePath )
)

function levelEditor_exportLevelAndPreview = 
(
	levelEditor_ExportLevel()
		
	levelName = getFilenameFile( maxfilename )
	commandPath = "C:\\Users\\NK\\Development\\viper\\build\\debug\\bin\\4ha6EW2cru.Applicaton.exe --levelName=" + levelName
	HiddenDOSCommand commandPath startpath:"C:\\Users\\NK\\Development\\viper\\build\\debug\\bin" donotwait:true
)