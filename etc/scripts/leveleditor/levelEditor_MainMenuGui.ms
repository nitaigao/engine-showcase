function levelEditor_resetLevelEditorMenu = 
(
	local mainMenuBar = menuMan.getMainMenuBar()
	
	for i=1 to mainMenuBar.numItems() do
	(
		mItem = mainMenuBar.getItem i
		
		if (mItem.getTitle() == "Level Editor") then
		(
			mainMenuBar.removeItemByPosition i
			menuMan.updateMenuBar()
			exit
		)
	)	
)

function levelEditor_createLevelEditorMenu = 
(
	local levelEditorMenu = menuMan.createMenu "Level Editor"

	
	local item = menuMan.createActionItem "levelEditor_exportLevel" "Level Editor"
	levelEditorMenu.addItem item -1

	item = menuMan.createActionItem "levelEditor_exportLevelAndPreview" "Level Editor"
	levelEditorMenu.addItem item -1

	local mainMenuBar = menuMan.getMainMenuBar()

	local mainMenuItem = menuMan.createSubMenuItem "Level Editor" levelEditorMenu
	local mainMenuItemIndex = mainMenuBar.numitems()

	mainMenuBar.addItem mainMenuItem mainMenuItemIndex

	menuMan.updateMenuBar()
)

levelEditor_resetLevelEditorMenu()
levelEditor_createLevelEditorMenu()