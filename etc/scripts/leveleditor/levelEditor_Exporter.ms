rootPath = "C:\\Users\\NK\\Development\\viper"

function levelEditor_setupPaths = 
(
	-- Ogre Max 
	sceneSettings = ogremax.GetSceneSettings()
	sceneSettings.MeshPath = "meshes"
	sceneSettings.MaterialPath = "materials"
	sceneSettings.ModelPath = "models"
	sceneSettings.BitmapPath = "textures"
	sceneSettings.CopyBitmaps = true
)

function levelEditor_exportLevel = 
(
	baseDir = rootPath + "\\etc\\data\\"
	baseDir += getFilenameFile( maxfilename ) + "\\entities\\"
	levelName = getFilenameFile( maxfilename )
	
	DosCommand( rootPath + "\\etc\\scripts\\purge.bat " + baseDir )
	
	makeDir( baseDir )
	makeDir( baseDir + "bodies" )
	makeDir( baseDir + "meshes" )
	makeDir( baseDir + "materials" )
	makeDir( baseDir + "models" )
	makeDir( baseDir + "textures" )
	
	hkSceneExportUtility.environmentVariables = "BASEDIR=" + baseDir
	
	for o in geometry do 
	( 
		if ( hvkPhysics_isRigidBody o == true ) then 
		(
			select o
			hkSceneExportUtility.exportSelectedOnly = true
			hkSceneExportUtility.exportScene true
		)
	)
	
	levelEditor_setupPaths()
	ogremax.ExportScene()
	
	-- Level Parse
	scenePath = baseDir + levelName + ".scene"
	
	--DOSCommand( rootPath + "\\etc\\scripts\\generate.bat " + scenePath )
)

function levelEditor_exportModel = 
(
	levelEditor_setupPaths()
	ogremax.ExportScene()
	
	DOSCommand( rootPath + "\\etc\\scripts\\addPaths.bat " + getOpenFileName caption:"Select Scene File:"   )
)

function levelEditor_exportLevelAndPreview = 
(
	levelEditor_ExportLevel()
		
	levelName = getFilenameFile( maxfilename )
	commandPath = rootPath + "\\build\\debug\\bin\\4ha6EW2cru.Applicaton.exe --levelName=" + levelName
	HiddenDOSCommand commandPath startpath:rootPath + "\\build\\debug\\bin" donotwait:true
)