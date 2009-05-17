utility levelEditor_exportUtility "Level Exporter" width:162 height:56
(
	button btn_export "Export" pos:[13,14] width:138 height:29
	on btn_export pressed  do
	(
		levelEditor_exportLevel()
	)
	
	button btn_exportandpreview "Export & Preview" pos:[13,50] width:138 height:29
	on btn_exportandpreview pressed  do
	(
		levelEditor_exportLevelAndPreview()
	)
)